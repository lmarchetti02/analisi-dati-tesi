#pragma once

#include "TDataType.h"
#include "constants.hh"
#include "options.hh"

#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

using std::cout;

namespace reference_algorithm
{
typedef std::tuple<std::vector<Int_t>, std::vector<Double_t>> event_info;

/**
 * Function for finding the index of the maximum
 * maximum energy element
 *
 * @param[in] energies The vector with the energies in the pixels.
 *
 * @return The aforementioned index.
 */
inline int max_energy_index(std::vector<Double_t> energies)
{
    Double_t max = energies[0];
    int index = 0;

    for (int i = 0; i < energies.size(); i++) {
        if (energies[i] > max) {
            max = energies[i];
            index = i;
        }
    }

    return index;
}

/**
 * Function for getting the IDs of the 9 pixel
 * which energies are to be summed.
 *
 * The function checks if the central pixel is
 * close to the edge of the array, therefore the final
 * array might not be of 9 elements, but smaller.
 *
 * @param[in] pixel_0 The id of the central pixel.
 * @param[in] n_pixel The number of pixels in the array.
 *
 * @return An array with the surrounding IDs.
 */
inline std::set<Int_t> get_surrounding_ids(Int_t pixel_0, int n_pixel)
{
    int ID_y = pixel_0 / n_pixel;
    int ID_x = pixel_0 - ID_y * n_pixel;

    std::set<Int_t> surrounding_ids{};

    // not very elegant, but fast enough
    if (ID_y > 0) surrounding_ids.insert(pixel_0 - n_pixel);                                     // T
    if (ID_x > 0) surrounding_ids.insert(pixel_0 - 1);                                           // T
    if (ID_x < n_pixel - 1) surrounding_ids.insert(pixel_0 + 1);                                 // T
    if (ID_y < n_pixel - 1) surrounding_ids.insert(pixel_0 + n_pixel);                           // T
    if (ID_y > 0 && ID_x > 0) surrounding_ids.insert(pixel_0 - n_pixel - 1);                     // TR
    if (ID_y > 0 && ID_x < n_pixel - 1) surrounding_ids.insert(pixel_0 - n_pixel + 1);           // TR
    if (ID_y < n_pixel - 1 && ID_x > 0) surrounding_ids.insert(pixel_0 + n_pixel - 1);           // TR
    if (ID_y < n_pixel - 1 && ID_x < n_pixel - 1) surrounding_ids.insert(pixel_0 + n_pixel + 1); // TR

    if (options::Options::get_instance().get_verbosity()) {
        printf("%sDEBUG - Surrounding IDs: ", DEBUG_COLOR);
        for (Int_t i : surrounding_ids)
            printf("%i ", i);
        printf("%s\n", END_COLOR);
    }

    return surrounding_ids;
}

inline event_info cluster(event_info data, int n_pixel)
{
    auto [ids, energies] = data;

    int i_max = max_energy_index(energies);
    Int_t pixel_max = ids[i_max];

    std::set<Int_t> surrounding_ids = get_surrounding_ids(pixel_max, n_pixel);

    std::set<Int_t> common_ids{};
    std::set_intersection(surrounding_ids.begin(), surrounding_ids.end(), ids.begin(), ids.end(),
                          std::inserter(common_ids, common_ids.begin()));

    for (Int_t id : common_ids) {
        for (int i = 0; i < ids.size(); i++) {
            if (ids[i] == id) {
                energies[i_max] += energies[i];
                energies[i] = 0;
            }
        }
    }

    return std::make_tuple(ids, energies);
}
} // namespace reference_algorithm
