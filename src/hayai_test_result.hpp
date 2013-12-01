#ifndef __HAYAI_TESTRESULT
#define __HAYAI_TESTRESULT
namespace hayai
{
    /// Test result descriptor.

    /// All times are expressed in microseconds.
    struct TestResult
    {
    public:
        /// Initialize test result descriptor.

        /// @param runs Number of runs.
        /// @param iterations Number of iterations.
        TestResult(std::size_t runs,
                   std::size_t iterations,
                   double timeTotal,
                   double timeRunMin,
                   double timeRunMax)
            :   _runs(runs),
                _iterations(iterations),
                _timeTotal(timeTotal),
                _timeRunMin(timeRunMin),
                _timeRunMax(timeRunMax)
        {

        }


        /// Total time.
        inline double TimeTotal() const
        {
            return _timeTotal;
        }


        /// Average time per run.
        inline double RunTimeAverage() const
        {
            return _timeTotal / double(_runs);
        }


        /// Maximum time per run.
        inline double RunTimeMaximum() const
        {
            return _timeRunMax;
        }


        /// Minimum time per run.
        inline double RunTimeMinimum() const
        {
            return _timeRunMin;
        }


        /// Average runs per second.
        inline double RunsPerSecondAverage() const
        {
            return 1000000.0 / RunTimeAverage();
        }


        /// Maximum runs per second.
        inline double RunsPerSecondMaximum() const
        {
            return 1000000.0 / _timeRunMin;
        }


        /// Minimum runs per second.
        inline double RunsPerSecondMinimum() const
        {
            return 1000000.0 / _timeRunMax;
        }


        /// Average time per iteration.
        inline double IterationTimeAverage() const
        {
            return RunTimeAverage() / double(_iterations);
        }


        /// Minimum time per iteration.
        inline double IterationTimeMinimum() const
        {
            return _timeRunMin / double(_iterations);
        }


        /// Maximum time per iteration.
        inline double IterationTimeMaximum() const
        {
            return _timeRunMax / double(_iterations);
        }


        /// Average iterations per second.
        inline double IterationsPerSecondAverage() const
        {
            return 1000000.0 / IterationTimeAverage();
        }


        /// Minimum iterations per second.
        inline double IterationsPerSecondMinimum() const
        {
            return 1000000.0 / IterationTimeMaximum();
        }


        /// Maximum iterations per second.
        inline double IterationsPerSecondMaximum() const
        {
            return 1000000.0 / IterationTimeMinimum();
        }
    private:
        std::size_t _runs;
        std::size_t _iterations;
        double _timeTotal;
        double _timeRunMin;
        double _timeRunMax;
    };
}
#endif
