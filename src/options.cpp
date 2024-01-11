#include "options.hh"

#include <filesystem>
#include <fstream>
#include <array>
#include <iostream>
#include <iomanip>

#include "constants.hh"

const std::filesystem::path options_path{"../utils/options.txt"};
constexpr std::array<const char *, 5> options_keys{"ROOT_FILE", "N_THR", "MIN_THR", "MAX_THR", "VERBOSITY"};
constexpr const char FILENAME_DEF[] = "output0.root";
constexpr int N_THRESHOLDS_DEF = 0;
constexpr double MIN_THRESHOLD_DEF = 0.0;
constexpr double MAX_THRESHOLD_DEF = 80.0;
constexpr bool VERBOSITY_DEF = false;

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
options::Options::Options() : filename(FILENAME_DEF),
                              n_thresholds(N_THRESHOLDS_DEF),
                              min_threshold(MIN_THRESHOLD_DEF),
                              max_threshold(MAX_THRESHOLD_DEF),
                              threshold_step(0),
                              verbosity(VERBOSITY_DEF),
                              opt_verbose(VERBOSITY_DEF)
{
    std::fstream options_file;
    options_file.open(options_path, std::ios::in);

    if (!options_file.is_open())
    {
        printf("%sOptions::Options() - WARING - Impossible to open %s%s\n", WARNING_COLOR, options_path.c_str(), END_COLOR);
        printf("%sOptions::Options() - INFO - Loaded default options.%s\n", ERROR_COLOR, END_COLOR);
        return;
    }

    std::string key, value;
    while (!options_file.eof())
    {
        options_file >> key >> value;

        const auto &[ROOT_FILE, N_THR, MIN_THR, MAX_THR, VERBOSITY] = options_keys;

        if (key == ROOT_FILE)
            filename = value;
        else if (key == N_THR)
            n_thresholds = std::stoi(value);
        else if (key == MIN_THR)
            min_threshold = std::stod(value);
        else if (key == MAX_THR)
            max_threshold = std::stod(value);
        else if (key == VERBOSITY)
            verbosity = std::stoi(value) != 0;
        else
            continue;
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
    opt_verbose = verbosity;

    if (previous_verbose)
        printf("\n%sOptions::set_default() - INFO - Set default options.%s\n", INFO_COLOR, END_COLOR);

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

    if (!options_file.is_open())
    {
        printf("%sOptions::Options() - ERROR - Impossible to open %s.%s\n", ERROR_COLOR, options_path.c_str(), END_COLOR);
        return;
    }

    int w = 20;
    const auto &[ROOT_FILE, N_THR, MIN_THR, MAX_THR, VERBOSITY] = options_keys;

    options_file << std::setw(w / 2) << ROOT_FILE << std::setw(w) << filename << "\n";
    options_file << std::setw(w / 2) << N_THR << std::setw(w) << n_thresholds << "\n";
    options_file << std::setw(w / 2) << MIN_THR << std::setw(w) << min_threshold << "\n";
    options_file << std::setw(w / 2) << MAX_THR << std::setw(w) << max_threshold << "\n";
    options_file << std::setw(w / 2) << VERBOSITY << std::setw(w) << verbosity << "\n";

    options_file.close();

    if (opt_verbose)
        printf("%sOptions::save_to_file() - INFO - Saved options to file.%s\n", INFO_COLOR, END_COLOR);
}

/**
 * Function for printing the current options.
 */
void options::Options::print_options() const
{
    const auto &[ROOT_FILE, N_THR, MIN_THR, MAX_THR, VERBOSITY] = options_keys;

    printf("\n");
    printf("1) %s = %s\n", ROOT_FILE, filename.c_str());
    printf("2) %s = %i\n", N_THR, n_thresholds);
    printf("3) %s = %.2f\n", MIN_THR, min_threshold);
    printf("4) %s = %.2f\n", MAX_THR, max_threshold);
    printf("5) %s = %i\n", VERBOSITY, verbosity);
}

/**
 * Function for changing the current options.
 */
void options::Options::change_options()
{
    bool done = false;
    do
    {
        clear_screen();
        print_options();
        printf("\nType:\n");
        printf("- 'number' to change the corresponding option;\n");
        printf("- 'd' to set the default options;\n");
        printf("- 'x' to go back\n");

        char num;
        std::string choice{};
        std::getline(std::cin, choice);
        if (choice.length() > 1)
            continue;

        num = choice[0];
        if (num != 'd' && num != 'x')
            printf("New value: ");

        std::string input{};
        switch (num)
        {
        case '1':
            std::getline(std::cin, filename);
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
        if (num != '1' && num != '5')
            threshold_step = (n_thresholds) ? (max_threshold - min_threshold) / n_thresholds : 0;
    } while (!done);

    if (opt_verbose)
        printf("\n%sOptions::change_options() - INFO - Options changed.%s\n", INFO_COLOR, END_COLOR);

    save_to_file();
}
