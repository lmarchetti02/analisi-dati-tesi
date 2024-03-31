#include "analysis.hh"

#include <iostream>
#include <stdexcept>

#include "TFile.h"
#include "TTree.h"

#include "options.hh"
#include "constants.hh"

/**
 * The default constructor.
 *
 * It allows the user to view and modify the
 * analysis options.
 */
analysis::Analysis::Analysis()
{
    // set options
    options::Options &opt = options::Options::get_instance();

    std::string opt_choice;
    while (true)
    {
        printf("\nType:\n");
        printf("- 's' to start the analysis\n");
        printf("- 'c' to see the current options\n");
        printf("- 'm' to modify the options\n");
        printf("- 'e' to exit\n");
        std::getline(std::cin, opt_choice);

        if (opt_choice.length() == 0 || opt_choice.length() > 1)
            continue;

        if (opt_choice == "e")
            std::exit(0);

        if (opt_choice == "s")
            break;
        else if (opt_choice == "c")
        {
            clear_screen();
            opt.print_options();
        }
        else if (opt_choice == "m")
        {
            clear_screen();
            opt.change_options();
        }
    }
    clear_screen();
}

/**
 * The default destructor.
 */
analysis::Analysis::~Analysis()
{
    results_file->Close();
}

/**
 * Function for opening the results file
 * and read the TTrees stored in it.
 */
void analysis::Analysis::get_trees()
{
    options::Options &opt = options::Options::get_instance();

    // get ROOT file name and verbosity
    char file_name[100];
    strcpy(file_name, "../results/");
    strncat(file_name, opt.get_filename().c_str(), 30);
    bool verbosity = opt.get_verbosity();

    // open file and get trees
    results_file = std::make_unique<TFile>(file_name, "READ");
    if (!results_file->IsOpen())
    {
        printf("%sERROR - Impossible to open %s%s\n", ERROR_COLOR, file_name, END_COLOR);
        throw std::runtime_error("");
    }

    info_tree = static_cast<TTree *>(results_file->Get("Info"));
    if (!info_tree)
    {
        printf("%sERROR - Impossible to load TTree Info %s\n", ERROR_COLOR, END_COLOR);
        throw std::runtime_error("");
    }

    event_tree = static_cast<TTree *>(results_file->Get("Event"));
    if (!event_tree)
    {
        printf("%sERROR - Impossible to load TTree Event %s\n", ERROR_COLOR, END_COLOR);
        throw std::runtime_error("");
    }

    // get data from trees
    info = std::make_unique<data::Info>(info_tree);
    event = std::make_unique<data::Event>(event_tree);
    hist = std::make_unique<graphs::Histograms>(info->get_n_pixel());

    // set verbosity
    if (verbosity)
    {
        data::Info::set_verbose(true);
        data::Event::set_verbose(true);
        graphs::Histograms::set_verbose(true);
    }
}

/**
 * Function for running the data analysis.
 */
void analysis::Analysis::run()
{

    std::string choice = " ";
    for (int i = 0; i < event_tree->GetEntries(); i++)
    {
        if (choice == 's')
            break;

        event_tree->GetEntry(i);
        if (i % 10000 == 0 && i != 0)
            printf("%sINFO - %i entries processed.%s\n", INFO_COLOR, i, END_COLOR);

        data::Entry entry = event->get_entry();

        if (choice != 'g')
        {
            (i != 0) ? printf("\033c") : printf("");
            printf("Entry number = %i\n", i);
            printf("Event ID = %i\n\n", entry.event_id);

            printf("%sNO CHARGE SHARING%s\n", BOLD, END_COLOR);
            printf("-----------------\n");
            hist->fill_histograms(entry.id_pixel, entry.pixel_energy, info->get_n_pixel(), false, true);

            printf("\n%sWITH CHARGE SHARING%s\n", BOLD, END_COLOR);
            printf("-------------------\n");
            hist->fill_histograms(entry.id_pixel_cs, entry.pixel_energy_cs, info->get_n_pixel(), true, true);

            // CHOICE
            printf("\nType:\n");
            printf("- 's' to stop\n");
            printf("- 'g' to go until the end (no print)\n");
            printf("- anything else to continue\n");
            std::getline(std::cin, choice);
            if (choice.length() == 0)
                continue;
        }

        hist->fill_histograms(entry.id_pixel, entry.pixel_energy, info->get_n_pixel(), false);
        hist->fill_histograms(entry.id_pixel_cs, entry.pixel_energy_cs, info->get_n_pixel(), true);

        event->clearEntry(entry);
    }

    hist->show_histograms();
}
