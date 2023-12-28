#pragma once

#include <map>

#include "TTree.h"
#include "TFile.h"

#include "data.hh"
#include "constants.hh"

namespace merge_pixel
{

    /**
     * Template for turing an `std::map` into two `std::vector`s,
     * one with all the keys amd one with all the values.
     *
     * @tparam K The type of the keys.
     * @tparam V The type of the values.
     *
     * @param[in] map The `std::map` to convert.
     * @param[in] v_keys The reference to the vector of keys.
     * @param[in] v_values The reference to the vector of values.
     */
    template <typename K, typename V>
    void convert(std::map<K, V> map, std::vector<K> &v_keys, std::vector<V> &v_values)
    {
        for (auto &[key, value] : map)
        {
            v_keys.push_back(key);
            v_values.push_back(value);
        }
    }

    /**
     * Function for merging the subpixels into the corresponding pixels.
     *
     * @param[in] entry The entry from the TTree "Event".
     * @param[in] n_pixel The number of pixel per side.
     * @param[in] n_subpixel The number of subpixels per side.
     * @param[in] verbose Turn verbosity on/off.
     *
     * @return The entry from the TTree "Event" with the merged data.
     */
    data::Entry merge(data::Entry entry, int n_pixel, int n_subpixel, bool verbose)
    {
        std::vector<Int_t> m_id_pixel{};
        std::vector<Double_t> m_pixel_energy{};
        std::vector<Int_t> m_id_pixel_cs{};
        std::vector<Double_t> m_pixel_energy_cs{};

        std::map<Int_t, Double_t> no_cs{};
        std::map<Int_t, Double_t> with_cs{};

        int ratio = n_subpixel / n_pixel;

        // no charge sharing
        for (int i = 0; i < entry.id_pixel.size(); i++)
        {
            int id = entry.id_pixel[i];
            int id_y = id / n_subpixel;
            int id_x = id - id_y * n_subpixel;
            id_x /= ratio;
            id_y /= ratio;
            int id_new = id_y * n_pixel + id_x;

            if (verbose)
                printf("%sDEBUG -- ID = %i; ID_new = %i \n%s", DEBUG_COLOR, id, id_new, END_COLOR);

            if (no_cs.find(id_new) == no_cs.end())
                no_cs.insert(std::make_pair(id_new, entry.pixel_energy[i]));
            else
                no_cs[id_new] += entry.pixel_energy[i];
        }

        // with charge sharing
        for (int i = 0; i < entry.id_pixel_cs.size(); i++)
        {
            int id = entry.id_pixel_cs[i];
            int id_y = id / n_subpixel;
            int id_x = id - id_y * n_subpixel;
            id_x /= ratio;
            id_y /= ratio;
            int id_new = id_y * n_pixel + id_x;

            if (verbose)
                printf("%sDEBUG -- ID = %i; ID_new = %i \n%s", DEBUG_COLOR, id, id_new, END_COLOR);

            if (with_cs.find(id_new) == with_cs.end())
                with_cs.insert(std::make_pair(id_new, entry.pixel_energy_cs[i]));
            else
                with_cs[id_new] += entry.pixel_energy_cs[i];
        }

        m_id_pixel.reserve(no_cs.size());
        m_pixel_energy.reserve(no_cs.size());
        m_id_pixel_cs.reserve(with_cs.size());
        m_pixel_energy_cs.reserve(with_cs.size());

        merge_pixel::convert(no_cs, m_id_pixel, m_pixel_energy);
        merge_pixel::convert(with_cs, m_id_pixel_cs, m_pixel_energy_cs);

        return {entry.event_id, m_id_pixel, m_pixel_energy, m_id_pixel_cs, m_pixel_energy_cs};
    }
} // namespace merge_pixel