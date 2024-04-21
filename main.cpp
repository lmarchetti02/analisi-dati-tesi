#include <iostream>

#include "TApplication.h"

#include "analysis.hh"

int main(int argc, char **argv)
{
    printf("\033c");
    TApplication app("app", &argc, argv);

    analysis::Analysis analysis = analysis::Analysis();

    try
    {
        analysis.get_trees();
        analysis.run();
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    app.Run();

    return 0;
}