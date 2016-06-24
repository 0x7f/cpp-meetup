include_directories(${CMAKE_SOURCE_DIR}/thirdparty/rapidjson/include/)

add_executable(rapidjson_evhtpd main.cpp)
target_link_libraries(rapidjson_evhtpd ${THIRDPARTY_LIBRARIES})
