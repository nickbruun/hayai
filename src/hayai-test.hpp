#include <cstddef>
#include <sys/time.h>
#include <stdint.h>

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

        }

        /// Method is called after test execution finished, provided execution stats.
        virtual void AfterRun(
        		double timeRunAverage,
        		double runsPerSecondAverage, double runsPerSecondMax, double runsPerSecondMin,
        		double timeIterationAverage, double timeIterationMax, double timeIterationMin,
        		double iterationsPerSecondAverage, double iterationsPerSecondMax, double iterationsPerSecondMin
        		)
        {

        }

        
        /// Run the test.
        
        /// @param iterations Number of iterations to gather data for.
        /// @returns the number of microseconds the run took.
        int64_t Run(std::size_t iterations)
        {
            // Set up the testing fixture.
            this->SetUp();
            
            // Get the starting time.
            struct timeval startTime,
                           endTime;
            
            gettimeofday(&startTime,
                         NULL);
            
            // Run the test body for each iteration.
            std::size_t iteration = iterations;
            while (iteration--)
                this->TestBody();
            
            // Get the ending time.
            gettimeofday(&endTime,
                         NULL);
            
            // Tear down the testing fixture.
            this->TearDown();
            
            // Return the duration in microseconds.
            return (endTime.tv_sec - startTime.tv_sec) * 1000000 + 
                   (endTime.tv_usec - startTime.tv_usec);
        }
    protected:
        /// Test body.
        
        /// Executed for each iteration the benchmarking test is run.
        virtual void TestBody()
        {
            
        }
    };
}
#endif
