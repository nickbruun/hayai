#include "hayai.hpp"

int main()
{
    Hayai::ConsoleOutputter consoleOutputter;

    Hayai::Benchmarker::AddOutputter(consoleOutputter);
    Hayai::Benchmarker::RunAllTests();
    return 0;
}
