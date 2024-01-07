#include "data.hh"

#include "constants.hh"

bool data::Info::verbose = false;
bool data::Event::verbose = false;

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

    printf("%sINFO - Loaded simulation info from tree.%s\n\n", INFO_COLOR, END_COLOR);

    // store info
    info_tree->GetEntry(0);
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
    hits_tree->SetBranchAddress("ID", &id_pixel);
    hits_tree->SetBranchAddress("Energy", &pixel_energy);
    hits_tree->SetBranchAddress("ID_CS", &id_pixel_cs);
    hits_tree->SetBranchAddress("Energy_CS", &pixel_energy_cs);
    hits_tree->SetBranchAddress("ID_Merge", &id_pixel_merge);
    hits_tree->SetBranchAddress("Energy_Merge", &pixel_energy_merge);
    hits_tree->SetBranchAddress("Energy_Escape", &energy_escape);

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
    id_pixel_merge->clear();
    pixel_energy_merge->clear();
    energy_escape->clear();
}

/**
 * Function for returning an entry of the TTree "Event".
 */
data::Entry data::Event::get_entry() const
{
    return {event_id, *id_pixel, *pixel_energy, *id_pixel_cs, *pixel_energy_cs, *id_pixel_merge, *pixel_energy_merge, *energy_escape};
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
    entry.id_pixel_merge.clear();
    entry.pixel_energy_merge.clear();
    entry.energy_escape.clear();
}
