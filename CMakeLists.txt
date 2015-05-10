##
# Project setup.
project(hayai)
cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0012 NEW)

# Offer the user the choice of overriding the installation directories.
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/hayai)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on.)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Proxy external CXXFLAGS.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} $ENV{CXXFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} $ENV{CXXFLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} $ENV{CXXFLAGS}")

# Determine system-dependant library paths.
include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" NEED_RT_LIB)
if (${NEED_RT_LIB})
  set(LIB_TIMING "rt")
else (${NEED_RT_LIB})
  set(LIB_TIMING "")
endif (${NEED_RT_LIB})

##
# Include the individual projects.
add_subdirectory(src)
add_subdirectory(sample)

# Include tests if requested by the user passing -DTEST=true.
if (${TEST})
  enable_testing()

  add_subdirectory(vendor/gtest)
  add_subdirectory(tests)
endif (${TEST})

##
# Export targets and package

# Add all targets to the build-tree export set
export(
  TARGETS hayai_main
  FILE "${PROJECT_BINARY_DIR}/hayai-targets.cmake"
)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry.)
export(PACKAGE hayai)

# Create the hayai-config.cmake.
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")
configure_file(hayai-config.cmake.in
  "${PROJECT_BINARY_DIR}/hayai-config.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${HAYAI_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(
  hayai-config.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/hayai-config.cmake" @ONLY
)

# Install the hayai-config.cmake
install(
  FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/hayai-config.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}"
  COMPONENT dev
)

# Install the export set for use with the install-tree.
install(
  EXPORT hayai-targets
  DESTINATION "${INSTALL_CMAKE_DIR}"
  COMPONENT dev
)
