#pragma once

#include <vector>
#include <tuple>

#include "TTree.h"

namespace data
{
    class Info
    {
    private:
        Int_t n_pixel;
        Double_t pixel_dimensions[2];
        Int_t n_subpixel;
        Double_t subpixel_dimensions[2];
        Int_t n_events;
        Int_t beam_width;

        static bool verbose;

    public:
        Info(TTree *info_tree);
        ~Info() = default;

        int get_n_pixel() const { return n_pixel; }
        int get_n_subpixel() const { return n_subpixel; }

        static void set_verbose(bool value) { verbose = value; }
    };

    struct Entry
    {
        Int_t event_id;
        std::vector<Int_t> id_pixel;
        std::vector<Double_t> pixel_energy;
        std::vector<Int_t> id_pixel_cs;
        std::vector<Double_t> pixel_energy_cs;
    };

    class Event
    {
    private:
        static bool verbose;

        Int_t event_id;
        std::vector<Int_t> *id_pixel = nullptr;
        std::vector<Double_t> *pixel_energy = nullptr;
        std::vector<Int_t> *id_pixel_cs = nullptr;
        std::vector<Double_t> *pixel_energy_cs = nullptr;

    public:
        Event(TTree *hits_tree);
        ~Event();

        Entry get_entry();
        void clearEntry(Entry &entry) const;

        static void set_verbose(bool value) { verbose = value; }
    };
} // namespace data
