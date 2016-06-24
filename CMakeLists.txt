cmake_minimum_required(VERSION 2.8)
project(protog_benchmark C CXX)

include(cmake/Prerequisites.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(demo)
add_subdirectory(benchmark)
