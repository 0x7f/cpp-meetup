cmake_minimum_required(VERSION 2.8)
project(protog_benchmark C CXX)

include(cmake/Prerequisites.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(demo)

add_subdirectory(benchmark/empty/)
add_subdirectory(benchmark/gason/)
add_subdirectory(benchmark/jsoncpp/)
add_subdirectory(benchmark/pbjson/)
add_subdirectory(benchmark/protog/)
add_subdirectory(benchmark/rapidjson/)
