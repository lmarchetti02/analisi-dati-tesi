#pragma once

#include <iostream>
#include <vector>

#include "TTree.h"

using std::cout;
using std::endl;

namespace data
{
    class Info
    {
    private:
        int n_pixel;
        double pixel_dimensions[2];
        int n_subpixel;
        double subpixel_dimensions[2];
        int n_events;

        static bool verbose;

    public:
        Info(TTree *info_tree);
        ~Info() = default;

        int get_n_pixel() const { return n_pixel; }

        static void set_verbose(bool value) { verbose = value; }
    };

    class Event
    {
    private:
        static bool verbose;

    protected:
        int event_id;
        std::vector<int> *id_pixel = nullptr;
        std::vector<double> *pixel_energy = nullptr;
        std::vector<int> *id_pixel_cs = nullptr;
        std::vector<double> *pixel_energy_cs = nullptr;

    public:
        Event() = default;
        Event(TTree *hits_tree);
        ~Event();

        static void set_verbose(bool value) { verbose = value; }
    };
} // namespace data
