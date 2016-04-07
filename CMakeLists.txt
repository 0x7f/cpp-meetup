cmake_minimum_required(VERSION 3.0)
project(protog_benchmark C CXX)

cmake_policy(SET CMP0063 NEW)

include(cmake/Prerequisites.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(tools/empty/)
add_subdirectory(tools/protog/)
add_subdirectory(tools/rapidjson/)
