#pragma once

#include <array>
#include <vector>

#include "data.hh"
#include "constants.hh"

namespace pixel
{
    /**
     *
     */
    struct EventCounts
    {
        int type;
        int bin;

        void reset() { bin = -1; }
    };

    /**
     * Class used for reconstructing
     * the energy spectrum of the
     * photons used in the simulation.
     */
    class PixelCollection
    {
    private:
        double bin_size{};

        std::array<std::vector<int>, MAX_PSF_ELEMENTS> energy_measured;
        std::array<std::vector<int>, MAX_PSF_ELEMENTS> energy_corrected;
        std::array<std::vector<std::vector<int>>, MAX_PSF_ELEMENTS - 1> counts_and;

        std::shared_ptr<data::PSFInfo> psf_info;

        std::array<EventCounts, MAX_PSF_ELEMENTS> event_counts;

        static bool verbose;

        int get_bin(double energy) const { return energy / bin_size; }
        void fill_collection(double energy, int type);

        void print_correlations() const;

    public:
        PixelCollection(std::shared_ptr<data::PSFInfo> psf);
        ~PixelCollection() = default;

        void add_event(std::vector<int> v_id, std::vector<double> v_energy);
        void reconstruct_spectrum(int beam_width);
        void print_counts() const;

        // Get the reconstructed spectrum
        std::vector<int> get_energy_corrected() const { return energy_corrected[0]; }

        // Set verbosity of the class
        static void set_verbose(bool value) { verbose = value; }
    };

} // namespace pixel
