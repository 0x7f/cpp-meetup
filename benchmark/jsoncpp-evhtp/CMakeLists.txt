include_directories(${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/include/)

add_executable(jsoncpp_evhtpd main.cpp)
target_link_libraries(jsoncpp_evhtpd ${THIRDPARTY_LIBRARIES})
target_link_libraries(jsoncpp_evhtpd ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/build/src/lib_json/libjsoncpp.a)
