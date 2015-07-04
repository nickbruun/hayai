#ifndef __HAYAI_BENCHMARKER
#define __HAYAI_BENCHMARKER
#include <algorithm>
#include <vector>
#include <limits>
#include <iomanip>
#include <string>
#include <cstring>

#include "hayai_test_factory.hpp"
#include "hayai_test_descriptor.hpp"
#include "hayai_test_result.hpp"
#include "hayai_console_outputter.hpp"


namespace hayai
{
    /// Benchmarking execution controller singleton.
    class Benchmarker
    {
    public:
        /// Get the singleton instance of @ref Benchmarker.

        /// @returns a reference to the singleton instance of the
        /// benchmarker execution controller.
        static Benchmarker& Instance()
        {
            static Benchmarker singleton;
            return singleton;
        }


        /// Register a test with the benchmarker instance.

        /// @param fixtureName Name of the fixture.
        /// @param testName Name of the test.
        /// @param runs Number of runs for the test.
        /// @param iterations Number of iterations per run.
        /// @param testFactory Test factory implementation for the test.
        /// @returns a pointer to a @ref TestDescriptor instance
        /// representing the given test.
        static TestDescriptor* RegisterTest(
            const char* fixtureName,
            const char* testName,
            std::size_t runs,
            std::size_t iterations,
            TestFactory* testFactory,
            TestParametersDescriptor parameters
        )
        {
            // Determine if the test has been disabled.
            static const char* disabledPrefix = "DISABLED_";
            bool isDisabled = ((::strlen(testName) >= 9) &&
                               (!::memcmp(testName, disabledPrefix, 9)));

            if (isDisabled)
                testName += 9;

            // Add the descriptor.
            TestDescriptor* descriptor = new TestDescriptor(fixtureName,
                                                            testName,
                                                            runs,
                                                            iterations,
                                                            testFactory,
                                                            parameters,
                                                            isDisabled);

            Instance()._tests.push_back(descriptor);

            return descriptor;
        }


        /// Add an outputter.

        /// @param outputter Outputter. The caller must ensure that the
        /// outputter remains in existence for the entire benchmark run.
        static void AddOutputter(Outputter& outputter)
        {
            Instance()._outputters.push_back(&outputter);
        }


        /// Apply a pattern filter to the tests.

        /// --gtest_filter-compatible pattern:
        ///
        /// https://code.google.com/p/googletest/wiki/AdvancedGuide
        ///
        /// @param pattern Filter pattern compatible with gtest.
        static void ApplyPatternFilter(const char* pattern)
        {
            Benchmarker& instance = Instance();

            // Split the filter at '-' if it exists.
            const char* const dash = strchr(pattern, '-');

            std::string positive;
            std::string negative;

            if (dash == NULL)
                positive = pattern;
            else
            {
                positive = std::string(pattern, dash);
                negative = std::string(dash + 1);
                if (positive.empty())
                    positive = "*";
            }

            // Iterate across all tests and test them against the patterns.
            std::size_t index = 0;
            while (index < instance._tests.size())
            {
                TestDescriptor* desc = instance._tests[index];

                if ((!FilterMatchesString(positive.c_str(),
                                          desc->CanonicalName)) ||
                    (FilterMatchesString(negative.c_str(),
                                         desc->CanonicalName)))
                {
                    instance._tests.erase(
                        instance._tests.begin() +
                        std::vector<TestDescriptor*>::difference_type(index)
                    );
                    delete desc;
                }
                else
                    ++index;
            }
        }


        /// Run all benchmarking tests.
        static void RunAllTests()
        {
            ConsoleOutputter defaultOutputter;
            std::vector<Outputter*> defaultOutputters;
            defaultOutputters.push_back(&defaultOutputter);

            Benchmarker& instance = Instance();
            std::vector<Outputter*>& outputters =
                (instance._outputters.empty() ?
                 defaultOutputters :
                 instance._outputters);

            // Get the tests for execution.
            std::vector<TestDescriptor*> tests = instance.GetTests();

            const std::size_t totalCount = tests.size();
            std::size_t disabledCount = 0;

            std::vector<TestDescriptor*>::const_iterator testsIt =
                tests.begin();

            while (testsIt != tests.end())
            {
                if ((*testsIt)->IsDisabled)
                    ++disabledCount;
                ++testsIt;
            }

            std::size_t enabledCount = totalCount - disabledCount;

            // Begin output.
            for (std::size_t outputterIndex = 0;
                 outputterIndex < outputters.size();
                 outputterIndex++)
                outputters[outputterIndex]->Begin(enabledCount, disabledCount);

            // Run through all the tests in ascending order.
            std::size_t index = 0;

            while (index < tests.size())
            {
                // Get the test descriptor.
                TestDescriptor* descriptor = tests[index++];

                // Check if test matches include filters
                if (instance._include.size() > 0)
                {
                    bool included = false;
                    std::string name =
                        descriptor->FixtureName + "." +
                        descriptor->TestName;

                    for (std::size_t i = 0; i < instance._include.size(); i++)
                    {
                        if (name.find(instance._include[i]) !=
                            std::string::npos)
                        {
                            included = true;
                            break;
                        }
                    }

                    if (!included)
                        continue;
                }

                // Check if test is not disabled.
                if (descriptor->IsDisabled)
                {
                    for (std::size_t outputterIndex = 0;
                         outputterIndex < outputters.size();
                         outputterIndex++)
                        outputters[outputterIndex]->SkipDisabledTest(
                            descriptor->FixtureName,
                            descriptor->TestName,
                            descriptor->Parameters,
                            descriptor->Runs,
                            descriptor->Iterations
                        );

                    continue;
                }

                // Describe the beginning of the run.
                for (std::size_t outputterIndex = 0;
                     outputterIndex < outputters.size();
                     outputterIndex++)
                    outputters[outputterIndex]->BeginTest(
                        descriptor->FixtureName,
                        descriptor->TestName,
                        descriptor->Parameters,
                        descriptor->Runs,
                        descriptor->Iterations
                    );

                // Execute each individual run.
                std::vector<uint64_t> runTimes(descriptor->Runs);

                std::size_t run = 0;
                while (run < descriptor->Runs)
                {
                    // Construct a test instance.
                    Test* test = descriptor->Factory->CreateTest();

                    // Run the test.
                    uint64_t time = test->Run(descriptor->Iterations);

                    // Store the test time.
                    runTimes[run] = time;

                    // Dispose of the test instance.
                    delete test;

                    ++run;
                }

                // Calculate the test result.
                TestResult testResult(runTimes, descriptor->Iterations);

                // Describe the end of the run.
                for (std::size_t outputterIndex = 0;
                     outputterIndex < outputters.size();
                     outputterIndex++)
                    outputters[outputterIndex]->EndTest(
                        descriptor->FixtureName,
                        descriptor->TestName,
                        descriptor->Parameters,
                        testResult
                    );

            }

            // End output.
            for (std::size_t outputterIndex = 0;
                 outputterIndex < outputters.size();
                 outputterIndex++)
                outputters[outputterIndex]->End(enabledCount,
                                                disabledCount);
        }


        /// List tests.
        static std::vector<const TestDescriptor*> ListTests()
        {
            std::vector<const TestDescriptor*> tests;
            Benchmarker& instance = Instance();

            std::size_t index = 0;
            while (index < instance._tests.size())
                tests.push_back(instance._tests[index++]);

            return tests;
        }


        /// Shuffle tests.

        /// Randomly shuffles the order of tests.
        static void ShuffleTests()
        {
            Benchmarker& instance = Instance();
            std::random_shuffle(instance._tests.begin(),
                                instance._tests.end());
        }
    private:
        /// Private constructor.
        Benchmarker()
        {

        }


        /// Private destructor.
        ~Benchmarker()
        {
            // Release all test descriptors.
            std::size_t index = _tests.size();
            while (index--)
                delete _tests[index];
        }


        /// Get the tests to be executed.
        std::vector<TestDescriptor*> GetTests() const
        {
            std::vector<TestDescriptor*> tests;

            std::size_t index = 0;
            while (index < _tests.size())
                tests.push_back(_tests[index++]);

            return tests;
        }


        /// Test if a filter matches a string.

        /// Adapted from gtest. All rights reserved by original authors.
        static bool FilterMatchesString(const char* filter,
                                        const std::string& str)
        {
            const char *patternStart = filter;

            while (true)
            {
                if (PatternMatchesString(patternStart, str.c_str()))
                    return true;

                // Finds the next pattern in the filter.
                patternStart = strchr(patternStart, ':');

                // Returns if no more pattern can be found.
                if (!patternStart)
                    return false;

                // Skips the pattern separater (the ':' character).
                patternStart++;
            }
        }


        /// Test if pattern matches a string.

        /// Adapted from gtest. All rights reserved by original authors.
        static bool PatternMatchesString(const char* pattern, const char *str)
        {
            switch (*pattern)
            {
            case '\0':
            case ':':
                return (*str == '\0');
            case '?':  // Matches any single character.
                return ((*str != '\0') &&
                        (PatternMatchesString(pattern + 1, str + 1)));
            case '*':  // Matches any string (possibly empty) of characters.
                return (((*str != '\0') &&
                         (PatternMatchesString(pattern, str + 1))) ||
                        (PatternMatchesString(pattern + 1, str)));
            default:
                return ((*pattern == *str) &&
                        (PatternMatchesString(pattern + 1, str + 1)));
            }
        }


        std::vector<Outputter*> _outputters; ///< Registered outputters.
        std::vector<TestDescriptor*> _tests; ///< Registered tests.
        std::vector<std::string> _include; ///< Test filters.
    };
}
#endif
