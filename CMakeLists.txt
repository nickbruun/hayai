# Project setup
project(hayai)
cmake_minimum_required(VERSION 2.4)

option(USE_XML "Use XML output" OFF)

if (USE_XML)
    FIND_PACKAGE(LibXml2 REQUIRED)
    ADD_DEFINITIONS(-D__HAYAI_USE_XML)
    INCLUDE_DIRECTORIES(SYSTEM "${LIBXML2_INCLUDE_DIR}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBXML2_DEFINITIONS}")
    LINK_DIRECTORIES("${LIBXML2_LIBRARIES}")
endif()

# Sub projects
add_subdirectory(src)
add_subdirectory(sample)
