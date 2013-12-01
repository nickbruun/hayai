include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_executable(sample
  WomanBenchmark.cpp
  WomanBenchmarkWithFixture.cpp
  WomanBenchmarkParametrized.cpp
)

target_link_libraries(sample
  hayai_main
)
