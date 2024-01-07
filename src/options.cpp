#include "options.hh"

#include <filesystem>
#include <fstream>
#include <array>
#include <iostream>
#include <iomanip>

#include "constants.hh"

const std::filesystem::path options_path{"../utils/options.txt"};
constexpr std::array<const char *, 5> options_keys{"ROOT_FILE", "N_THR", "MIN_THR", "MAX_THR", "VERBOSITY"};

/**
 * The default constructor.
 *
 * It loads either the option specified in the options.txt file,
 * or the default ones.
 */
options::Options::Options()
{
    std::fstream options_file;
    options_file.open(options_path, std::ios::in);

    if (!options_file.is_open())
    {
        printf("%sOptions::Options() ERROR: Impossible to open %s%s\n", ERROR_COLOR, options_path.c_str(), END_COLOR);
        set_default();
        printf("%sOptions::load_default() - INFO - Loaded default options.%s\n", ERROR_COLOR, END_COLOR);
        return;
    }

    std::string key, value;
    while (!options_file.eof())
    {
        options_file >> key >> value;

        if (key == options_keys[0])
            filename = value;
        else if (key == options_keys[1])
            n_thresholds = std::stoi(value);
        else if (key == options_keys[2])
            min_threshold = std::stod(value);
        else if (key == options_keys[3])
            max_threshold = std::stod(value);
        else if (key == options_keys[4])
            verbosity = static_cast<bool>(std::stoi(value));
        else
            continue;
    }

    options_file.close();

    printf("%sOptions::Options() - INFO - Loaded info from %s.%s\n", INFO_COLOR, options_path.c_str(), END_COLOR);
}

/**
 * The destructor.
 *
 * It writes the options to the options.txt file,
 * in order to save them for the next analysis.
 */
options::Options::~Options()
{
    std::fstream options_file;
    options_file.open(options_path, std::ios::out);

    if (!options_file.is_open())
    {
        printf("%sOptions::Options() - ERROR - Impossible to open %s.%s\n", ERROR_COLOR, options_path.c_str(), END_COLOR);
        return;
    }

    int w = 20;
    options_file << std::setw(w / 2) << options_keys[0] << std::setw(w) << filename << "\n";
    options_file << std::setw(w / 2) << options_keys[1] << std::setw(w) << n_thresholds << "\n";
    options_file << std::setw(w / 2) << options_keys[2] << std::setw(w) << min_threshold << "\n";
    options_file << std::setw(w / 2) << options_keys[3] << std::setw(w) << max_threshold << "\n";
    options_file << std::setw(w / 2) << options_keys[4] << std::setw(w) << verbosity << "\n";

    options_file.close();
}

/**
 * Function for setting the default options.
 */
void options::Options::set_default()
{
    filename = FILENAME_DEF;
    n_thresholds = N_THRESHOLDS_DEF;
    min_threshold = MIN_THRESHOLD_DEF;
    max_threshold = MAX_THRESHOLD_DEF;
    verbosity = VERBOSITY_DEF;
}

/**
 * Function for printing the current options.
 */
void options::Options::print_options() const
{
    printf("\n");
    printf("1) %s = %s\n", options_keys[0], filename.c_str());
    printf("2) %s = %i\n", options_keys[1], n_thresholds);
    printf("3) %s = %.2f\n", options_keys[2], min_threshold);
    printf("4) %s = %.2f\n", options_keys[3], max_threshold);
    printf("5) %s = %i\n", options_keys[4], verbosity);
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
            break;
        case 'd':
            set_default();
        case 'x':
            done = true;
            clear_screen();
            break;
        default:
            break;
        }
    } while (!done);
}
