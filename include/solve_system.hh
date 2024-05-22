#pragma once

#include "TMatrixD.h"
#include "options.hh"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const std::filesystem::path prob_path{"../output/transition_probabilities.csv"};

namespace solve_system
{
/**
 * Function for reading the trnasition
 * probabilities stored in the .csv file.
 *
 * @return The matrix with the probabilities.
 */
inline TMatrixD read_file()
{
    int N = options::Options::get_instance().get_n_thresholds();
    TMatrixD probabilities(N, N);

    std::fstream probabilities_file;
    probabilities_file.open(prob_path, std::ios::in);
    if (!probabilities_file.is_open()) throw std::runtime_error("Impossible to open probabilities file.");

    int row = -1;
    int col = 0;
    std::string line;
    while (std::getline(probabilities_file, line)) {
        if (row < 0) {
            row++;
            continue;
        }

        std::string buffer;
        std::stringstream input_line(line);

        std::getline(input_line, buffer, ',');
        std::getline(input_line, buffer, ',');
        std::getline(input_line, buffer, ',');

        probabilities(row, col) = std::stod(buffer.c_str());

        if (col < N - 1) {
            col++;
        } else {
            col = 0;
            row++;
        }
    }

    return probabilities;
}

/*
 * Function for getting the matrix that
 * defines the system to be solved.
 *
 * @return Said matrix.
 */
inline TMatrixD get_system_matrix()
{
    int N = options::Options::get_instance().get_n_thresholds();
    TMatrixD system_matrix(N, N);
    const TMatrixD &K = read_file();

    if (options::Options::get_instance().get_verbosity()) K.Print();

    bool invertible = true;

    for (int row = 0; row < N; row++) {
        for (int col = row; col < N; col++) {

            if (col == row) {
                double first = 1 + 0.5 * K(row, 0);
                for (int j = 1; j < row; j++)
                    first -= 0.5 * K(row - col, col);

                system_matrix(row, col) = first;

                if (!first) invertible = false;
                continue;
            }

            system_matrix(row, col) += K(col, row);
        }
    }

    if (options::Options::get_instance().get_verbosity()) {
        system_matrix.Print();

        (invertible) ? printf("The matrix is invertible\n") : printf("The matrix is not invertible\n");
    }

    return system_matrix;
}

/**
 * Function for solving the linear system necessary
 * for the spectrum reconstruction from the transition
 * probabilities.
 *
 * @param[in] counts The measured counts.
 * @return The corrected counts.
 */
inline std::vector<int> solve(std::vector<int> counts)
{
    int N = options::Options::get_instance().get_n_thresholds();
    std::vector<int> reconstructed_counts(N, 0);
    const TMatrixD &system_matrix = get_system_matrix();

    if (options::Options::get_instance().get_verbosity()) {
        printf("Measured counts\n");
        for (int e : counts) {
            printf("%i\n", e);
        }
    }

    for (int row = N - 1; row > -1; row--) {
        double factor = 0;
        for (int j = row + 1; j < N; j++)
            factor -= system_matrix(row, j) * reconstructed_counts[j];

        reconstructed_counts[row] = (counts[row] - factor) / system_matrix(row, row);
    }

    if (options::Options::get_instance().get_verbosity()) {
        printf("Reconstructed counts\n");
        for (int e : reconstructed_counts) {
            printf("%i\n", e);
        }
    }

    return reconstructed_counts;
}
} // namespace solve_system
