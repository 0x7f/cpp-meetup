include_directories(${CMAKE_SOURCE_DIR}/thirdparty/gason/src/)

add_executable(gasond main.cpp ${CMAKE_SOURCE_DIR}/thirdparty/gason/src/gason.cpp)
target_link_libraries(gasond ${THIRDPARTY_LIBRARIES})
