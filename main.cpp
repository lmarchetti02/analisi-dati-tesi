#include "TApplication.h"
#include "analysis.hh"

#include <filesystem>
#include <iostream>

int main(int argc, char **argv)
{
    // create necessary directories
    std::filesystem::create_directory("../output/");
    std::filesystem::create_directory("../results/");
    std::filesystem::create_directory("../utils/");

    printf("\033c");
    TApplication app("app", &argc, argv);

    analysis::Analysis analysis = analysis::Analysis();

    try {
        analysis.get_trees();
        analysis.run();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    app.Run();

    return 0;
}
