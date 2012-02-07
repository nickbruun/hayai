#include <string>

#include "hayai-test.hpp"
#include "hayai-testfactory.hpp"

#ifndef __HAYAI_TESTDESCRIPTOR
#define __HAYAI_TESTDESCRIPTOR
namespace Hayai
{
    /// Test descriptor.
    
    /// Describes the test
    class TestDescriptor
    {
    public:
        /// Initialize a new test descriptor.
        
        /// @param fixtureName Name of the fixture.
        /// @param testName Name of the test.
        /// @param runs Number of runs for the test.
        /// @param iterations Number of iterations per run.
        /// @param testFactory Test factory implementation for the test.
        TestDescriptor(const char* fixtureName,
                       const char* testName,
                       std::size_t runs,
                       std::size_t iterations,
                       TestFactory* testFactory)
            :   FixtureName(fixtureName),
                TestName(testName),
                Runs(runs),
                Iterations(iterations),
                Factory(testFactory)
        {
            
        }
        
        
        /// Dispose of a test descriptor.
        ~TestDescriptor()
        {
            delete this->Factory;
        }
        
        
        /// Fixture name.
        std::string FixtureName;
        
        
        /// Test name.
        std::string TestName;
        
        
        /// Test runs.
        std::size_t Runs;
        
        
        /// Iterations per test run.
        std::size_t Iterations;
        
        
        /// Test factory.
        TestFactory* Factory;
    };
}
#endif
