#include "graphs.hh"

#include "TRootCanvas.h"
#include "TApplication.h"

#include "constants.hh"
#include "options.hh"

bool graphs::Histograms::verbose = false;

/**
 * The default constructor.
 *
 * It creates the histograms, which are data members of the class.
 *
 * @param[in] N The number of pixels per side of the array.
 */
graphs::Histograms::Histograms(int n_pixel, int n_subpixel)
{
    using namespace options;

    hist_energy_spectrum = new TH1D("TH1D energy spectrum", "Energy spectrum (no charge sharing)", 100, 0, 0.1);
    hist_energy_spectrum_cs = new TH1D("TH1D energy spectrum CS", "Energy spectrum (with charge sharing)", 100, 0, 0.1);
    hist_energy_spectrum_merge = new TH1D("TH1D energy spectrum CS (merged)", "Energy spectrum (merged)", 100, 0, 0.1);

    hist_total_energy = new TH1D("TH1D total energy", "Total energy (no charge sharing)", 100, 0, 0.1);
    hist_total_energy_cs = new TH1D("TH1D total energy CS", "Total energy (with charge sharing)", 100, 0, 0.1);
    hist_total_energy_merge = new TH1D("TH1D total energy CS (merged)", "Total energy (merged)", 100, 0, 0.1);
    hist_total_energy_escape = new TH1D("TH1D total energy escape", "Total energy (escape)", 100, 0, 0.1);

    hist_energy_pixels = new TH2D("TH2D pixel energy", "Energy per pixel (no charge sharing)", n_subpixel, -0.5, n_subpixel - 0.5, n_subpixel, -0.5, n_subpixel - 0.5);
    hist_energy_pixels_cs = new TH2D("TH2D pixel energy CS", "Energy per pixel (with charge sharing)", n_subpixel, -0.5, n_subpixel - 0.5, n_subpixel, -0.5, n_subpixel - 0.5);
    hist_energy_pixels_merge = new TH2D("TH2D pixel energy CS (merged)", "Energy per pixel (merged)", n_pixel, -0.5, n_pixel - 0.5, n_pixel, -0.5, n_pixel - 0.5);

    hist_energy_central = new TH1D("TH1D central pixel energy", "Energy in the central pixel", 100, 0, Options::get_instance().get_max_threshold());
    hist_energy_t = new TH1D("TH1D T pixels energy", "Energy in the T pixels", 100, 0, Options::get_instance().get_max_threshold());
    hist_energy_tr = new TH1D("TH1D TR pixels energy", "Energy in the TR pixels", 100, 0, Options::get_instance().get_max_threshold());

    printf("%sINFO - Histograms created.%s\n\n", INFO_COLOR, END_COLOR);
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

    delete hist_energy_spectrum_merge;
    hist_energy_spectrum_merge = nullptr;

    delete hist_total_energy;
    hist_total_energy = nullptr;

    delete hist_total_energy_cs;
    hist_total_energy_cs = nullptr;

    delete hist_total_energy_merge;
    hist_total_energy_merge = nullptr;

    delete hist_energy_pixels;
    hist_energy_pixels = nullptr;

    delete hist_energy_pixels_cs;
    hist_energy_pixels_cs = nullptr;

    delete hist_energy_pixels_merge;
    hist_energy_pixels_merge = nullptr;

    if (verbose)
        printf("INFO - Histograms destroyed.\n");
}

/**
 * Function for adding entries to the histograms.
 *
 * @param[in] v_id The vector containing the pixel IDs.
 * @param[in] v_energy The vector containing the pixel energies.
 * @param[in] n_subpixel The number of subpixels per side.
 * @param[in] CS Whether to fill the charge sharing histograms or the normal ones.
 * @param[in] print Whether to print the entry to the terminal.
 */
void graphs::Histograms::fill_histograms(std::vector<Int_t> v_id, std::vector<Double_t> v_energy, int n_subpixel, bool CS, bool print)
{
    double total_energy = 0;
    int limit = v_energy.size();
    for (int i = 0; i < limit; i++)
    {
        double energy = v_energy.at(i);

        total_energy += energy;
        (!CS) ? hist_energy_spectrum->Fill(energy) : hist_energy_spectrum_cs->Fill(energy);

        int ID = v_id.at(i);
        int ID_y = ID / n_subpixel;
        int ID_x = ID - ID_y * n_subpixel;
        if (verbose)
            printf("%sDEBUG - ID_x = %i; ID_y = %i%s\n", DEBUG_COLOR, ID_x, ID_y, END_COLOR);

        (!CS) ? hist_energy_pixels->Fill(ID_x, ID_y, energy) : hist_energy_pixels_cs->Fill(ID_x, ID_y, energy);

        if (print)
            printf("ID = %i; Energy = %f GeV\n", ID, energy);
    }
    (!CS) ? hist_total_energy->Fill(total_energy) : hist_total_energy_cs->Fill(total_energy);

    if (print)
        printf("Total energy = %f GeV\n", total_energy);

    if (verbose)
    {
        printf("%s\nINFO - Filled histograms ", INFO_COLOR);
        (!CS) ? printf("(no CS). %s\n", END_COLOR) : printf("(with CS). %s\n", END_COLOR);
    }
}

/**
 * Function for adding entries to the merged histograms.
 *
 * @param[in] v_id The vector containing the pixel IDs.
 * @param[in] v_energy The vector containing the pixel energies.
 * @param[in] n_pixel The number of pixels per side.
 * @param[in] print Whether to print the entry to the terminal.
 */
void graphs::Histograms::fill_histograms_merged(std::vector<Int_t> v_id, std::vector<Double_t> v_energy, int n_pixel, bool print)
{
    double total_energy = 0;
    int limit = v_energy.size();
    for (int i = 0; i < limit; i++)
    {
        double energy = v_energy.at(i);

        total_energy += energy;
        hist_energy_spectrum_merge->Fill(energy);

        int ID = v_id.at(i);
        int ID_y = ID / n_pixel;
        int ID_x = ID - ID_y * n_pixel;
        if (verbose)
            printf("%sDEBUG - ID_x = %i; ID_y = %i%s\n", DEBUG_COLOR, ID_x, ID_y, END_COLOR);

        hist_energy_pixels_merge->Fill(ID_x, ID_y, energy);

        if (print)
            printf("ID = %i; Energy = %f GeV\n", ID, energy);
    }
    hist_total_energy_merge->Fill(total_energy);

    if (print)
        printf("Total energy = %f GeV\n", total_energy);

    if (verbose)
        printf("%s\nINFO - Filled histograms (merged)%s\n", INFO_COLOR, END_COLOR);
}

/**
 * Function for adding entries to the histogram showing the energy
 * that escapes the detector.
 *
 * @param[in] v_energy The vector containing energy.
 * @param[in] print Whether to print the entry to the terminal.
 */
void graphs::Histograms::fill_histogram_escape(std::vector<Double_t> v_energy, bool print)
{
    Double_t total_energy = 0;
    for (Double_t energy : v_energy)
    {
        total_energy += energy;
    }
    hist_total_energy_escape->Fill(total_energy);

    if (print)
        printf("Total energy = %f GeV\n", total_energy);
}

/**
 * Function for displaying the histograms at the end of the program.
 */
void graphs::Histograms::show_histograms()
{
    canvas_energy_spectrum = new TCanvas("Canvas E spectrum", "Energy Spectrum", 1400, 500);
    canvas_energy_spectrum->Divide(3, 1);
    canvas_energy_spectrum->cd(1);
    hist_energy_spectrum->Draw();
    hist_energy_spectrum->SetDirectory(nullptr);
    canvas_energy_spectrum->cd(2);
    hist_energy_spectrum_cs->Draw();
    hist_energy_spectrum_cs->SetDirectory(nullptr);
    canvas_energy_spectrum->cd(3);
    hist_energy_spectrum_merge->Draw();
    hist_energy_spectrum_merge->SetDirectory(nullptr);
    canvas_energy_spectrum->Update();

    canvas_total_energy = new TCanvas("Canvas total E", "Total Energy", 900, 900);
    canvas_total_energy->Divide(2, 2);
    canvas_total_energy->cd(1);
    hist_total_energy->Draw();
    hist_total_energy->SetDirectory(nullptr);
    canvas_total_energy->cd(2);
    hist_total_energy_cs->Draw();
    hist_total_energy_cs->SetDirectory(nullptr);
    canvas_total_energy->cd(3);
    hist_total_energy_merge->Draw();
    hist_total_energy_merge->SetDirectory(nullptr);
    canvas_total_energy->cd(4);
    hist_total_energy_escape->Draw();
    hist_total_energy_escape->SetDirectory(nullptr);
    canvas_total_energy->Update();

    canvas_energy_pixel = new TCanvas("Canvas pixel E", "Energy per pixel", 1400, 500);
    canvas_energy_pixel->Divide(3, 1);
    canvas_energy_pixel->cd(1);
    hist_energy_pixels->Draw();
    hist_energy_pixels->SetDirectory(nullptr);
    canvas_energy_pixel->cd(2);
    hist_energy_pixels_cs->Draw();
    hist_energy_pixels_cs->SetDirectory(nullptr);
    canvas_energy_pixel->cd(3);
    hist_energy_pixels_merge->Draw();
    hist_energy_pixels_merge->SetDirectory(nullptr);
    canvas_energy_pixel->Update();

    canvas_cross_talk = new TCanvas("Canvas cross-talk", "Study of cross-talk", 1400, 500);
    canvas_cross_talk->Divide(3, 1);
    canvas_cross_talk->cd(1);
    hist_energy_central->Draw();
    hist_energy_central->SetDirectory(nullptr);
    canvas_cross_talk->cd(2);
    hist_energy_t->Draw();
    hist_energy_t->SetDirectory(nullptr);
    canvas_cross_talk->cd(3);
    hist_energy_tr->Draw();
    hist_energy_tr->SetDirectory(nullptr);
    canvas_cross_talk->Update();

    if (verbose)
        printf("%sINFO - Canvases created.%s\n", INFO_COLOR, END_COLOR);

    TRootCanvas *rc = static_cast<TRootCanvas *>(canvas_energy_spectrum->GetCanvasImp());
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
}