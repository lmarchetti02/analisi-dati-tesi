#pragma once

#include <vector>

#include "TTree.h"

namespace data
{
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

        static bool verbose;

    public:
        Info(TTree *info_tree);
        ~Info() = default;

        // Returns the number of pixels.
        int get_n_pixel() const { return n_pixel; }
        // Returns the number of subpixels.
        int get_n_subpixel() const { return n_subpixel; }

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
        std::vector<Int_t> id_pixel;
        std::vector<Double_t> pixel_energy;
        std::vector<Int_t> id_pixel_cs;
        std::vector<Double_t> pixel_energy_cs;
        std::vector<Int_t> id_pixel_merge;
        std::vector<Double_t> pixel_energy_merge;
        std::vector<Double_t> energy_escape;
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
        std::vector<Int_t> *id_pixel = nullptr;
        std::vector<Double_t> *pixel_energy = nullptr;
        std::vector<Int_t> *id_pixel_cs = nullptr;
        std::vector<Double_t> *pixel_energy_cs = nullptr;
        std::vector<Int_t> *id_pixel_merge = nullptr;
        std::vector<Double_t> *pixel_energy_merge = nullptr;
        std::vector<Double_t> *energy_escape = nullptr;

    public:
        Event(TTree *hits_tree);
        ~Event();

        Entry get_entry() const;
        void clearEntry(Entry &entry) const;

        // Set verbosity of the class.
        static void set_verbose(bool value) { verbose = value; }
    };
} // namespace data
