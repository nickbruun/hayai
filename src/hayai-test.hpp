#include <cstddef>
#include <stdint.h>

#include "hayai-timer.hpp"

#ifndef __HAYAI_TEST
#define __HAYAI_TEST
namespace Hayai
{
    /// Base test class.
    
    /// @ref SetUp is invoked before each run, and @ref TearDown is invoked 
    /// once the run is finished. Iterations rely on the same fixture 
    /// for every run.
    /// 
    /// The default test class does not contain any actual code in the 
    /// SetUp and TearDown methods, which means that tests can inherit 
    /// this class directly for non-fixture based benchmarking tests.
    class Test
    {
    public:
        /// Set up the testing fixture for execution of a run.
        virtual void SetUp()
        {
            
        }
        
        
        /// Tear down the previously set up testing fixture after the 
        /// execution run.
        virtual void TearDown()
        {
            
        }
        
        /// Method is called before test execution, may be useful for statistics collection.
        virtual void BeforeTest(std::string FixtureName, std::string TestName, std::size_t runs, std::size_t iterations)
        {
            (void)FixtureName;
            (void)TestName;
            (void)runs;
            (void)iterations;
        }

        /// Method is called after test execution finished, provided execution stats.
        virtual void AfterRun(
        		double timeRunAverage,
        		double runsPerSecondAverage, double runsPerSecondMax, double runsPerSecondMin,
        		double timeIterationAverage, double timeIterationMax, double timeIterationMin,
        		double iterationsPerSecondAverage, double iterationsPerSecondMax, double iterationsPerSecondMin
        		)
        {
            (void)timeRunAverage;
            (void)runsPerSecondAverage;
            (void)runsPerSecondMax;
            (void)runsPerSecondMin;
            (void)timeIterationAverage;
            (void)timeIterationMax;
            (void)timeIterationMin;
            (void)iterationsPerSecondAverage;
            (void)iterationsPerSecondMax;
            (void)iterationsPerSecondMin;
        }

        
        /// Run the test.
        
        /// @param iterations Number of iterations to gather data for.
        /// @returns the number of nanoseconds the run took.
        int64_t Run(std::size_t iterations)
        {
            // Set up the testing fixture.
            this->SetUp();

            // Get the starting time.
            HighResolutionTimer::CpuTime startTime = 
                HighResolutionTimer::getCpuTime();
            // Run the test body for each iteration.
            std::size_t iteration = iterations;
            while (iteration--)
                this->TestBody();
            
            HighResolutionTimer::CpuTime endTime = 
                HighResolutionTimer::getCpuTime();
            
            // Tear down the testing fixture.
            this->TearDown();
            
            // Return the duration in nanoseconds.
            return HighResolutionTimer::getDuration(startTime, endTime);
        }
        virtual ~Test() {}
    protected:
        /// Test body.
        
        /// Executed for each iteration the benchmarking test is run.
        virtual void TestBody()
        {
            
        }
    };
}
#endif
