#include <vector>
#include <iostream>
#include <limits>
#include <iomanip>

#include "hayai-console.hpp"
#include "hayai-testfactory.hpp"
#include "hayai-testdescriptor.hpp"

#ifndef __HAYAI_BENCHMARKER
#define __HAYAI_BENCHMARKER
namespace Hayai
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
                                            TestFactory* testFactory)
        {
            TestDescriptor* descriptor = new TestDescriptor(fixtureName,
                                                            testName,
                                                            runs,
                                                            iterations,
                                                            testFactory);
                
            Instance()._tests.push_back(descriptor);

            return descriptor;
        }
            
            
        /// Run all benchmarking tests.
        static void RunAllTests()
        {
            Benchmarker& instance = Instance();
            
            // Initial output
            std::cout << std::fixed;
            std::cout << Console::TextGreen << "[==========]"
                      << Console::TextDefault << " Running "
                      << instance._tests.size()
                      << (instance._tests.size() == 1 ?
                          " benchmark." : 
                          " benchmarks.")
                      << std::endl;

            // Run through all the tests in ascending order.
#define PAD(x) std::cout << std::setw(34) << x << std::endl;
#define PAD_DEVIATION(description,                                      \
                      deviated,                                         \
                      average,                                          \
                      unit)                                             \
            {                                                           \
                double _d_ =                                            \
                    double(deviated) - double(average);                 \
                                                                        \
                PAD(description <<                                      \
                    deviated << " " << unit << " (" <<                  \
                    (deviated < average ?                               \
                     Console::TextRed :                                 \
                     Console::TextGreen) <<                             \
                    (deviated > average ? "+" : "") <<                  \
                    _d_ << " " << unit << " / " <<                      \
                    (deviated > average ? "+" : "") <<                  \
                    (_d_ * 100.0 / average) << " %" <<                  \
                    Console::TextDefault << ")");                       \
            }
            
#define PAD_DEVIATION_INVERSE(description,                              \
                              deviated,                                 \
                              average,                                  \
                              unit)                                     \
            {                                                           \
                double _d_ =                                            \
                    double(deviated) - double(average);                 \
                                                                        \
                PAD(description <<                                      \
                    deviated << " " << unit << " (" <<                  \
                    (deviated > average ?                               \
                     Console::TextRed :                                 \
                     Console::TextGreen) <<                             \
                    (deviated > average ? "+" : "") <<                  \
                    _d_ << " " << unit << " / " <<                      \
                    (deviated > average ? "+" : "") <<                  \
                    (_d_ * 100.0 / average) << " %" <<                  \
                    Console::TextDefault << ")");                       \
            }


            std::size_t index = 0;
            while (index < instance._tests.size())
            {
                // Get the test descriptor.
                TestDescriptor* descriptor = instance._tests[index++];
               
                // Describe the beginning of the run.
                std::cout << Console::TextGreen << "[ RUN      ]"
                          << Console::TextDefault << " "
                          << descriptor->FixtureName << "."
                          << descriptor->TestName
                          << " (" << descriptor->Runs
                          << (descriptor->Runs == 1 ? " run, " : " runs, ") 
                          << descriptor->Iterations
                          << (descriptor->Iterations == 1 ?
                              " iteration per run)" :
                              " iterations per run)")
                          << std::endl;
                
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
                
                // Calculate different metrics.
                double timeRunAverage = double(timeTotal) / double(descriptor->Runs);

                double runsPerSecondAverage = 1000000.0 / timeRunAverage;
                double runsPerSecondMax = 1000000.0 / double(timeRunMin);
                double runsPerSecondMin = 1000000.0 / double(timeRunMax);
                
                double timeIterationAverage = timeRunAverage / double(descriptor->Iterations);
                double timeIterationMin = double(timeRunMin) / double(descriptor->Iterations);
                double timeIterationMax = double(timeRunMax) / double(descriptor->Iterations);

                double iterationsPerSecondAverage = 1000000.0 / timeIterationAverage;
                double iterationsPerSecondMax = 1000000.0 / timeIterationMin;
                double iterationsPerSecondMin = 1000000.0 / timeIterationMax;

                // Describe the end of the run.
                std::cout << Console::TextGreen << "[     DONE ]"
                          << Console::TextDefault << " "
                          << descriptor->FixtureName << "."
                          << descriptor->TestName << " ("
                          << (double(timeTotal) / 1000.0) << " ms)"
                          << std::endl;
                
                std::cout << Console::TextYellow << "[   RUNS   ] "
                          << Console::TextDefault
                          << "       Average time: " << timeRunAverage
                          << " us" << std::endl;
                
                PAD_DEVIATION_INVERSE("Fastest: ",
                              timeRunMin,
                              timeRunAverage,
                              "us");
                PAD_DEVIATION_INVERSE("Slowest: ",
                              timeRunMax,
                              timeRunAverage,
                              "us");
                PAD("");
                PAD("Average performance: " << runsPerSecondAverage << " runs/s");
                PAD_DEVIATION("Best performance: ",
                              runsPerSecondMax,
                              runsPerSecondAverage,
                              "runs/s");
                PAD_DEVIATION("Worst performance: ",
                              runsPerSecondMin,
                              runsPerSecondAverage,
                              "runs/s");

                std::cout << Console::TextYellow << "[ITERATIONS] "
                          << Console::TextDefault
                          << "       Average time: " << timeIterationAverage
                          << " us" << std::endl;
                
                PAD_DEVIATION_INVERSE("Fastest: ",
                              timeIterationMin,
                              timeIterationAverage,
                              "us");
                PAD_DEVIATION_INVERSE("Slowest: ",
                              timeIterationMax,
                              timeIterationAverage,
                              "us");
                PAD("");
                PAD("Average performance: " << iterationsPerSecondAverage << " iterations/s");
                PAD_DEVIATION("Best performance: ",
                              iterationsPerSecondMax,
                              iterationsPerSecondAverage,
                              "iterations/s");
                PAD_DEVIATION("Worst performance: ",
                              iterationsPerSecondMin,
                              iterationsPerSecondAverage,
                              "iterations/s");
            }

#undef PAD

            // Final output.
            std::cout << Console::TextGreen << "[==========]"
                      << Console::TextDefault << " Ran "
                      << instance._tests.size()
                      << (instance._tests.size() == 1 ?
                          " benchmark." : 
                          " benchmarks.")
                      << std::endl;
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
            std::size_t index = this->_tests.size();
            while (index--)
                delete this->_tests[index];
        }
            
            
        std::vector<TestDescriptor*> _tests; ///< Registered tests.
    };
}
#endif
