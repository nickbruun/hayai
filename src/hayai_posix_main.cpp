#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <set>
#include <vector>

#include "hayai_main.hpp"


#if defined(_WIN32)
#    define PATH_SEPARATOR '\\'
#else
#    define PATH_SEPARATOR '/'
#endif


#define FORMAT_FLAG(_desc)                                              \
    ::hayai::Console::TextGreen << _desc << ::hayai::Console::TextDefault
#define FORMAT_ARGUMENT(_desc)                                          \
    ::hayai::Console::TextYellow << _desc << ::hayai::Console::TextDefault
#define FORMAT_ERROR(_desc)                                             \
    ::hayai::Console::TextRed << "Error:" <<                            \
    ::hayai::Console::TextDefault << ": " << _desc
#define USAGE_ERROR(_desc)                                          \
    {                                                               \
        std::cerr << FORMAT_ERROR(_desc) << std::endl << std::endl; \
        ShowUsage(argv[0]);                                         \
        return EXIT_FAILURE;                                        \
    }


/// Show usage.

/// @param execName Executable name.
static void ShowUsage(const char* execName)
{
    const char* baseName = strrchr(execName, PATH_SEPARATOR);

    std::cerr << "Usage: " << (baseName ? baseName + 1 : execName)
              << " " << FORMAT_FLAG("[OPTIONS]") << std::endl
              << std::endl

              << "  Runs the benchmarks for this project." << std::endl
              << std::endl

              << "Benchmark selection options:" << std::endl
              << "  " << FORMAT_FLAG("-l") << ", " << FORMAT_FLAG("--list")
              << std::endl
              << "    List the names of all benchmarks instead of "
              << "running them." << std::endl
              << "  " << FORMAT_FLAG("-f") << ", " << FORMAT_FLAG("--filter")
              << " <" << FORMAT_ARGUMENT("pattern") << ">" << std::endl
              << "    Run only tests that match a given pattern." << std::endl
              << std::endl

              << "Benchmark execution options:" << std::endl
              << "  " << FORMAT_FLAG("-s") << ", " << FORMAT_FLAG("--shuffle")
              << std::endl
              << "    Randomize benchmark execution order." << std::endl
              << std::endl

              << "Benchmark output options:" << std::endl
              << "  " << FORMAT_FLAG("-o") << ", " << FORMAT_FLAG("--output")
              << " <" << FORMAT_ARGUMENT("format") << ">[:"
              << FORMAT_ARGUMENT("<path>") << "]"
              << std::endl
              << "    Output results in a specific format. If no path is "
              << "specified, the output" << std::endl
              << "    will be presented on stdout. Can be specified multiple "
              << "times to get output" << std::endl
              << "    in different formats. The supported formats are:"
              << std::endl
              << std::endl
              << "    " << FORMAT_ARGUMENT("console") << std::endl
              << "      Standard console output." << std::endl
              << "    " << FORMAT_ARGUMENT("json") << std::endl
              << "      JSON." << std::endl
              << std::endl
              << "    If multiple output formats are provided without a "
              << "path, only the last" << std::endl
              << "    provided format will be output to stdout." << std::endl
              << "  " << FORMAT_FLAG("--c") << ", "
              << FORMAT_FLAG("--color") << " ("
              << ::hayai::Console::TextGreen << "yes"
              << ::hayai::Console::TextDefault << "|"
              << ::hayai::Console::TextGreen << "no"
              << ::hayai::Console::TextDefault << ")" << std::endl
              << "    Enable colored output when available. Default "
              << ::hayai::Console::TextGreen << "yes"
              << ::hayai::Console::TextDefault << "." << std::endl
              << std::endl

              << "Miscellaneous options:" << std::endl
              << "  " << FORMAT_FLAG("-?") << ", " << FORMAT_FLAG("-h") << ", "
              << FORMAT_FLAG("--help") << std::endl
              << "    Show this help information." << std::endl
              << std::endl

              << "hayai version: " << HAYAI_VERSION << std::endl
              << "Clock implementation: " << ::hayai::Clock::Description()
              << std::endl;
}


int main(int argc, char** argv)
{
    // Set up defaults.
    ::hayai::MainExecutionMode execMode = ::hayai::MainRunBenchmarks;
    bool shuffle = false;
    std::vector< ::hayai::FileOutputter*> fileOutputters;
    ::hayai::Outputter* stdoutOutputter = NULL;

    // Parse the arguments.
    int argI = 1;
    while (argI < argc)
    {
        char* arg = argv[argI++];
        bool argLast = (argI == argc);
        std::size_t argLen = strlen(arg);

        if (argLen == 0)
            continue;

        // List flag.
        if ((!strcmp(arg, "-l")) || (!strcmp(arg, "--list")))
            execMode = ::hayai::MainListBenchmarks;
        // Shuffle flag.
        else if ((!strcmp(arg, "-s")) || (!strcmp(arg, "--shuffle")))
            shuffle = true;
        // Output flag.
        else if ((!strcmp(arg, "-o")) || (!strcmp(arg, "--output")))
        {
            if (argLast)
                USAGE_ERROR(FORMAT_FLAG(arg) <<
                            " requires a format to be specified");
            char* formatSpecifier = argv[argI++];

            char* format = formatSpecifier;
            char* path = strchr(formatSpecifier, ':');
            if (path)
            {
                *(path++) = 0;
                if (!strlen(path))
                    path = NULL;
            }

#define ADD_OUTPUTTER(_prefix)                                          \
            {                                                           \
                if (path)                                               \
                    fileOutputters.push_back(                           \
                        new ::hayai::_prefix ## FileOutputter(path)     \
                    );                                                  \
                else                                                    \
                {                                                       \
                    if (stdoutOutputter)                                \
                        delete stdoutOutputter;                         \
                    stdoutOutputter =                                   \
                        new ::hayai::_prefix ## Outputter(std::cout);   \
                }                                                       \
            }

            if (!strcmp(format, "console"))
                ADD_OUTPUTTER(Console)
            else if (!strcmp(format, "json"))
                ADD_OUTPUTTER(Json)
            else
                USAGE_ERROR("invalid format: " << format);

#undef ADD_OUTPUTTER
        }
        // Console coloring flag.
        else if ((!strcmp(arg, "-c")) || (!strcmp(arg, "--color")))
        {
            if (argLast)
                USAGE_ERROR(FORMAT_FLAG(arg) << " requires an argument " <<
                            "of either " << FORMAT_FLAG("yes") << " or " <<
                            FORMAT_FLAG("no"));

            char* choice = argv[argI++];

            if ((!strcmp(choice, "yes")) ||
                (!strcmp(choice, "true")) ||
                (!strcmp(choice, "on")) ||
                (!strcmp(choice, "1")))
            {}
            else if ((!strcmp(choice, "no")) ||
                     (!strcmp(choice, "false")) ||
                     (!strcmp(choice, "off")) ||
                     (!strcmp(choice, "0")))
            {}
            else
                USAGE_ERROR("invalid argument to " << FORMAT_FLAG(arg) <<
                           ": " << choice);
        }
        // Help
        else if ((!strcmp(arg, "-?")) ||
                 (!strcmp(arg, "-h")) ||
                 (!strcmp(arg, "--help")))
        {
            ShowUsage(argv[0]);
            return EXIT_FAILURE;
        }
        else
            USAGE_ERROR("unknown option: " << arg);
    }

    // Execute based on the selected mode.
    switch (execMode)
    {
    case ::hayai::MainRunBenchmarks:
    {
        // Ensure that at least one output exists.
        if ((!stdoutOutputter) && (fileOutputters.empty()))
            stdoutOutputter = new ::hayai::ConsoleOutputter();

        // Hook up the outputs.
        if (stdoutOutputter)
            ::hayai::Benchmarker::AddOutputter(*stdoutOutputter);

        for (std::vector< ::hayai::FileOutputter*>::iterator it =
                 fileOutputters.begin();
             it < fileOutputters.end();
             ++it)
        {
            ::hayai::FileOutputter& fileOutputter = **it;

            try
            {
                fileOutputter.SetUp();
            }
            catch (std::exception& e)
            {
                std::cerr << FORMAT_ERROR(e.what()) << std::endl;
                return EXIT_FAILURE;
            }

            ::hayai::Benchmarker::AddOutputter(fileOutputter.Outputter());
        }

        // Run the benchmarks.
        ::hayai::Benchmarker::RunAllTests();

        break;
    }

    case ::hayai::MainListBenchmarks:
    {
        // List out the unique benchmark names.
        std::vector<const ::hayai::TestDescriptor*> tests =
            ::hayai::Benchmarker::ListTests();
        std::vector<std::string> testNames;
        std::set<std::string> uniqueTestNames;

        for (std::vector<const ::hayai::TestDescriptor*>::iterator it =
                 tests.begin();
             it < tests.end();
             ++it)
        {
            std::stringstream testNameStream;
            testNameStream << (*it)->FixtureName << "." << (*it)->TestName;
            std::string testName = testNameStream.str();

            if (uniqueTestNames.find(testName) != uniqueTestNames.end())
                continue;

            testNames.push_back(testName);
            uniqueTestNames.insert(testName);
        }

        // Sort the benchmark names.
        std::sort(testNames.begin(), testNames.end());

        // Dump the list.
        for (std::vector<std::string>::iterator it = testNames.begin();
             it < testNames.end();
             ++it)
            std::cout << *it << std::endl;

        break;
    }
    }

    // Clean up the outputs.
    for (std::vector< ::hayai::FileOutputter*>::iterator it =
             fileOutputters.begin();
         it < fileOutputters.end();
         ++it)
        delete *it;

    if (stdoutOutputter)
        delete stdoutOutputter;

    return EXIT_SUCCESS;
}
