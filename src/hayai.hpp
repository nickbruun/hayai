#include "hayai_benchmarker.hpp"
#include "hayai_test.hpp"
#include "hayai_default_test_factory.hpp"
#include "hayai_fixture.hpp"
#include "hayai_console_outputter.hpp"

#ifndef __HAYAI

#ifndef __HAYAI_
#define __HAYAI_

#define BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) \
    fixture_name ## _ ## benchmark_name ## _Benchmark

#define BENCHMARK_(fixture_name,                                        \
                   benchmark_name,                                      \
                   fixture_class_name,                                  \
                   runs,                                                \
                   iterations)                                          \
    class BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)           \
        :   public fixture_class_name                                   \
    {                                                                   \
    public:                                                             \
        BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)()           \
        {                                                               \
                                                                        \
        };                                                              \
    protected:                                                          \
        virtual void TestBody();                                        \
    private:                                                            \
        static const ::hayai::TestDescriptor* _descriptor;              \
    };                                                                  \
                                                                        \
    const ::hayai::TestDescriptor* BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_descriptor = \
        ::hayai::Benchmarker::Instance().RegisterTest(#fixture_name,    \
                                                      #benchmark_name,  \
                                                      runs,             \
                                                      iterations,       \
                                                      new ::hayai::TestFactoryDefault<BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)>()); \
                                                                        \
    void BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::TestBody()

#define BENCHMARK_F(fixture_name,                        \
                    benchmark_name,                      \
                    runs,                                \
                    iterations)                          \
    BENCHMARK_(fixture_name,                             \
               benchmark_name,                           \
               fixture_name,                             \
               runs,                                     \
               iterations)

#define BENCHMARK(fixture_name,                          \
                  benchmark_name,                        \
                  runs,                                  \
                  iterations)                            \
    BENCHMARK_(fixture_name,                             \
               benchmark_name,                           \
               ::hayai::Test,                            \
               runs,                                     \
               iterations)

// Parametrized benchmarks support
#define BENCHMARK_P_(fixture_name, benchmark_name, fixture_class_name, runs, iterations, arguments) \
    class BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name): public fixture_class_name { \
    public:                                                             \
        BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) () {}       \
        virtual ~ BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) () {} \
        static const std::size_t _runs = runs;                          \
        static const std::size_t _iterations = iterations;              \
    protected:                                                          \
        inline void TestPayload arguments;                              \
    };                                                                  \
    void BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::TestPayload arguments

#define BENCHMARK_P(fixture_name, benchmark_name, runs, iterations, arguments) \
        BENCHMARK_P_(fixture_name, benchmark_name, hayai::Fixture, runs, iterations, arguments)

#define BENCHMARK_P_F(fixture_name, benchmark_name, runs, iterations, arguments) \
        BENCHMARK_P_(fixture_name, benchmark_name, fixture_name, runs, iterations, arguments)

#define BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id)   \
        fixture_name ## _ ## benchmark_name ## _Benchmark_ ## id

#define BENCHMARK_P_INSTANCE1(fixture_name, benchmark_name, arguments, id) \
    class BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id):    \
        public BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) {    \
    protected:                                                          \
        virtual void TestBody() { this->TestPayload arguments; }        \
    private:                                                            \
        static const ::hayai::TestDescriptor* _descriptor;              \
    };                                                                  \
    const ::hayai::TestDescriptor* BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id)::_descriptor = \
        ::hayai::Benchmarker::Instance().RegisterTest(                  \
            #fixture_name, #benchmark_name,                             \
            BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_runs, \
            BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_iterations, \
            new ::hayai::TestFactoryDefault< BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id) >(), \
            #arguments);

#define BENCHMARK_P_INSTANCE(fixture_name, benchmark_name, arguments)   \
    BENCHMARK_P_INSTANCE1(fixture_name, benchmark_name, arguments, __COUNTER__)


#endif
