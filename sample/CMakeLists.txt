include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" NEED_RT_LIB)

if(${NEED_RT_LIB}) 
    set(EXT_LIB "rt")
else()
    set(EXT_LIB "")
endif()

add_executable(sample
  delivery_man_benchmark.cpp
  delivery_man_benchmark_with_fixture.cpp
  delivery_man_benchmark_parameterized.cpp
  delivery_man_benchmark_parameterized_with_fixture.cpp
  delivery_man_sleep.cpp
)

target_link_libraries(sample
  hayai_main
  ${EXT_LIB}
)
