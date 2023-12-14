#pragma once

#include <iostream>

#include "TTree.h"

using std::cout;
using std::endl;

namespace data
{
    bool verbose = false;

    struct Info
    {
        int n_pixel;
        double pixel_dimensions[3];
        int n_events;
    };

    struct Event
    {
        int event_id;
        int n_elements;
        std::vector<int> *id_pixel = nullptr;
        std::vector<double> *pixel_energy = nullptr;
        int n_elements_cs;
        std::vector<int> *id_pixel_cs = nullptr;
        std::vector<double> *pixel_energy_cs = nullptr;
    };

    void read_simulation_info(TTree *info_tree, Info &info)
    {
        info_tree->SetBranchAddress("Pixel N", &info.n_pixel);
        info_tree->SetBranchAddress("Pixels x-dim", &info.pixel_dimensions[0]);
        info_tree->SetBranchAddress("Pixels y-dim", &info.pixel_dimensions[1]);
        info_tree->SetBranchAddress("Pixels z-dim", &info.pixel_dimensions[2]);
        info_tree->SetBranchAddress("Event N", &info.n_events);

        if (verbose)
            printf("INFO - Loaded simulation info from tree.\n");

        info_tree->GetEntry(0);
        cout << "Number of pixels = " << info.n_pixel << "\n";
        cout << "Pixel x-dimension = " << info.pixel_dimensions[0] << "\n";
        cout << "Pixel y-dimension = " << info.pixel_dimensions[1] << "\n";
        cout << "Pixel z-dimension = " << info.pixel_dimensions[2] << "\n";
        cout << "Number of events = " << info.n_events << "\n";
    }

    void read_hits(TTree *hits_tree, Event &event)
    {
        hits_tree->SetBranchAddress("Event ID", &event.event_id);
        hits_tree->SetBranchAddress("N Elements", &event.n_elements);
        hits_tree->SetBranchAddress("ID", &event.id_pixel);
        hits_tree->SetBranchAddress("Energy", &event.pixel_energy);
        hits_tree->SetBranchAddress("N Elements CS", &event.n_elements_cs);
        hits_tree->SetBranchAddress("ID CS", &event.id_pixel_cs);
        hits_tree->SetBranchAddress("Energy CS", &event.pixel_energy_cs);

        if (verbose)
            printf("INFO - Loaded hits info from tree.\n");
    }
} // namespace data
