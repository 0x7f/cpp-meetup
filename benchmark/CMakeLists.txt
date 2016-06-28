include_directories(${CMAKE_CURRENT_BINARY_DIR})

# openrtb proto
set(OPENRTB_PROTO ${PROJECT_SOURCE_DIR}/share/openrtb.proto)
set(PROTOG_BIN ${PROJECT_SOURCE_DIR}/thirdparty/protog/build/protog)
set(PARSER_SRCS ${CMAKE_CURRENT_BINARY_DIR}/bidrequest_parser.pb.cc)
set(PARSER_HDRS ${CMAKE_CURRENT_BINARY_DIR}/bidrequest_parser.pb.h)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${OPENRTB_PROTO})
add_custom_command(
    OUTPUT ${PARSER_SRCS} ${PARSER_HDRS}
    COMMAND ${PROTOG_BIN} -p ${OPENRTB_PROTO} -i openrtb.pb.h -m com.google.openrtb.BidRequest)

#empty
add_executable(empty-evhtpd empty-evhtp.cpp)
target_link_libraries(empty-evhtpd ${THIRDPARTY_LIBRARIES})
add_executable(empty-httppd empty-httpp.cpp)
target_link_libraries(empty-httppd ${THIRDPARTY_LIBRARIES})

# gason
add_executable(gason-evhtpd gason-evhtp.cpp ${CMAKE_SOURCE_DIR}/thirdparty/gason/src/gason.cpp)
target_link_libraries(gason-evhtpd ${THIRDPARTY_LIBRARIES})
target_include_directories(gason-evhtpd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/gason/src/)
add_executable(gason-httppd gason-httpp.cpp ${CMAKE_SOURCE_DIR}/thirdparty/gason/src/gason.cpp)
target_link_libraries(gason-httppd ${THIRDPARTY_LIBRARIES})
target_include_directories(gason-httppd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/gason/src/)

# jsoncpp
add_executable(jsoncpp-evhtpd jsoncpp-evhtp.cpp)
target_link_libraries(jsoncpp-evhtpd ${THIRDPARTY_LIBRARIES})
target_link_libraries(jsoncpp-evhtpd ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/build/src/lib_json/libjsoncpp.a)
target_include_directories(jsoncpp-evhtpd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/include/)
add_executable(jsoncpp-httppd jsoncpp-httpp.cpp)
target_link_libraries(jsoncpp-httppd ${THIRDPARTY_LIBRARIES})
target_link_libraries(jsoncpp-httppd ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/build/src/lib_json/libjsoncpp.a)
target_include_directories(jsoncpp-httppd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/include/)

# pbjson
add_executable(pbjson-evhtpd pbjson-evhtp.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PARSER_SRCS}
    ${CMAKE_SOURCE_DIR}/thirdparty/pbjson/src/pbjson.cpp)
target_link_libraries(pbjson-evhtpd ${THIRDPARTY_LIBRARIES} yajl)
target_include_directories(pbjson-evhtpd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/pbjson/src/)
add_executable(pbjson-httppd pbjson-httpp.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PARSER_SRCS}
    ${CMAKE_SOURCE_DIR}/thirdparty/pbjson/src/pbjson.cpp)
target_link_libraries(pbjson-httppd ${THIRDPARTY_LIBRARIES} yajl)
target_include_directories(pbjson-httppd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/pbjson/src/)

# protog
add_executable(protog-evhtpd protog-evhtp.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PARSER_SRCS})
target_link_libraries(protog-evhtpd ${THIRDPARTY_LIBRARIES} yajl)
add_executable(protog-httppd protog-httpp.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PARSER_SRCS})
target_link_libraries(protog-httppd ${THIRDPARTY_LIBRARIES} yajl)

# rapidjson
add_executable(rapidjson-evhtpd rapidjson-evhtp.cpp)
target_link_libraries(rapidjson-evhtpd ${THIRDPARTY_LIBRARIES})
target_include_directories(rapidjson-evhtpd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/rapidjson/include/)
add_executable(rapidjson-httppd rapidjson-httpp.cpp)
target_link_libraries(rapidjson-httppd ${THIRDPARTY_LIBRARIES})
target_include_directories(rapidjson-httppd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/rapidjson/include/)

# arduinojson
add_executable(arduinojson-evhtpd arduinojson-evhtp.cpp)
target_link_libraries(arduinojson-evhtpd ${THIRDPARTY_LIBRARIES})
target_include_directories(arduinojson-evhtpd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/ArduinoJson/include/)
add_executable(arduinojson-httppd arduinojson-httpp.cpp)
target_link_libraries(arduinojson-httppd ${THIRDPARTY_LIBRARIES})
target_include_directories(arduinojson-httppd PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/ArduinoJson/include/)
