cmake_minimum_required(VERSION 3.0)
project(protog_benchmark C CXX)

cmake_policy(SET CMP0063 NEW)

include(cmake/Prerequisites.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(demo)

add_subdirectory(benchmark/empty/)
add_subdirectory(benchmark/gason/)
add_subdirectory(benchmark/jsoncpp/)
add_subdirectory(benchmark/protog/)
add_subdirectory(benchmark/rapidjson/)
