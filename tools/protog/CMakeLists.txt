set(OPENRTB_PROTO ${PROJECT_SOURCE_DIR}/share/openrtb.proto)
set(PROTOG_BIN ${PROJECT_SOURCE_DIR}/thirdparty/protog/build/protog)
set(PARSER_SRCS ${CMAKE_CURRENT_BINARY_DIR}/bidrequest_parser.pb.cpp)
set(PARSER_HDRS ${CMAKE_CURRENT_BINARY_DIR}/bidrequest_parser.pb.h)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${OPENRTB_PROTO})
add_custom_command(
    OUTPUT ${PARSER_SRCS} ${PARSER_HDRS}
    COMMAND ${PROTOG_BIN} -p ${OPENRTB_PROTO} -i openrtb.pb.h -m com.google.openrtb.BidRequest)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(protogd main.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PARSER_SRCS})
target_link_libraries(protogd ${THIRDPARTY_LIBRARIES} yajl)
