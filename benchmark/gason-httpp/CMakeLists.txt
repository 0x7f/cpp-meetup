include_directories(${CMAKE_SOURCE_DIR}/thirdparty/gason/src/)

add_executable(gason_httppd main.cpp ${CMAKE_SOURCE_DIR}/thirdparty/gason/src/gason.cpp)
target_link_libraries(gason_httppd ${THIRDPARTY_LIBRARIES})
