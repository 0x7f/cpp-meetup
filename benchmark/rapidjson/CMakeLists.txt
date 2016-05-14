include_directories(${CMAKE_SOURCE_DIR}/thirdparty/rapidjson/include/)

add_executable(rapidjsond main.cpp)
target_link_libraries(rapidjsond ${THIRDPARTY_LIBRARIES})
