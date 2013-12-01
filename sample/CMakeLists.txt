include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_executable(sample
  delivery_man_benchmark.cpp
  delivery_man_benchmark_with_fixture.cpp
  delivery_man_benchmark_parameterized.cpp
  delivery_man_benchmark_parameterized_with_fixture.cpp
)

target_link_libraries(sample
  hayai_main
)
