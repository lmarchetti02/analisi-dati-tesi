#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"

#include "data.hh"
#include "graphs.hh"

int main(int argc, char *argv[])
{
    printf("\033c");
    bool verbose = false;
    TApplication app("app", &argc, argv);

    // get root file name and verbosity
    char file_name[100];
    strcpy(file_name, "../results/");
    strcat(file_name, argv[1]);
    const char *verbosity = (argc == 2) ? "" : argv[2];

    // OPEN FILE & GET TREES
    // -------------------------------------------------------------------
    TFile *results_file = new TFile(file_name, "READ");
    if (!results_file->IsOpen())
    {
        printf("Error opening file %s\n", file_name);
        return 1;
    }

    TTree *info_tree, *event_tree;
    info_tree = static_cast<TTree *>(results_file->Get("Info"));
    if (!info_tree)
    {
        printf("Error loading TTree Info\n");
        return 1;
    }

    event_tree = static_cast<TTree *>(results_file->Get("Event"));
    if (!event_tree)
    {
        printf("Error loading TTree Event\n");
        return 1;
    }
    // -------------------------------------------------------------------

    // GET DATA FROM TREES
    // -------------------------------------------------------------------
    data::Info info(info_tree);
    data::Event event(event_tree);
    graphs::Histograms hist(info.get_n_pixel());

    // set verbosity
    if (!strncmp(verbosity, "-v", 2))
    {
        data::Info::set_verbose(true);
        data::Event::set_verbose(true);
        graphs::Histograms::set_verbose(true);
        verbose = true;
    }
    // -------------------------------------------------------------------

    // DISPLAY RESULTS
    // -------------------------------------------------------------------
    std::string choice = " ";
    for (int i = 0; i < event_tree->GetEntries(); i++)
    {
        if (choice == 's')
            break;

        event_tree->GetEntry(i);
        if (i % 10000 == 0 && i != 0)
            printf("Entry number = %i\n", i);

        data::Entry entry = event.get_entry();

        if (choice != 'g')
        {
            (i != 0) ? printf("\033c") : printf("");
            printf("Entry number = %i\n\n", i);
            printf("NO CHARGE SHARING\n");
            printf("-----------------\n");
            hist.fill_histograms(entry, info.get_n_pixel(), false, true);

            printf("\nWITH CHARGE SHARING\n");
            printf("-------------------\n");
            hist.fill_histograms(entry, info.get_n_pixel(), true, true);

            // CHOICE
            printf("\nType:\n");
            printf("- 's' to stop\n");
            printf("- 'g' to go until the end (no print)\n");
            printf("- anything else to continue\n");
            std::getline(std::cin, choice);
            if (choice.length() == 0)
                continue;
        }

        hist.fill_histograms(entry, info.get_n_pixel(), false);
        hist.fill_histograms(entry, info.get_n_pixel(), true);

        event.clearEntry(entry);
    }

    hist.show_histograms();
    // -------------------------------------------------------------------

    results_file->Close();
    delete results_file;
    results_file = nullptr;

    app.Run();
    return 0;
}
