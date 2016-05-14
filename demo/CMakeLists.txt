include_directories(${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/include/)

add_executable(demo1 demo1.cpp)
target_link_libraries(demo1 ${THIRDPARTY_LIBRARIES})
target_link_libraries(demo1 ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/build/src/lib_json/libjsoncpp.a)
