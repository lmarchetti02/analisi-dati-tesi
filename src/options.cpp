#include "options.hh"

#include "constants.hh"

#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

const std::filesystem::path options_path{"../utils/options.txt"};
const std::filesystem::path results_path{"../results"};
constexpr std::array<const char *, 6> options_keys{"ROOT_FILE", "N_THR",     "MIN_THR",
                                                   "MAX_THR",   "VERBOSITY", "USE_PROBABILITIES"};
constexpr const char FILENAME_DEF[] = "uniform_mono.root";
constexpr int N_THRESHOLDS_DEF = 50;
constexpr double MIN_THRESHOLD_DEF = 0.0;
constexpr double MAX_THRESHOLD_DEF = 0.11;
constexpr bool VERBOSITY_DEF = false;
constexpr bool USE_PROBABILITIES_DEF = false;

/**
 * Static function for accessing the singleton instance.
 *
 * @return The singleton instance
 */
options::Options &options::Options::get_instance()
{
    static Options instance;
    return instance;
}

/**
 * The default constructor.
 *
 * It loads either the option specified in the options.txt file,
 * or the default ones.
 */
options::Options::Options()
    : filename(FILENAME_DEF)
    , n_thresholds(N_THRESHOLDS_DEF)
    , min_threshold(MIN_THRESHOLD_DEF)
    , max_threshold(MAX_THRESHOLD_DEF)
    , threshold_step(0)
    , verbosity(VERBOSITY_DEF)
    , opt_verbose(VERBOSITY_DEF)
    , use_probabilities(USE_PROBABILITIES_DEF)
{
    std::fstream options_file;
    options_file.open(options_path, std::ios::in);

    if (!options_file.is_open()) {
        printf("%sOptions::Options() - WARING - Impossible to open %s%s\n", WARNING_COLOR, options_path.c_str(),
               END_COLOR);
        printf("%sOptions::Options() - INFO - Loaded default options.%s\n", ERROR_COLOR, END_COLOR);
        return;
    }

    std::string key, value;
    while (!options_file.eof()) {
        options_file >> key >> value;

        const auto &[ROOT_FILE, N_THR, MIN_THR, MAX_THR, VERBOSITY, USE_PROBABILITIES] = options_keys;

        if (key == ROOT_FILE) filename = value;
        else if (key == N_THR) n_thresholds = std::stoi(value);
        else if (key == MIN_THR) min_threshold = std::stod(value);
        else if (key == MAX_THR) max_threshold = std::stod(value);
        else if (key == VERBOSITY) verbosity = std::stoi(value) != 0;
        else if (key == USE_PROBABILITIES) use_probabilities = std::stoi(value) != 0;
        else continue;
    }

    threshold_step = (n_thresholds) ? (max_threshold - min_threshold) / n_thresholds : 0;
    opt_verbose = verbosity;

    options_file.close();

    if (opt_verbose)
        printf("%sOptions::Options() - INFO - Loaded info from %s.%s\n", INFO_COLOR, options_path.c_str(), END_COLOR);
}

/**
 * Function for setting the default options.
 */
void options::Options::set_default()
{
    bool previous_verbose = opt_verbose;

    filename = FILENAME_DEF;
    n_thresholds = N_THRESHOLDS_DEF;
    min_threshold = MIN_THRESHOLD_DEF;
    max_threshold = MAX_THRESHOLD_DEF;
    threshold_step = 0;
    verbosity = VERBOSITY_DEF;
    use_probabilities = USE_PROBABILITIES_DEF;
    opt_verbose = verbosity;

    if (previous_verbose) print_warning("\nOptions::set_default() - INFO - Set default options.\n");

    save_to_file();
}

/**
 * Function for saving options.
 *
 * It writes the options to the options.txt file,
 * in order to save them for the next analysis.
 */
void options::Options::save_to_file() const
{
    std::fstream options_file;
    options_file.open(options_path, std::ios::out);

    if (!options_file.is_open()) {
        printf("%sOptions::Options() - ERROR - Impossible to open %s.%s\n", ERROR_COLOR, options_path.c_str(),
               END_COLOR);
        return;
    }

    int w = 50;
    const auto &[ROOT_FILE, N_THR, MIN_THR, MAX_THR, VERBOSITY, USE_PROBABILITIES] = options_keys;

    options_file << std::setw(w / 2) << ROOT_FILE << std::setw(w) << filename << "\n";
    options_file << std::setw(w / 2) << N_THR << std::setw(w) << n_thresholds << "\n";
    options_file << std::setw(w / 2) << MIN_THR << std::setw(w) << min_threshold << "\n";
    options_file << std::setw(w / 2) << MAX_THR << std::setw(w) << max_threshold << "\n";
    options_file << std::setw(w / 2) << VERBOSITY << std::setw(w) << verbosity << "\n";
    options_file << std::setw(w / 2) << USE_PROBABILITIES << std::setw(w) << use_probabilities << "\n";

    options_file.close();

    if (opt_verbose) printf("%sOptions::save_to_file() - INFO - Saved options to file.%s\n", INFO_COLOR, END_COLOR);
}

/**
 * Function for getting the root file
 * where the results are stored.
 *
 * @return The string with the file name.
 */
std::string options::Options::results_files()
{
    std::vector<std::filesystem::path> filenames;

    printf("\n");

    int counter = 0;
    for (const auto &entry : std::filesystem::directory_iterator(results_path)) {
        printf("%i. %s\n", counter, entry.path().c_str());
        filenames.push_back(entry.path());
        counter++;
    }

    std::string input{};
    std::getline(std::cin, input);
    int index = std::stoi(input);

    return filenames[index].filename().string();
}

/**
 * Function for printing the current options.
 */
void options::Options::print_options() const
{
    const auto &[ROOT_FILE, N_THR, MIN_THR, MAX_THR, VERBOSITY, USE_PROBABILITIES] = options_keys;

    printf("\n");
    printf("1) %s = %s\n", ROOT_FILE, filename.c_str());
    printf("2) %s = %i\n", N_THR, n_thresholds);
    printf("3) %s = %.3f GeV\n", MIN_THR, min_threshold);
    printf("4) %s = %.3f GeV\n", MAX_THR, max_threshold);
    printf("5) %s = %i\n", VERBOSITY, verbosity);
    printf("6) %s = %i\n", USE_PROBABILITIES, use_probabilities);
}

/**
 * Function for changing the current options.
 */
void options::Options::change_options()
{
    bool done = false;
    do {
        clear_screen();
        print_options();
        printf("\nType:\n");
        printf("- 'number' to change the corresponding option;\n");
        printf("- 'd' to set the default options;\n");
        printf("- 'x' to go back\n");

        char num;
        std::string choice{};
        std::getline(std::cin, choice);
        if (choice.length() > 1 || !choice.length()) continue;

        num = choice[0];
        if (num != 'd' && num != 'x') printf("New value: ");

        std::string input{};
        switch (num) {
        case '1':
            filename = results_files();
            break;
        case '2':
            std::getline(std::cin, input);
            n_thresholds = std::stoi(input);
            break;
        case '3':
            std::getline(std::cin, input);
            min_threshold = std::stod(input);
            break;
        case '4':
            std::getline(std::cin, input);
            max_threshold = std::stod(input);
            break;
        case '5':
            std::getline(std::cin, input);
            verbosity = static_cast<bool>(std::stoi(input));
            opt_verbose = verbosity;
            break;
        case '6':
            std::getline(std::cin, input);
            use_probabilities = static_cast<bool>(std::stoi(input));
            break;
        case 'd':
            done = true;
            set_default();
            break;
        case 'x':
            done = true;
            clear_screen();
            break;
        default:
            break;
        }
        if (num != '1' && num != '5' && num != 6)
            threshold_step = (n_thresholds) ? (max_threshold - min_threshold) / n_thresholds : 0;
    } while (!done);

    if (opt_verbose) print_info("\nOptions::change_options() - INFO - Options changed.\n");

    save_to_file();
}
