#pragma once

#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"
#include "data.hh"

#include <fstream>
#include <memory>

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

    TH1D *hist_photon_energy;
    TH1D *hist_energy_central_corrected;
    TH1D *hist_energy_central_corrected_reference;
    THStack *hist_stack_corrections;
    THStack *hist_stack_corrections_reference;

    TCanvas *canvas_energy;
    TCanvas *canvas_energy_pixel;
    TCanvas *canvas_cross_talk;
    TCanvas *canvas_reconstruction;

    std::shared_ptr<data::PSFInfo> psf_info;

    std::fstream energy_spectrum_file;
    std::fstream energy_spectrum_cs_file;
    std::fstream counts_file;
    std::fstream reconstruction_file;
    std::fstream photon_energy_file;

    static bool verbose;

    void fill_psf_histograms(int id, double energy);

  public:
    Histograms(int n_pixel, std::shared_ptr<data::PSFInfo> psf);
    ~Histograms();

    void fill_histograms(std::vector<Int_t> v_id, std::vector<Double_t> v_energy, bool CS, bool print = false);
    void fill_results(std::vector<Int_t> v_counts);
    void fill_reference(std::vector<Int_t> v_id, std::vector<Double_t> v_energy);
    void fill_photon_energy(Double_t energy);
    void show_histograms();

    // Set verbosity of the class.
    static void set_verbose(bool value) { verbose = value; }
};
} // namespace graphs
