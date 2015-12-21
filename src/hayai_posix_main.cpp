#include "hayai_main.hpp"


int main(int argc, char** argv)
{
    std::srand(static_cast<unsigned>(std::time(0)));

    // Set up the main runtime.
    ::hayai::Runner runner;

    // Parse the arguments.
    int result = runner.ParseArgs(argc, argv);
    if (result)
    {
        return result;
    }

    // Execute based on the selected mode.
    return runner.Run();
}
