include_directories(${CMAKE_SOURCE_DIR}/thirdparty/rapidjson/include/)

add_executable(rapidjson_httppd main.cpp)
target_link_libraries(rapidjson_httppd ${THIRDPARTY_LIBRARIES})
