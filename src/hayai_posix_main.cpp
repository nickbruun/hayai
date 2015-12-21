#include "hayai_main.hpp"


int main(int argc, char** argv)
{
    // Set up the main runner.
    ::hayai::MainRunner runner;

    // Parse the arguments.
    int result = runner.ParseArgs(argc, argv);
    if (result)
    {
        return result;
    }

    // Execute based on the selected mode.
    return runner.Run();
}
