#include <iostream>
#include <string>
#include <memory>

#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"

#include "data.hh"
#include "graphs.hh"
#include "constants.hh"

int main(int argc, char **argv)
{
    printf("\033c");
    bool verbose = false; // `main()` verbosity
    TApplication app("app", &argc, argv);

    // get ROOT file name and verbosity
    char file_name[100];
    strcpy(file_name, "../results/");
    strncat(file_name, argv[1], 30);
    const char *verbosity = (argc == 2) ? "" : argv[2];

    // OPEN FILE & GET TREES
    // -------------------------------------------------------------------
    std::unique_ptr<TFile> results_file = std::make_unique<TFile>(file_name, "READ");
    if (!results_file->IsOpen())
    {
        printf("%sError opening file %s%s\n", ERROR_COLOR, file_name, END_COLOR);
        return 1;
    }

    TTree *info_tree, *event_tree; // owned by TFile object
    info_tree = static_cast<TTree *>(results_file->Get("Info"));
    if (!info_tree)
    {
        printf("%sError loading TTree Info%s\n", ERROR_COLOR, END_COLOR);
        return 1;
    }

    event_tree = static_cast<TTree *>(results_file->Get("Event"));
    if (!event_tree)
    {
        printf("%sError loading TTree Event%s\n", ERROR_COLOR, END_COLOR);
        return 1;
    }
    // -------------------------------------------------------------------

    // GET DATA FROM TREES
    // -------------------------------------------------------------------
    data::Info info(info_tree);
    data::Event event(event_tree);
    graphs::Histograms hist(info.get_n_pixel(), info.get_n_subpixel());

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
            printf("%sINFO - %i entries processed.%s\n", INFO_COLOR, i, END_COLOR);

        data::Entry entry = event.get_entry();

        if (choice != 'g')
        {
            (i != 0) ? printf("\033c") : printf("");
            printf("Entry number = %i\n", i);
            printf("Event ID = %i\n\n", entry.event_id);

            printf("%sNO CHARGE SHARING%s\n", BOLD, END_COLOR);
            printf("-----------------\n");
            hist.fill_histograms(entry.id_pixel, entry.pixel_energy, info.get_n_subpixel(), false, true);

            printf("\n%sWITH CHARGE SHARING%s\n", BOLD, END_COLOR);
            printf("-------------------\n");
            hist.fill_histograms(entry.id_pixel_cs, entry.pixel_energy_cs, info.get_n_subpixel(), true, true);

            printf("\n%sWITH CHARGE SHARING (MERGED)%s\n", BOLD, END_COLOR);
            printf("----------------------------\n");
            hist.fill_histograms_merged(entry.id_pixel_merge, entry.pixel_energy_merge, info.get_n_pixel(), true);

            // CHOICE
            printf("\nType:\n");
            printf("- 's' to stop\n");
            printf("- 'g' to go until the end (no print)\n");
            printf("- anything else to continue\n");
            std::getline(std::cin, choice);
            if (choice.length() == 0)
                continue;
        }

        hist.fill_histograms(entry.id_pixel, entry.pixel_energy, info.get_n_subpixel(), false);
        hist.fill_histograms(entry.id_pixel_cs, entry.pixel_energy_cs, info.get_n_subpixel(), true);
        hist.fill_histograms_merged(entry.id_pixel_cs, entry.pixel_energy_cs, info.get_n_pixel());

        event.clearEntry(entry);
    }

    hist.show_histograms();
    // -------------------------------------------------------------------

    results_file->Close();

    app.Run();
    return 0;
}
