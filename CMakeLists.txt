cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME stack-n-formula)
project(${PROJECT_NAME})

# TODO(Korniakov): not sure if these lines are needed
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

set(MP2_LIBRARY "${PROJECT_NAME}")
set(MP2_LIBRARY_STACK "stack")
set(MP2_LIBRARY_FORMULA "formula")
set(MP2_TESTS   "test_${PROJECT_NAME}")
set(MP2_INCLUDE_STACK "${CMAKE_CURRENT_SOURCE_DIR}/stacklib")
set(MP2_INCLUDE_FORMULA "${CMAKE_CURRENT_SOURCE_DIR}/formulalib")

include_directories("${MP2_INCLUDE_STACK}" gtest)
include_directories("${MP2_INCLUDE_FORMULA}" gtest)

# BUILD
add_subdirectory(stacklib)
add_subdirectory(formulalib)
add_subdirectory(samples)
add_subdirectory(gtest)
add_subdirectory(test)

# REPORT
message( STATUS "")
message( STATUS "General configuration for ${PROJECT_NAME}")
message( STATUS "======================================")
message( STATUS "")
message( STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
message( STATUS "")
