include_directories(${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/include/)

add_executable(jsoncpp_httppd main.cpp)
target_link_libraries(jsoncpp_httppd ${THIRDPARTY_LIBRARIES})
target_link_libraries(jsoncpp_httppd ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/build/src/lib_json/libjsoncpp.a)
