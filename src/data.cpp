#include "data.hh"

bool data::Info::verbose = false;
bool data::Event::verbose = false;

data::Info::Info(TTree *info_tree)
{
    info_tree->SetBranchAddress("Pixel N", &n_pixel);
    info_tree->SetBranchAddress("Pixels xy-dim", &pixel_dimensions[0]);
    info_tree->SetBranchAddress("Pixels z-dim", &pixel_dimensions[1]);
    info_tree->SetBranchAddress("Subpixel N", &n_subpixel);
    info_tree->SetBranchAddress("Subpixels xy-dim", &subpixel_dimensions[0]);
    info_tree->SetBranchAddress("Subpixels z-dim", &subpixel_dimensions[1]);
    info_tree->SetBranchAddress("Event N", &n_events);
    info_tree->SetBranchAddress("Beam Width", &beam_width);

    printf("INFO - Loaded simulation info from tree.\n\n");

    info_tree->GetEntry(0);
    printf("Number of pixels = %i\n", n_pixel);
    printf("Pixel x-dimension = %.4f mm\n", pixel_dimensions[0]);
    printf("Pixel y-dimension = %.4f mm\n", pixel_dimensions[0]);
    printf("Pixel z-dimension = %.2f mm\n", pixel_dimensions[1]);
    printf("Number of events = %i\n", n_events);
}

data::Event::Event(TTree *hits_tree)
{
    hits_tree->SetBranchAddress("Event ID", &event_id);
    hits_tree->SetBranchAddress("ID", &id_pixel);
    hits_tree->SetBranchAddress("Energy", &pixel_energy);
    hits_tree->SetBranchAddress("ID CS", &id_pixel_cs);
    hits_tree->SetBranchAddress("Energy CS", &pixel_energy_cs);

    printf("\nINFO - Loaded hits info from tree.\n");
}

data::Event::~Event()
{
    id_pixel->clear();
    pixel_energy->clear();
    id_pixel_cs->clear();
    pixel_energy_cs->clear();
}

data::Entry data::Event::get_entry()
{
    return {event_id, *id_pixel, *pixel_energy, *id_pixel_cs, *pixel_energy_cs};
}

void data::Event::clearEntry(Entry &entry) const
{
    entry.id_pixel.clear();
    entry.id_pixel_cs.clear();
    entry.pixel_energy.clear();
    entry.pixel_energy_cs.clear();
}
