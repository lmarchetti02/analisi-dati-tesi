#pragma once

#include <memory>

#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "THStack.h"

#include "data.hh"
#include "pixel_collection.hh"

namespace graphs
{
    /**
     * Class for storing the histograms
     * onto which to show the data obtained
     * from the Geant4 simulation.
     */
    class Histograms
    {
    private:
        int n_pixel;

        TH1D *hist_energy_spectrum;
        TH1D *hist_energy_spectrum_cs;

        TH1D *hist_total_energy;
        TH1D *hist_total_energy_cs;

        TH2D *hist_energy_pixels;
        TH2D *hist_energy_pixels_cs;

        TH1D *hist_energy_central;
        TH1D *hist_energy_t;
        TH1D *hist_energy_tr;
        TH1D *hist_energy_sum;

        TCanvas *canvas_energy_spectrum;
        TCanvas *canvas_total_energy;
        TCanvas *canvas_energy_pixel;
        TCanvas *canvas_cross_talk;
        TCanvas *canvas_reconstruction;

        TH1D *hist_energy_central_corrected;
        THStack *hist_stack_corrections;

        std::shared_ptr<data::PSFInfo> psf_info;

        static bool verbose;

        void fill_psf_histograms(int id, double energy);

    public:
        Histograms(int n_pixel, std::shared_ptr<data::PSFInfo> psf);
        ~Histograms();

        void fill_histograms(std::vector<Int_t> v_id, std::vector<Double_t> v_energy, bool CS, bool print = false);
        void fill_results(std::vector<Int_t> v_counts);
        void show_histograms();

        // Set verbosity of the class.
        static void set_verbose(bool value) { verbose = value; }
    };
} // namespace graphs
