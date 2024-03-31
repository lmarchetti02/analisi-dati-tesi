#pragma once

#include <exception>
#include <memory>

#include "TFile.h"

#include "data.hh"
#include "graphs.hh"
#include "constants.hh"

namespace analysis
{
    /**
     * Class for running the analysis
     * of the results obtained from the
     * Geant4 simulation.
     */
    class Analysis
    {
    private:
        std::unique_ptr<data::Info> info;
        std::unique_ptr<data::Event> event;
        std::unique_ptr<graphs::Histograms> hist;

        std::unique_ptr<TFile> results_file;
        TTree *info_tree, *event_tree;

    public:
        Analysis();
        ~Analysis();

        void get_trees();
        void run();
    };
} // namespace analysis