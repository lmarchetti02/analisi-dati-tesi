#include <iostream>

#include "TFile.h"
#include "TTree.h"

using std::cout;

void analysis(const char *file_name)
{
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
    int n_events = hits_tree->GetEntries();
    cout << "Number of events: " << n_events << "\n";

    results_file->Close();
    delete results_file;
    results_file = nullptr;
    // -------------------------------------------------------------------
}