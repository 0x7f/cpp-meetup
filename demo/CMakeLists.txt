set(OPENRTB_PROTO ${PROJECT_SOURCE_DIR}/share/openrtb.proto)
set(PROTOG_BIN ${PROJECT_SOURCE_DIR}/thirdparty/protog/build/protog)
set(PARSER_SRCS ${CMAKE_CURRENT_BINARY_DIR}/bidrequest_parser.pb.cc)
set(PARSER_HDRS ${CMAKE_CURRENT_BINARY_DIR}/bidrequest_parser.pb.h)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${OPENRTB_PROTO})
add_custom_command(
    OUTPUT ${PARSER_SRCS} ${PARSER_HDRS}
    COMMAND ${PROTOG_BIN} -p ${OPENRTB_PROTO} -i openrtb.pb.h -m com.google.openrtb.BidRequest)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/include/)

add_executable(demo1 demo1.cpp)
target_link_libraries(demo1 ${THIRDPARTY_LIBRARIES})
target_link_libraries(demo1 ${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp/build/src/lib_json/libjsoncpp.a)

add_executable(demo2 demo2.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PARSER_SRCS})
target_link_libraries(demo2 ${THIRDPARTY_LIBRARIES} yajl)

add_executable(demo3 demo3.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PARSER_SRCS})
target_link_libraries(demo3 ${THIRDPARTY_LIBRARIES} yajl)
