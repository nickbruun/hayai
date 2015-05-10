# Quench warnings about CMP0003 with CMake 2.4.
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/vendor/gtest/include
)

add_executable(tests
  hayai_test_parameter_descriptor.cpp
)

target_link_libraries(tests
  gtest_main
  ${LIB_TIMING}
)

add_test(HayaiTests tests)
