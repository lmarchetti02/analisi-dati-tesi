#include <iostream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"

#include "include/data.hh"
#include "include/graphs.hh"

using std::cin;
using std::cout;

bool verbose = false;

void analysis(const char *file_name, const char *verbosity = "")
{
    // SET VERBOSITY
    // -------------------------------------------------------------------
    if (!strcmp(verbosity, "-v"))
    {
        data::verbose = true;
        graphs::verbose = true;
        verbose = true;
    }
    // -------------------------------------------------------------------

    // OPEN FILE & GET TREES
    // -------------------------------------------------------------------
    TFile *results_file = new TFile(file_name, "READ");
    if (!results_file->IsOpen())
    {
        cout << "Error opening file " << file_name << "\n";
        return;
    }

    TTree *info_tree, *hits_tree;
    info_tree = static_cast<TTree *>(results_file->Get("Info"));
    if (!info_tree)
    {
        cout << "Error loading TTree Info\n";
        return;
    }
    hits_tree = static_cast<TTree *>(results_file->Get("Event"));
    if (!hits_tree)
    {
        cout << "Error loading TTree Event\n";
        return;
    }
    // -------------------------------------------------------------------

    // GET DATA FROM TREES
    // -------------------------------------------------------------------
    data::Info info = data::read_simulation_info(info_tree);
    data::Event event = data::read_hits(hits_tree);
    graphs::Histograms hist;
    hist.create_histograms(info.n_pixel);
    // -------------------------------------------------------------------

    // DISPLAY RESULTS
    // -------------------------------------------------------------------
    std::string choice = " ";
    for (int i = 0; i < hits_tree->GetEntries(); i++)
    {
        if (choice == 's')
            break;

        hits_tree->GetEntry(i);
        cout << "Entry number = " << i << "\n";

        hist.fill_histograms(event, info.n_pixel);
        hist.fill_histograms_cs(event, info.n_pixel);

        if (choice != 'g')
        {
            // NO CHARGE SHARING
            cout << "Event ID = " << event.event_id << "\n";
            double total_energy = 0;
            for (int j = 0; j < event.id_pixel->size(); j++)
            {
                cout << "ID = " << event.id_pixel->at(j) << "; Energy = " << event.pixel_energy->at(j) << " GeV\n";
                total_energy += event.pixel_energy->at(j);
            }
            cout << "Total energy = " << total_energy << " GeV\n";
            cout << "\n";

            // CHARGE SHARING
            cout << "Event ID = " << event.event_id << "\n";
            double total_energy_cs = 0;
            for (int j = 0; j < event.id_pixel_cs->size(); j++)
            {
                cout << "ID = " << event.id_pixel_cs->at(j) << "; Energy = " << event.pixel_energy_cs->at(j) << " GeV\n";
                total_energy_cs += event.pixel_energy_cs->at(j);
            }
            cout << "Total energy = " << total_energy_cs << " GeV\n";
            cout << "\n";

            // CHOICE
            cout << "Type:\n";
            cout << "- 's' to stop\n";
            cout << "- 'g' to go until the end (no print)\n";
            cout << "- anything else to continue\n";
            std::getline(cin, choice);
            if (choice.length() == 0)
                continue;
        }
    }
    hist.show_histograms();
    // -------------------------------------------------------------------
}
