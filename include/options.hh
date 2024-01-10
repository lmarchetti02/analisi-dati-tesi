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

        bool opt_verbose;

        Options();

        void set_default();
        void save_to_file() const;

    public:
        static Options &get_instance();

        Options(const Options &) = delete;
        Options(const Options &&) = delete;
        Options &operator=(const Options &) = delete;
        Options &operator=(const Options &&) = delete;

        void print_options() const;
        void change_options();

        std::string get_filename() const { return filename; }
        int get_n_thresholds() const { return n_thresholds; }
        std::array<double, 2> get_thresholds_extremes() const { return {min_threshold, max_threshold}; }
        bool get_verbosity() const { return verbosity; }
    };
} // namespace options
