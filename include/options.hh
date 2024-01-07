#pragma once

#include <string>
#include <iostream>
#include <fstream>

class Options
{
private:
    std::string filename;
    int n_thresholds;
    int min_threshold;
    int max_threshold;

public:
    Options();
    ~Options();
};