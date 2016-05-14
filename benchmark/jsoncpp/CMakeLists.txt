include_directories(${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/include/)

add_executable(jsoncppd main.cpp)
target_link_libraries(jsoncppd ${THIRDPARTY_LIBRARIES})
target_link_libraries(jsoncppd ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/build/src/lib_json/libjsoncpp.a)
