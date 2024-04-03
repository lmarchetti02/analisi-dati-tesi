#include "data.hh"

#include "constants.hh"

bool data::Info::verbose = false;
bool data::Event::verbose = false;

/**
 * Function for filling the IDs
 * of the 0, T and TR pixels.
 */
void data::PSFInfo::get_ids(int n_pixel)
{
    id_pixel_0 = (n_pixel / 2) * (1 + n_pixel);
    id_pixel_t = {id_pixel_0 - 15, id_pixel_0 - 1, id_pixel_0 + 1, id_pixel_0 + 15};
    id_pixel_tr = {id_pixel_0 - 16, id_pixel_0 - 14, id_pixel_0 + 14, id_pixel_0 + 16};

    printf("Pixel 0: %i\n", id_pixel_0);

    printf("Pixel T: ");
    for (int i : id_pixel_t)
        printf("%i ", i);

    printf("\nPixel TR: ");
    for (int i : id_pixel_tr)
        printf("%i ", i);
    printf("\n");
}

/**
 * The default constructor.
 *
 * Loads the information about the simulation and stores it into
 * the data members of the class.
 *
 * @param[in] info_tree The pointer to the TTree from which to load the simulation info.
 */
data::Info::Info(TTree *info_tree)
{
    // load info
    info_tree->SetBranchAddress("Pixel_N", &n_pixel);
    info_tree->SetBranchAddress("Pixels_xy_dim", &pixel_dimensions[0]);
    info_tree->SetBranchAddress("Pixels_z_dim", &pixel_dimensions[1]);
    info_tree->SetBranchAddress("Subpixel_N", &n_subpixel);
    info_tree->SetBranchAddress("Subpixels_xy_dim", &subpixel_dimensions[0]);
    info_tree->SetBranchAddress("Subpixels_z_dim", &subpixel_dimensions[1]);
    info_tree->SetBranchAddress("Event_N", &n_events);
    info_tree->SetBranchAddress("Beam_Width", &beam_width);
    info_tree->SetBranchAddress("Beam_Type", &beam_type);

    printf("%sINFO - Loaded simulation info from tree.%s\n\n", INFO_COLOR, END_COLOR);

    // store info
    info_tree->GetEntry(0);
    printf("%sINFO ABOUT THE SIMULATION\n", BOLD);
    printf("-------------------------%s\n", END_COLOR);
    printf("Number of pixels = %i\n", n_pixel);
    printf("Pixel x-dimension = %.4f mm\n", pixel_dimensions[0]);
    printf("Pixel y-dimension = %.4f mm\n", pixel_dimensions[0]);
    printf("Pixel z-dimension = %.2f mm\n", pixel_dimensions[1]);
    printf("Number of subpixels = %i\n", n_subpixel);
    printf("Subpixel x-dimension = %.4f mm\n", subpixel_dimensions[0]);
    printf("Subpixel y-dimension = %.4f mm\n", subpixel_dimensions[0]);
    printf("Subpixel z-dimension = %.2f mm\n", subpixel_dimensions[1]);
    printf("Number of events = %i\n", n_events);
    printf("Beam width = %i\n", beam_width);
    printf("Beam type = %i\n", beam_type);

    printf("%s\nINFO ABOUT THE ALGORITHM\n", BOLD);
    printf("------------------------%s\n", END_COLOR);
    psf_info.get_ids(n_pixel);
}

/**
 * The default constructor.
 *
 * Loads the results of the simulation.
 *
 * @param[in] hits_tree The pointer to the TTree where the results of the simulation are stored.
 */
data::Event::Event(TTree *hits_tree)
{
    hits_tree->SetBranchAddress("Event_ID", &event_id);
    hits_tree->SetBranchAddress("Photon_Energy", &photon_energy);
    hits_tree->SetBranchAddress("ID_Merge_NOCS", &id_pixel);
    hits_tree->SetBranchAddress("Energy_Merge_NOCS", &pixel_energy);
    hits_tree->SetBranchAddress("ID_Merge", &id_pixel_cs);
    hits_tree->SetBranchAddress("Energy_Merge", &pixel_energy_cs);

    printf("\n%sINFO - Loaded hits info from tree.%s\n", INFO_COLOR, END_COLOR);
}

/**
 * The destructor.
 *
 * Clears the `std::vector`s used to store the data.
 */
data::Event::~Event()
{
    id_pixel->clear();
    pixel_energy->clear();
    id_pixel_cs->clear();
    pixel_energy_cs->clear();
}

/**
 * Function for returning an entry of the TTree "Event".
 */
data::Entry data::Event::get_entry() const
{
    return {event_id, photon_energy, *id_pixel, *pixel_energy, *id_pixel_cs, *pixel_energy_cs};
}

/**
 * Function for clearing the previous row, before
 * loading a new one.
 */
void data::Event::clearEntry(Entry &entry) const
{
    entry.id_pixel.clear();
    entry.id_pixel_cs.clear();
    entry.pixel_energy.clear();
    entry.pixel_energy_cs.clear();
}
