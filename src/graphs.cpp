#include "graphs.hh"

#include "TRootCanvas.h"
#include "TApplication.h"

bool graphs::Histograms::verbose = false;

graphs::Histograms::Histograms(int N)
{
    hist_energy_spectrum = new TH1D("TH1D energy spectrum", "Energy spectrum (no charge sharing)", 100, 0, 0.1);
    hist_energy_spectrum_cs = new TH1D("TH1D energy spectrum CS", "Energy spectrum (with charge sharing)", 100, 0, 0.1);
    hist_total_energy = new TH1D("TH1D total energy", "Total energy (no charge sharing)", 100, 0, 0.1);
    hist_total_energy_cs = new TH1D("TH1D total energy CS", "Total energy (with charge sharing)", 100, 0, 0.1);

    hist_energy_pixels = new TH2D("TH2D pixel energy", "Energy per pixel (no charge sharing)", N, -0.5, N - 0.5, N, -0.5, N - 0.5);
    hist_energy_pixels_cs = new TH2D("TH2D pixel energy CS", "Energy per pixel (with charge sharing)", N, -0.5, N - 0.5, N, -0.5, N - 0.5);

    printf("INFO - Histograms created.\n");
}

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

    if (verbose)
        printf("INFO - Histograms destroyed.\n");
}

void graphs::Histograms::fill_histograms(int N, bool CS, bool print)
{
    if (print)
        printf("Event ID = %i\n", event_id);

    double total_energy = 0;
    int limit = (!CS) ? pixel_energy->size() : pixel_energy->size();
    for (int i = 0; i < limit; i++)
    {
        double energy = (!CS) ? pixel_energy->at(i) : pixel_energy_cs->at(i);
        total_energy += energy;
        (!CS) ? hist_energy_spectrum->Fill(energy) : hist_energy_spectrum_cs->Fill(energy);

        int ID = (!CS) ? id_pixel->at(i) : id_pixel_cs->at(i);
        int ID_y = ID / N;
        int ID_x = ID - ID_y * N;
        (!CS) ? hist_energy_pixels->Fill(ID_x, ID_y, energy) : hist_energy_pixels_cs->Fill(ID_x, ID_y, energy);

        if (print)
            printf("ID = %i; Energy = %f GeV\n", ID, energy);
    }
    (!CS) ? hist_total_energy->Fill(total_energy) : hist_total_energy_cs->Fill(total_energy);

    if (print)
        printf("Total energy = %f GeV\n", total_energy);

    if (verbose)
        printf("INFO - Filled histograms (no CS).\n");
}

void graphs::Histograms::show_histograms()
{
    canvas_energy_spectrum = new TCanvas("Canvas E spectrum", "Energy Spectrum", 1000, 500);
    canvas_energy_spectrum->Divide(2, 1);
    canvas_energy_spectrum->cd(1);
    hist_energy_spectrum->Draw();
    canvas_energy_spectrum->cd(2);
    hist_energy_spectrum_cs->Draw();
    canvas_energy_spectrum->Update();

    canvas_total_energy = new TCanvas("Canvas total E", "Total Energy", 1000, 500);
    canvas_total_energy->Divide(2, 1);
    canvas_total_energy->cd(1);
    hist_total_energy->Draw();
    canvas_total_energy->cd(2);
    hist_total_energy_cs->Draw();
    canvas_total_energy->Update();

    canvas_energy_pixel = new TCanvas("Canvas pixel E", "Energy per pixel", 1400, 500);
    canvas_energy_pixel->Divide(2, 1);
    canvas_energy_pixel->cd(1);
    hist_energy_pixels->Draw();
    canvas_energy_pixel->cd(2);
    hist_energy_pixels_cs->Draw();
    canvas_energy_pixel->Update();

    if (verbose)
        printf("INFO - Canvases created.\n");

    TRootCanvas *rc = (TRootCanvas *)canvas_energy_spectrum->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
}