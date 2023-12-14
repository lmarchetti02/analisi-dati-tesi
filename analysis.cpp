#include <iostream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include "include/data.hh"
#include "include/graphs.hh"

using std::cin;
using std::cout;

void analysis(const char *file_name, const char *verbosity = "")
{
    // SET VERBOSITY
    // -------------------------------------------------------------------
    bool verbose = false;
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
    data::Info info;
    data::Event event;

    data::read_simulation_info(info_tree, info);
    data::read_hits(hits_tree, event);

    graphs::create_histograms(info);
    // -------------------------------------------------------------------

    // DISPLAY RESULTS
    // -------------------------------------------------------------------
    std::string choice = " ";
    for (int i = 0; i < hits_tree->GetEntries(); i++)
    {
        if (choice == 's')
            break;

        hits_tree->GetEntry(i);
        graphs::fill_histograms(event, info);
        graphs::fill_histograms_cs(event, info);

        if (choice != 'g')
        {
            cout << "\n";
            cout << "Type:\n";
            cout << "- 's' to stop\n";
            cout << "- 'g' to go until the end (no print)\n";
            cout << "- anything else to continue\n";
            std::getline(cin, choice);
            if (choice.length() == 0)
                continue;
        }
    }
    // graphs::show_histograms();

    // -------------------------------------------------------------------

    // CLEANUP
    // -------------------------------------------------------------------
    // graphs::destroy_histograms();

    results_file->Close();
    delete results_file;
    results_file = nullptr;
    // -------------------------------------------------------------------
}