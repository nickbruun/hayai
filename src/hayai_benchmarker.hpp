#ifndef __HAYAI_BENCHMARKER
#define __HAYAI_BENCHMARKER
#include <vector>
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
#include <fstream>
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
        static TestDescriptor* RegisterTest(const char* fixtureName,
                                            const char* testName,
                                            std::size_t runs,
                                            std::size_t iterations,
                                            TestFactory* testFactory,
                                            std::string parameters = "")
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


        /// Add an inclusion filter pattern.

        /// @param pattern Inclusion pattern.
        static void AddIncludeFilter(const std::string& pattern)
        {
            Instance()._include.push_back(pattern);
        }


        /// Add an outputter.

        /// @param outputter Outputter. The caller must ensure that the
        /// outputter remains in existence for the entire benchmark run.
        static void AddOutputter(Outputter& outputter)
        {
            Instance()._outputters.push_back(&outputter);
        }

        /// Run all benchmarking tests.
        static void RunAllTests()
        {
            Benchmarker& instance = Instance();
            std::vector<Outputter*>& outputters = instance._outputters;

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
                    std::string name = descriptor->FixtureName + "." +
                        descriptor->TestName + descriptor->Parameters;

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
                int64_t timeTotal = 0,
                        timeRunMin = std::numeric_limits<int64_t>::max(),
                        timeRunMax = std::numeric_limits<int64_t>::min();

                std::size_t run = descriptor->Runs;
                while (run--)
                {
                    // Construct a test instance.
                    Test* test = descriptor->Factory->CreateTest();

                    // Run the test.
                    int64_t time = test->Run(descriptor->Iterations);

                    // Store the test time.
                    timeTotal += time;
                    if (timeRunMin > time)
                        timeRunMin = time;
                    if (timeRunMax < time)
                        timeRunMax = time;

                    // Dispose of the test instance.
                    delete test;
                }

                // Calculate the test result.
                TestResult testResult(descriptor->Runs,
                                      descriptor->Iterations,
                                      timeTotal,
                                      timeRunMin,
                                      timeRunMax);

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
            {
                // Get the test descriptor.
                TestDescriptor* descriptor = _tests[index++];

                // Check if test matches include filters
                if (!_include.empty())
                {
                    bool included = false;
                    std::string name = descriptor->FixtureName + "." +
                        descriptor->TestName + descriptor->Parameters;

                    for (std::size_t i = 0; i < _include.size(); i++)
                    {
                        if (name.find(_include[i]) != std::string::npos)
                        {
                            included = true;
                            break;
                        }
                    }

                    if (!included)
                        continue;
                }

                tests.push_back(descriptor);
            }

            return tests;
        }

        std::vector<Outputter*> _outputters; ///< Registered outputters.
        std::vector<TestDescriptor*> _tests; ///< Registered tests.
        std::vector<std::string> _include; ///< Test filters.
    };
}
#endif
