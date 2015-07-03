#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>

#include "hayai.hpp"
#include "hayai_console.hpp"


#if defined(_WIN32)
#    define PATH_SEPARATOR '\\'
#else
#    define PATH_SEPARATOR '/'
#endif


#define Flag(_desc)                                                \
    ::hayai::Console::TextGreen << _desc << ::hayai::Console::TextDefault
#define Argument(_desc)                                                 \
    ::hayai::Console::TextYellow << _desc << ::hayai::Console::TextDefault
#define UsageError(_desc)                                           \
    {                                                               \
        std::cerr << ::hayai::Console::TextRed << "Error"           \
                  << ::hayai::Console::TextDefault << ": " << _desc \
                  << std::endl << std::endl;                        \
        ShowUsage(argv[0]);                                         \
        return EXIT_FAILURE;                                        \
    }


/// Execution mode.
enum ExecutionMode
{
    /// Run benchmarks.
    RunBenchmarks,


    /// List benchmarks but do not execute them.
    ListBenchmarks
};


/// File outputter.
class FileOutputter
{
public:
    /// File outputter.

    /// @param path Output path. Expected to be available during the life time
    /// of the outputter.
    FileOutputter(const char* path)
        :   _path(path),
            _outputter(NULL)
    {

    }


    virtual ~FileOutputter()
    {
        if (_outputter)
            delete _outputter;

        _stream.close();
    }


    /// Set up.

    /// Opens the output file for writing and initializes the outputter.
    virtual void SetUp()
    {
        _stream.open(_path,
                     std::ios_base::out |
                     std::ios_base::trunc |
                     std::ios_base::binary);
        if (_stream.bad())
        {
            std::stringstream error;
            error << "failed to open " << _path << " for writing: "
                  << strerror(errno);
            throw std::runtime_error(error.str());
        }

        _outputter = CreateOutputter(_stream);
    }


    /// Outputter.
    virtual ::hayai::Outputter& Outputter()
    {
        if (!_outputter)
            throw std::runtime_error("outputter has not been set up");

        return *_outputter;
    }
protected:
    /// Create outputter from output stream.

    /// @param stream Output stream for the outputter.
    /// @returns the outputter for the given format.
    virtual ::hayai::Outputter* CreateOutputter(std::ostream& stream) = 0;
private:
    const char* _path;
    std::ofstream _stream;
    ::hayai::Outputter* _outputter;
};


class JsonFileOutputter
    :   public FileOutputter
{
public:
    /// JSON file outputter.

    /// @param path Output path. Expected to be available during the life time
    /// of the outputter.
    JsonFileOutputter(const char* path)
        :   FileOutputter(path)
    {

    }
protected:
    virtual ::hayai::Outputter* CreateOutputter(std::ostream& stream)
    {
        return new ::hayai::JsonOutputter(stream);
    }
};


/// Show usage.

/// @param execName Executable name.
static void ShowUsage(const char* execName)
{
    const char* baseName = strrchr(execName, PATH_SEPARATOR);

    std::cerr << "Usage: " << (baseName ? baseName + 1 : execName)
              << " " << Flag("[OPTIONS]") << std::endl
              << std::endl

              << "  Runs the benchmarks for this project." << std::endl
              << std::endl

              << "Benchmark selection options:" << std::endl
              << "  " << Flag("-l") << ", " << Flag("--list") << std::endl
              << "    List the names of all benchmarks instead of "
              << "running them." << std::endl
              << "  " << Flag("-f") << ", " << Flag("--filter") << " <"
              << Argument("pattern") << ">" << std::endl
              << "    Run only tests that match a given pattern." << std::endl
              << std::endl

              << "Benchmark execution options:" << std::endl
              << "  " << Flag("-s") << ", " << Flag("--shuffle") << std::endl
              << "    Randomize benchmark execution order." << std::endl
              << std::endl

              << "Benchmark output options:" << std::endl
              << "  " << Flag("-o") << ", " << Flag("--output") << " <"
              << Argument("format") << ">[:" << Argument("<path>") << "]"
              << std::endl
              << "    Output results in a specific format. If no path is "
              << "specified, the output" << std::endl
              << "    will be presented on stdout. Can be specified multiple "
              << "times to get output" << std::endl
              << "    in different formats. The supported formats are:"
              << std::endl
              << std::endl
              << "    " << Argument("console") << std::endl
              << "      Standard console output." << std::endl
              << "    " << Argument("json") << std::endl
              << "      JSON." << std::endl
              << std::endl
              << "    If multiple output formats are provided without a "
              << "path, only the last" << std::endl
              << "    provided format will be output to stdout." << std::endl
              << "  " << Flag("--c") << ", " << Flag("--color") << " ("
              << ::hayai::Console::TextGreen << "yes"
              << ::hayai::Console::TextDefault << "|"
              << ::hayai::Console::TextGreen << "no"
              << ::hayai::Console::TextDefault << ")" << std::endl
              << "    Enable colored output when available. Default "
              << ::hayai::Console::TextGreen << "yes"
              << ::hayai::Console::TextDefault << "." << std::endl
              << std::endl

              << "Miscellaneous options:" << std::endl
              << "  " << Flag("-?") << ", " << Flag("-h") << ", "
              << Flag("--help") << std::endl
              << "    Show this help information." << std::endl
              << std::endl

              << "hayai version: " << HAYAI_VERSION << std::endl
              << "Clock: " << ::hayai::Clock::Description() << std::endl;
}


int main(int argc, char** argv)
{
    // Set up defaults.
    ExecutionMode execMode = RunBenchmarks;
    bool shuffle = false;
    std::vector<FileOutputter*> fileOutputters;
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
            execMode = ListBenchmarks;
        // Shuffle flag.
        else if ((!strcmp(arg, "-s")) || (!strcmp(arg, "--shuffle")))
            shuffle = true;
        // Output flag.
        else if ((!strcmp(arg, "-o")) || (!strcmp(arg, "--output")))
        {
            if (argLast)
                UsageError(Flag(arg) << " requires a format to be specified");
            char* formatSpecifier = argv[argI++];

            char* format = formatSpecifier;
            char* path = strchr(formatSpecifier, ':');
            if (path)
            {
                *(path++) = 0;
                if (!strlen(path))
                    path = NULL;
            }

            if (!strcmp(format, "console"))
            {
                if (path)
                {

                }
                else
                {
                    if (stdoutOutputter)
                        delete stdoutOutputter;
                    stdoutOutputter = new ::hayai::ConsoleOutputter();
                }
            }
            else if (!strcmp(format, "json"))
            {
                if (path)
                    fileOutputters.push_back(new JsonFileOutputter(path));
                else
                {
                    if (stdoutOutputter)
                        delete stdoutOutputter;
                    stdoutOutputter = new ::hayai::JsonOutputter(std::cout);
                }
            }
            else
                UsageError("invalid format: " << format);
        }
        // Console coloring flag.
        else if ((!strcmp(arg, "-c")) || (!strcmp(arg, "--color")))
        {
            if (argLast)
                UsageError(Flag(arg) << " requires an argument of either " <<
                         Flag("yes") << " or " << Flag("no"));

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
                UsageError("invalid argument to " << Flag(arg) << ": " <<
                           choice);
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
            UsageError("unknown option: " << arg);
    }

    // Execute based on the selected mode.
    switch (execMode)
    {
    case RunBenchmarks:
    {
        // Ensure that at least one output exists.
        if ((!stdoutOutputter) && (fileOutputters.empty()))
            stdoutOutputter = new ::hayai::ConsoleOutputter();

        // Hook up the outputs.
        if (stdoutOutputter)
            ::hayai::Benchmarker::AddOutputter(*stdoutOutputter);

        for (std::vector<FileOutputter*>::iterator it = fileOutputters.begin();
             it < fileOutputters.end();
             ++it)
        {
            FileOutputter& fileOutputter = **it;

            try
            {
                fileOutputter.SetUp();
            }
            catch (std::exception& e)
            {
                std::cerr << ::hayai::Console::TextRed << "Error"
                          << ::hayai::Console::TextDefault << ": " << e.what()
                          << std::endl;
                return EXIT_FAILURE;
            }

            ::hayai::Benchmarker::AddOutputter(fileOutputter.Outputter());
        }

        // Run the benchmarks.
        ::hayai::Benchmarker::RunAllTests();

        break;
    }

    case ListBenchmarks:
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
    for (std::vector<FileOutputter*>::iterator it = fileOutputters.begin();
         it < fileOutputters.end();
         ++it)
        delete *it;

    delete stdoutOutputter;

    return EXIT_SUCCESS;
}
