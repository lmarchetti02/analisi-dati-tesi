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
    double threshold_step;
    bool verbosity;
    bool opt_verbose;
    bool use_probabilities;

    Options();

    void set_default();
    void save_to_file() const;

    std::string results_files();

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
    double get_min_threshold() const { return min_threshold; }
    double get_max_threshold() const { return max_threshold; }
    double get_threshold_step() const { return threshold_step; }
    bool get_verbosity() const { return verbosity; }
    bool get_use_probabilities() const { return use_probabilities; }
};
} // namespace options
