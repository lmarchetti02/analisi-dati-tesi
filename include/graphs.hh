#pragma once

#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"

#include "data.hh"

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
        TH1D *hist_energy_spectrum;
        TH1D *hist_energy_spectrum_cs;
        TH1D *hist_total_energy;
        TH1D *hist_total_energy_cs;
        TH2D *hist_energy_pixels;
        TH2D *hist_energy_pixels_cs;

        TCanvas *canvas_energy_spectrum;
        TCanvas *canvas_total_energy;
        TCanvas *canvas_energy_pixel;

        static bool verbose;

    public:
        Histograms(int N);
        ~Histograms();

        void fill_histograms(data::Entry entry, int N, bool CS, bool print = false);
        void show_histograms();

        // Set verbosity of the class.
        static void set_verbose(bool value) { verbose = value; }
    };
} // namespace graphs
