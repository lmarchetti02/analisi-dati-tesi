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

    printf("INFO - Loaded simulation info from tree.\n");

    info_tree->GetEntry(0);
    cout << "Number of pixels = " << n_pixel << "\n";
    cout << "Pixel x-dimension = " << pixel_dimensions[0] << "\n";
    cout << "Pixel y-dimension = " << pixel_dimensions[0] << "\n";
    cout << "Pixel z-dimension = " << pixel_dimensions[1] << "\n";
    cout << "Number of events = " << n_events << "\n";
}

data::Event::Event(TTree *hits_tree)
{
    hits_tree->SetBranchAddress("Event ID", &event_id);
    hits_tree->SetBranchAddress("ID", &id_pixel);
    hits_tree->SetBranchAddress("Energy", &pixel_energy);
    hits_tree->SetBranchAddress("ID CS", &id_pixel_cs);
    hits_tree->SetBranchAddress("Energy CS", &pixel_energy_cs);

    printf("INFO - Loaded hits info from tree.\n");
}

data::Event::~Event()
{
    id_pixel->clear();
    pixel_energy->clear();
    id_pixel_cs->clear();
    pixel_energy_cs->clear();
}
