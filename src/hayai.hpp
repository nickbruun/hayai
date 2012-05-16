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

// Parametrized benchmarks support
#define BENCHMARK_P_(fixture_name, benchmark_name, fixture_class_name, runs, iterations, arguments)     \
class BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name): public fixture_class_name {                  \
public:                                                                                                 \
  BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) () {}                                             \
  virtual ~ BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) () {}                                   \
  static const std::size_t _runs = runs;                                                                \
  static const std::size_t _iterations = iterations;                                                    \
protected:                                                                                              \
  inline void TestPayload arguments;                                                                    \
};                                                                                                      \
void BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::TestPayload arguments

#define BENCHMARK_P(fixture_name, benchmark_name, runs, iterations, arguments)                          \
  BENCHMARK_P_(fixture_name, benchmark_name, Hayai::Fixture, runs, iterations, arguments)

#define BENCHMARK_P_F(fixture_name, benchmark_name, runs, iterations, arguments)                        \
  BENCHMARK_P_(fixture_name, benchmark_name, fixture_name, runs, iterations, arguments)

#define BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id)                                       \
  fixture_name ## _ ## benchmark_name ## _Benchmark_ ## id

#define BENCHMARK_P_INSTANCE1(fixture_name, benchmark_name, arguments, id)                              \
class BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id):                                        \
  public BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) {                                          \
protected:                                                                                              \
  virtual void TestBody() { this->TestPayload arguments; }                                            \
private:                                                                                                \
  static const ::Hayai::TestDescriptor* _descriptor;                                                    \
};                                                                                                      \
const ::Hayai::TestDescriptor* BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id)::_descriptor = \
    ::Hayai::Benchmarker::Instance().RegisterTest(                                                      \
        #fixture_name, #benchmark_name,                                                                 \
        BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_runs,                                     \
		BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_iterations,                               \
        new ::Hayai::TestFactoryDefault< BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id) >(), \
        #arguments);

#define BENCHMARK_P_INSTANCE(fixture_name, benchmark_name, arguments)                 \
  BENCHMARK_P_INSTANCE1(fixture_name, benchmark_name, arguments, __COUNTER__)
