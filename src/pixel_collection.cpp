#include "pixel_collection.hh"

#include "options.hh"

bool pixel::PixelCollection::verbose = false;

/**
 * The default constructor.
 *
 * @param[in] psf The pointer to the structure containing the info about
 * the point spread function to consider.
 */
pixel::PixelCollection::PixelCollection(std::shared_ptr<data::PSFInfo> psf) : psf_info(psf)
{
    options::Options &opt = options::Options::get_instance();

    bin_size = opt.get_threshold_step();
    int n_thr = opt.get_n_thresholds();

    for (int i = 0; i < n_thr; i++)
    {
        energy_measured[0].push_back(0);
        energy_measured[1].push_back(0);

        energy_corrected[0].push_back(0);
        energy_corrected[1].push_back(0);

        counts_and[0].push_back(std::vector<int>(n_thr, 0));
    }

    for (int i = 0; i < event_counts.size(); i++)
        event_counts[i].type = i;
}

/**
 * Function for filling the energy bins
 * of the pixel with the counts.
 *
 * @param[in] energy The energy of the event.
 * @param[in] type The type of pixel to consider (0 (0), 1 (T), 2 (TR),...).
 */
void pixel::PixelCollection::fill_collection(double energy, int type)
{
    // add count
    energy_measured[type][get_bin(energy)]++;

    constexpr std::array<const char *, 2> debug = {"0", "T"};
    if (verbose)
        printf("%sDEBUG - Pixel %s - Energy %.4f GeV - Bin %i%s\n", DEBUG_COLOR, debug[type], energy, get_bin(energy), END_COLOR);
}

/**
 * Function for printing the correlations
 * matrix between the energy bins.
 */
void pixel::PixelCollection::print_correlations() const
{
    printf("CORRELATIONS 0-T\n");
    printf("----------------\n");
    for (int i = 0; i < counts_and[0].size(); i++)
    {
        if (!i)
        {
            printf("     ");
            for (int i = 0; i < counts_and[0][0].size(); i++)
                printf("%6i ", i);
            printf("\n     ");
            for (int i = 0; i < counts_and[0][0].size(); i++)
                printf("------ ");

            printf("\n");
        }

        printf("%2i | ", i);
        for (int d : counts_and[0][i])
            printf("%6i ", d);

        printf("\n");
    }
}

/**
 * Function for adding an event to the
 * pixel collection.
 *
 * @param[in] v_id The vector with the IDs of the pixels.
 * @param[in] v_energy The vector with the energy deposited in the pixels.
 */
void pixel::PixelCollection::add_event(std::vector<int> v_id, std::vector<double> v_energy)
{
    for (auto &e : event_counts)
        e.reset();

    for (int i = 0; i < v_id.size(); i++)
    {
        int id = v_id[i];
        double energy = v_energy[i];
        double bin = get_bin(energy);

        if (id == psf_info->id_pixel_0)
        {
            fill_collection(energy, 0);
            event_counts[0].bin = bin;
        }
        else if (id == psf_info->id_pixel_t[0])
        {
            fill_collection(energy, 1);
            event_counts[1].bin = bin;
        }
    }

    if (verbose)
    {
        printf(DEBUG_COLOR);
        for (auto &e : event_counts)
            printf("DEBUG - Pixel %i: bin %i\n", e.type, e.bin);
        printf(END_COLOR);
    }

    if (event_counts[0].bin >= 0 && event_counts[1].bin >= 0)
        counts_and[0][event_counts[0].bin][event_counts[1].bin]++;

    if (verbose)
    {
        printf(DEBUG_COLOR);
        print_correlations();
        printf(END_COLOR);
    }
}

/**
 * Function for reconstruction the energy spectrum
 * of the pixels (right now just 0).
 *
 * @param[in] beam_width The type of illumination: 0 for central pixel, 1 for the whole array.
 */
void pixel::PixelCollection::reconstruct_spectrum(int beam_width)
{
    int N = options::Options::get_instance().get_n_thresholds();
    int mult = (!beam_width) ? 4 : 2;

    for (int i = 0; i < N; i++)
    {
        int correction_1 = 0;
        int correction_2 = 0;

        for (int j = 0; j < i; j++)
            correction_1 += mult * counts_and[0][i - j - 1][j];

        for (int j = i + 1; j < N; j++)
            correction_2 += mult * counts_and[0][i][j - i - 1];

        energy_corrected[0][i] = energy_measured[0][i] + correction_1 - correction_2;
    }
}

/**
 * Function for printing the results
 * of the algorithm.
 */
void pixel::PixelCollection::print_counts() const
{
    printf("PIXEL 0\n");
    printf("-------\n");
    for (int i = 0; i < energy_measured[0].size(); i++)
        printf("Bin %i: %i counts\n", i, energy_measured[0][i]);

    printf("\n");

    printf("PIXEL T\n");
    printf("-------\n");
    for (int i = 0; i < energy_measured[1].size(); i++)
        printf("Bin %i: %i counts\n", i, energy_measured[1][i]);

    printf("\n");

    print_correlations();

    printf("\n");

    printf("PIXEL 0 CORRECTED\n");
    printf("-----------------\n");
    for (int i = 0; i < energy_corrected[0].size(); i++)
        printf("Bin %i: %i counts\n", i, energy_corrected[0][i]);
}
