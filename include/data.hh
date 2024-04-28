#pragma once

#include <vector>

#include "TTree.h"

namespace data
{
    /**
     * Structure containing the info
     * about the pixels used for reconstructing
     * the spectrum.
     */
    struct PSFInfo
    {
        int id_pixel_0;
        std::array<int, 4> id_pixel_t;
        std::array<int, 4> id_pixel_tr;

        void get_ids(int n_pixel);
    };

    /**
     * Class for reading and storing the
     * info about the Geant4 simulation.
     */
    class Info
    {
    private:
        Int_t n_pixel{};
        Double_t pixel_dimensions[2]{};
        Int_t n_subpixel{};
        Double_t subpixel_dimensions[2]{};
        Int_t n_events{};
        Int_t beam_width{};
        Int_t beam_type{};

        PSFInfo psf_info{};

        static bool verbose;

    public:
        Info(TTree *info_tree);
        ~Info() = default;

        // Returns the number of pixels.
        int get_n_pixel() const { return n_pixel; }
        // Returns the type of illumination.
        int get_beam_width() const { return beam_width; }
        // Returns the type of spectrum.
        int get_beam_type() const { return beam_type; }
        // Returns the psf info structure.
        std::shared_ptr<PSFInfo> get_psf_info() { return std::make_shared<PSFInfo>(psf_info); }

        // Set verbosity of the class.
        static void set_verbose(bool value) { verbose = value; }
    };

    /**
     * Struct containing single entries
     * of the Event TTree.
     */
    struct Entry
    {
        Int_t event_id;
        Double_t photon_energy;
        std::vector<Int_t> id_pixel;
        std::vector<Double_t> pixel_energy;
        std::vector<Int_t> id_pixel_cs;
        std::vector<Double_t> pixel_energy_cs;
    };

    /**
     * Class for reading and storing the
     * results of the Geant4 simulation.
     */
    class Event
    {
    private:
        static bool verbose;

        Int_t event_id;
        Double_t photon_energy;
        std::vector<Int_t> *id_pixel = nullptr;
        std::vector<Double_t> *pixel_energy = nullptr;
        std::vector<Int_t> *id_pixel_cs = nullptr;
        std::vector<Double_t> *pixel_energy_cs = nullptr;

    public:
        Event(TTree *hits_tree);
        ~Event();

        Entry get_entry() const;
        void clearEntry(Entry &entry) const;

        // Set verbosity of the class.
        static void set_verbose(bool value) { verbose = value; }
    };
} // namespace data
