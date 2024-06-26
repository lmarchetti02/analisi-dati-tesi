#include "graphs.hh"

#include "TApplication.h"
#include "TRootCanvas.h"
#include "constants.hh"
#include "options.hh"

#include <TH1.h>
#include <algorithm>
#include <array>
#include <iostream>

bool graphs::Histograms::verbose = false;

/**
 * The default constructor.
 *
 * It creates the histograms, which are data members of the class.
 *
 * @param[in] n_pixel The number of pixels per side of the array.
 * @param[in] psf The pointer to the PSFInfo structure.
 */
graphs::Histograms::Histograms(int n_pixel, std::shared_ptr<data::PSFInfo> psf)
    : psf_info(psf)
{
    using namespace options;

    hist_energy_spectrum = new TH1D("TH1D energy spectrum", "Energy spectrum (no charge sharing)", 100, 0, 0.1);
    hist_energy_spectrum_cs = new TH1D("TH1D energy spectrum CS", "Energy spectrum (with charge sharing)", 100, 0, 0.1);

    hist_total_energy = new TH1D("TH1D total energy", "Total energy (no charge sharing)", 100, 0, 0.1);
    hist_total_energy_cs = new TH1D("TH1D total energy CS", "Total energy (with charge sharing)", 100, 0, 0.1);

    hist_energy_pixels = new TH2D("TH2D pixel energy", "Energy per pixel (no charge sharing)", n_pixel, -0.5,
                                  n_pixel - 0.5, n_pixel, -0.5, n_pixel - 0.5);
    hist_energy_pixels_cs = new TH2D("TH2D pixel energy CS", "Energy per pixel (with charge sharing)", n_pixel, -0.5,
                                     n_pixel - 0.5, n_pixel, -0.5, n_pixel - 0.5);

    int N = Options::get_instance().get_n_thresholds();

    hist_energy_central = new TH1D("TH1D central pixel energy", "Energy in the central pixel", N, 0,
                                   Options::get_instance().get_max_threshold());
    hist_energy_t =
        new TH1D("TH1D T pixels energy", "Energy in the T pixels", N, 0, Options::get_instance().get_max_threshold());
    hist_energy_tr =
        new TH1D("TH1D TR pixels energy", "Energy in the TR pixels", N, 0, Options::get_instance().get_max_threshold());

    hist_photon_energy = new TH1D("TH1D photon energy", "Original spectrum of the photon", 50, 0,
                                  Options::get_instance().get_max_threshold());
    hist_energy_central_corrected =
        new TH1D("TH1D reconstructed central pixel energy", "Energy in the central pixel (after reconstruction)", N, 0,
                 Options::get_instance().get_max_threshold());
    hist_energy_central_corrected_reference = new TH1D("TH1D reconstructed central pixel energy (reference)",
                                                       "Energy in the central pixel (after reference algorithm)", N, 0,
                                                       Options::get_instance().get_max_threshold());
    hist_stack_corrections = new THStack("THStack central pixel corrections", "Energy before and after reconstruction");
    hist_stack_corrections_reference =
        new THStack("THStack central pixel corrections", "Energy before and after reconstruction (reference)");

    printf("%sINFO - Histograms created.%s\n\n", INFO_COLOR, END_COLOR);

    this->n_pixel = n_pixel;

    energy_spectrum_file.open("../plots/data/energy_spectrum.txt", std::ios::out);
    if (!energy_spectrum_file.is_open()) throw std::runtime_error("Impossible to open energy spectrum file.");

    energy_spectrum_cs_file.open("../plots/data/energy_spectrum_cs.txt", std::ios::out);
    if (!energy_spectrum_cs_file.is_open()) throw std::runtime_error("Impossible to open energy spectrum file.");

    photon_energy_file.open("../plots/data/photon_energy.txt", std::ios::out);
    if (!photon_energy_file.is_open()) throw std::runtime_error("Impossible to open photon energy file.");

    std::string root_filename = Options::get_instance().get_filename();
    root_filename.erase(root_filename.length() - 9, 9);

    std::string filename_counts = "../plots/data/counts/counts_" + root_filename + ".txt";
    counts_file.open(filename_counts, std::ios::out);
    if (!counts_file.is_open()) throw std::runtime_error("Impossible to open counts file.");

    std::string filename_reconstruction = "../plots/data/counts/reconstruction_" + root_filename + ".txt";
    reconstruction_file.open(filename_reconstruction, std::ios::out);
    if (!reconstruction_file.is_open()) throw std::runtime_error("Impossible to open reconstruction file.");
}

/**
 * The destructor.
 *
 * Deletes all the histograms created.
 */
graphs::Histograms::~Histograms()
{
    delete hist_energy_spectrum;
    hist_energy_spectrum = nullptr;

    delete hist_energy_spectrum_cs;
    hist_energy_spectrum_cs = nullptr;

    delete hist_total_energy;
    hist_total_energy = nullptr;

    delete hist_total_energy_cs;
    hist_total_energy_cs = nullptr;

    delete hist_energy_pixels;
    hist_energy_pixels = nullptr;

    delete hist_energy_pixels_cs;
    hist_energy_pixels_cs = nullptr;

    delete hist_energy_central;
    hist_energy_central = nullptr;

    delete hist_energy_t;
    hist_energy_t = nullptr;

    delete hist_energy_tr;
    hist_energy_tr = nullptr;

    delete hist_photon_energy;
    hist_photon_energy = nullptr;

    delete hist_energy_central_corrected;
    hist_energy_central_corrected = nullptr;

    delete hist_stack_corrections;
    hist_stack_corrections = nullptr;

    if (verbose) print_info("INFO - Histograms destroyed.\n");

    delete canvas_energy;
    canvas_energy = nullptr;

    delete canvas_energy_pixel;
    canvas_energy_pixel = nullptr;

    delete canvas_cross_talk;
    canvas_cross_talk = nullptr;

    delete canvas_reconstruction;
    canvas_reconstruction = nullptr;

    if (verbose) print_info("INFO - Canvases destroyed.\n");

    energy_spectrum_file.close();
    energy_spectrum_cs_file.close();
    photon_energy_file.close();
    counts_file.close();
    reconstruction_file.close();
}

/**
 * Function for filling the histograms
 * with the energy in pixel 0, T and TR.
 *
 * @param[in] id The pixel ID.
 * @param[in] energy The energy in the pixel.
 */
void graphs::Histograms::fill_psf_histograms(int id, double energy)
{
    if (id == psf_info->id_pixel_0) {
        hist_energy_central->Fill(energy);
        counts_file << energy << std::endl;
    } else if (std::find(psf_info->id_pixel_t.begin(), psf_info->id_pixel_t.end(), id) != psf_info->id_pixel_t.end())
        hist_energy_t->Fill(energy);
    else if (std::find(psf_info->id_pixel_tr.begin(), psf_info->id_pixel_tr.end(), id) != psf_info->id_pixel_tr.end())
        hist_energy_tr->Fill(energy);
}

/**
 * Function for adding entries to the histograms.
 *
 * @param[in] v_id The vector containing the pixel IDs.
 * @param[in] v_energy The vector containing the pixel energies.
 * @param[in] CS Whether to fill the charge sharing histograms or the normal ones.
 * @param[in] print Whether to print the entry to the terminal.
 */
void graphs::Histograms::fill_histograms(std::vector<Int_t> v_id, std::vector<Double_t> v_energy, bool CS, bool print)
{
    double total_energy = 0;
    int limit = v_energy.size();
    for (int i = 0; i < limit; i++) {
        double energy = v_energy.at(i);

        if (!CS) energy_spectrum_file << energy << "\n";
        else energy_spectrum_cs_file << energy << "\n";

        total_energy += energy;
        if (energy > 0) (!CS) ? hist_energy_spectrum->Fill(energy) : hist_energy_spectrum_cs->Fill(energy);

        int ID = v_id.at(i);
        int ID_y = ID / n_pixel;
        int ID_x = ID - ID_y * n_pixel;
        if (verbose) printf("%sDEBUG - ID_x = %i; ID_y = %i%s\n", DEBUG_COLOR, ID_x, ID_y, END_COLOR);

        (!CS) ? hist_energy_pixels->Fill(ID_x, ID_y, energy) : hist_energy_pixels_cs->Fill(ID_x, ID_y, energy);

        // fill 0, T and TR histograms
        if (CS) fill_psf_histograms(ID, energy);

        if (print) printf("ID = %i; Energy = %f GeV\n", ID, energy);
    }
    (!CS) ? hist_total_energy->Fill(total_energy) : hist_total_energy_cs->Fill(total_energy);
    if (print) printf("Total energy = %f GeV\n", total_energy);

    if (verbose) {
        printf("%s\nINFO - Filled histograms ", INFO_COLOR);
        (!CS) ? printf("(no CS). %s\n", END_COLOR) : printf("(with CS). %s\n", END_COLOR);
    }
}

/**
 * Function for filling the histogram with
 * the energy spectrum after reconstruction.
 *
 * @param[in] v_count The vector with the counts in each of the energy bins.
 */
void graphs::Histograms::fill_results(std::vector<Int_t> v_counts)
{
    double max = options::Options::get_instance().get_max_threshold();
    int N = options::Options::get_instance().get_n_thresholds();
    double step = max / N;

    // corrected counts
    for (int i = 0; i < N; i++) {
        hist_energy_central_corrected->SetBinContent((i + 1), v_counts[i]);
        reconstruction_file << (i + 1) * step << "  " << v_counts[i] << std::endl;
    }
}

/**
 * Function for filling the histogram with the results
 * of the reference algorithm.
 *
 * @param[in] v_id The vector containing the pixel IDs.
 * @param[in] v_energy The vector containing the pixel energies.
 */
void graphs::Histograms::fill_reference(std::vector<Int_t> v_id, std::vector<Double_t> v_energy)
{
    Int_t id_0 = (n_pixel / 2) * (1 + n_pixel);

    for (int i = 0; i < v_id.size(); i++) {
        if (v_id[i] == id_0 && v_energy[i] > 0) hist_energy_central_corrected_reference->Fill(v_energy[i]);
    }
}

/**
 * Function for filling the histogram with
 * the original energy spectrum.
 *
 * @param[in] energy The energy of the photon.
 */
void graphs::Histograms::fill_photon_energy(Double_t energy)
{
    hist_photon_energy->Fill(energy);
    photon_energy_file << energy << std::endl;
}

/**
 * Function for displaying the histograms at the end of the program.
 */
void graphs::Histograms::show_histograms()
{
    canvas_energy = new TCanvas("Canvas E array", "Energy Spectrum and Total Energy", 1'000, 1'000);
    canvas_energy->Divide(2, 2);
    canvas_energy->cd(1);
    hist_energy_spectrum->Draw();
    hist_energy_spectrum->SetDirectory(nullptr);
    canvas_energy->cd(2);
    hist_energy_spectrum_cs->Draw();
    hist_energy_spectrum_cs->SetDirectory(nullptr);
    canvas_energy->cd(3);
    hist_total_energy->Draw();
    hist_total_energy->SetDirectory(nullptr);
    canvas_energy->cd(4);
    hist_total_energy_cs->Draw();
    hist_total_energy_cs->SetDirectory(nullptr);
    canvas_energy->Update();

    canvas_energy_pixel = new TCanvas("Canvas pixel E", "Energy per pixel", 1'400, 700);
    canvas_energy_pixel->Divide(2, 1);
    canvas_energy_pixel->cd(1);
    hist_energy_pixels->Draw();
    hist_energy_pixels->SetDirectory(nullptr);
    canvas_energy_pixel->cd(2);
    hist_energy_pixels_cs->Draw();
    hist_energy_pixels_cs->SetDirectory(nullptr);
    canvas_energy_pixel->Update();

    // canvas_cross_talk = new TCanvas("Canvas cross-talk", "Study of cross-talk", 1400, 700);
    // canvas_cross_talk->Divide(3, 1);
    // canvas_cross_talk->cd(1);
    // hist_energy_central->Draw();
    // hist_energy_central->SetDirectory(nullptr);
    // canvas_cross_talk->cd(2);
    // hist_energy_t->Draw();
    // hist_energy_t->SetDirectory(nullptr);
    // canvas_cross_talk->cd(3);
    // hist_energy_tr->Draw();
    // hist_energy_tr->SetDirectory(nullptr);
    // canvas_cross_talk->cd(4);
    // canvas_cross_talk->Update();

    canvas_reconstruction = new TCanvas("Canvas reconstruction", "Spectrum Reconstruction", 1'500, 1'000);
    canvas_reconstruction->Divide(3, 2);
    canvas_reconstruction->cd(1);
    hist_energy_central->Draw();
    hist_energy_central->SetDirectory(nullptr);
    canvas_reconstruction->cd(2);
    hist_energy_central_corrected->Draw();
    hist_energy_central_corrected->SetDirectory(nullptr);
    canvas_reconstruction->cd(4);
    hist_photon_energy->Draw();
    hist_photon_energy->SetDirectory(nullptr);
    canvas_reconstruction->cd(5);
    hist_stack_corrections->Add(hist_energy_central);
    TH1D *hist_energy_central_corrected_clone = static_cast<TH1D *>(hist_energy_central_corrected->Clone());
    hist_stack_corrections->Add(hist_energy_central_corrected_clone);
    hist_energy_central_corrected_clone->SetFillColor(2);
    hist_stack_corrections->Draw("nostack");
    canvas_reconstruction->cd(3);
    hist_energy_central_corrected_reference->Draw();
    hist_energy_central_corrected_reference->SetDirectory(nullptr);
    canvas_reconstruction->cd(6);
    hist_stack_corrections_reference->Add(hist_energy_central);
    TH1D *hist_energy_central_corrected_reference_clone =
        static_cast<TH1D *>(hist_energy_central_corrected_reference->Clone());
    hist_stack_corrections_reference->Add(hist_energy_central_corrected_reference_clone);
    hist_energy_central_corrected_reference_clone->SetFillColor(3);
    hist_stack_corrections_reference->Draw("nostack");
    canvas_reconstruction->Update();

    if (verbose) printf("%sINFO - Canvases created.%s\n", INFO_COLOR, END_COLOR);

    TRootCanvas *rc = static_cast<TRootCanvas *>(canvas_energy->GetCanvasImp());
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
}
