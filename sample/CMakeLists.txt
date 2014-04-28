# Quench warnings about CMP0003 with CMake 2.4.
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_executable(sample
  delivery_man_benchmark.cpp
  delivery_man_benchmark_with_fixture.cpp
  delivery_man_benchmark_parameterized.cpp
  delivery_man_benchmark_parameterized_with_fixture.cpp
  delivery_man_sleep.cpp
)

target_link_libraries(sample
  hayai_main
  ${LIB_TIMING}
)
