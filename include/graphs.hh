#pragma once

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

#include "data.hh"

namespace graphs
{
    bool verbose = false;

    TH1D *hist_energy_spectrum = nullptr;
    TH1D *hist_energy_spectrum_cs = nullptr;
    TH1D *hist_total_energy = nullptr;
    TH1D *hist_total_energy_cs = nullptr;
    TH2D *hist_energy_pixels = nullptr;
    TH2D *hist_energy_pixels_cs = nullptr;

    TCanvas *canvas_energy_spectrum;
    TCanvas *canvas_total_energy;
    TCanvas *canvas_energy_pixel;

    void create_histograms(const data::Info &info)
    {
        hist_energy_spectrum = new TH1D("TH1D energy spectrum", "Energy spectrum (no charge sharing)", 100, 0, 0.1);
        hist_energy_spectrum_cs = new TH1D("TH1D energy spectrum CS", "Energy spectrum (with charge sharing)", 100, 0, 0.1);
        hist_total_energy = new TH1D("TH1D total energy", "Total energy (no charge sharing)", 100, 0, 0.1);
        hist_total_energy_cs = new TH1D("TH1D total energy CS", "Total energy (with charge sharing)", 100, 0, 0.1);

        int N = info.n_pixel;
        hist_energy_pixels = new TH2D("TH2D pixel energy", "Energy per pixel (no charge sharing)", N, -0.5, N - 0.5, N, -0.5, N - 0.5);
        hist_energy_pixels_cs = new TH2D("TH2D pixel energy CS", "Energy per pixel (with charge sharing)", N, -0.5, N - 0.5, N, -0.5, N - 0.5);

        if (verbose)
            printf("INFO - Histograms created.\n");
    }

    void fill_histograms(const data::Event &event, const data::Info &info)
    {
        int N = info.n_pixel;

        int n_elements = event.n_elements;
        double total_energy;

        for (int i = 0; i < n_elements; i++)
        {
            double pixel_energy = event.pixel_energy->at(i);
            total_energy += pixel_energy;
            hist_energy_spectrum->Fill(pixel_energy);

            int ID = event.id_pixel->at(i);
            int ID_y = ID / N;
            int ID_x = ID - ID_y * N;
            hist_energy_pixels->Fill(ID_x, ID_y, pixel_energy);
        }
        hist_total_energy->Fill(total_energy);

        if (verbose)
            printf("INFO - Filled histograms (no CS).\n");
    }

    void fill_histograms_cs(const data::Event &event, const data::Info &info)
    {
        int N = info.n_pixel;

        int n_elements = event.n_elements_cs;
        double total_energy;

        for (int i = 0; i < n_elements; i++)
        {
            double pixel_energy = event.pixel_energy_cs->at(i);
            total_energy += pixel_energy;
            hist_energy_spectrum_cs->Fill(pixel_energy);

            int ID = event.id_pixel_cs->at(i);
            int ID_y = ID / N;
            int ID_x = ID - ID_y * N;
            hist_energy_pixels_cs->Fill(ID_x, ID_y, pixel_energy);
        }
        hist_total_energy_cs->Fill(total_energy);

        if (verbose)
            printf("INFO - Filled histograms (with CS).\n");
    }

    void show_histograms()
    {
        canvas_energy_spectrum = new TCanvas("Canvas E spectrum", "Energy Spectrum", 1000, 500);
        canvas_energy_spectrum->Divide(2, 1);
        canvas_energy_spectrum->cd(1);
        hist_energy_spectrum->Draw();
        canvas_energy_spectrum->cd(2);
        hist_energy_spectrum_cs->Draw();

        canvas_total_energy = new TCanvas("Canvas total E", "Total Energy", 1000, 500);
        canvas_total_energy->Divide(2, 1);
        canvas_total_energy->cd(1);
        hist_total_energy->Draw();
        canvas_total_energy->cd(2);
        hist_total_energy_cs->Draw();

        canvas_energy_pixel = new TCanvas("Canvas E spectrum", "Energy Spectrum", 1000, 500);
        canvas_energy_pixel->Divide(2, 1);
        canvas_energy_pixel->cd(1);
        hist_energy_pixels->Draw();
        canvas_energy_pixel->cd(2);
        hist_energy_pixels_cs->Draw();
    }

    void destroy_histograms()
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
} // namespace graphs
