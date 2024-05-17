#pragma once

#include "TFile.h"
#include "data.hh"
#include "graphs.hh"
#include "pixel_collection.hh"

#include <memory>

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
    std::shared_ptr<data::Info> info;
    std::unique_ptr<data::Event> event;
    std::unique_ptr<graphs::Histograms> hist;

    std::unique_ptr<TFile> results_file;
    TTree *info_tree, *event_tree;

    std::unique_ptr<pixel::PixelCollection> pixel_collection;

    void show_results() const;

  public:
    Analysis();
    ~Analysis();

    void get_trees();
    void run() const;
};
} // namespace analysis
