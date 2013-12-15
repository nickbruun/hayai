include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

add_executable(hayai_sample
  delivery_man_benchmark.cpp
  delivery_man_benchmark_with_fixture.cpp
  delivery_man_benchmark_parameterized.cpp
  delivery_man_benchmark_parameterized_with_fixture.cpp
)

target_link_libraries(hayai_sample
  hayai_main
)

install(TARGETS hayai_sample
  # IMPORTANT: Add the executable to the "export-set"
  EXPORT hayai-targets 
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
  COMPONENT bin)
