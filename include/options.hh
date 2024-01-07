#pragma once

#include <string>

namespace options
{
    class Options
    {
    private:
        std::string filename;
        int n_thresholds;
        double min_threshold;
        double max_threshold;
        bool verbosity;

        static bool verbose;

    public:
        Options();
        ~Options();

        void set_default();
        void print_options() const;
        void change_options();

        // Set verbosity of the class.
        static void set_verbose(bool value) { verbose = value; }
    };
} // namespace options
