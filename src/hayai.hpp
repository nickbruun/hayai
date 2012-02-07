#include "hayai-benchmarker.hpp"
#include "hayai-test.hpp"
#include "hayai-defaulttestfactory.hpp"
#include "hayai-fixture.hpp"

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
        static const ::Hayai::TestDescriptor* _descriptor;              \
    };                                                                  \
                                                                        \
    const ::Hayai::TestDescriptor* BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_descriptor = \
        ::Hayai::Benchmarker::Instance().RegisterTest(#fixture_name, \
                                                                     #benchmark_name, \
                                                                     runs, \
                                                                     iterations, \
                                                                     new ::Hayai::TestFactoryDefault<BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)>()); \
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
               ::Hayai::Test,                            \
               runs,                                     \
               iterations)
