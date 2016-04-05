set(THIRDPARTY_LIBRARIES)

include(${CMAKE_CURRENT_LIST_DIR}/LibFindMacros.cmake)

# boost
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.48.0 REQUIRED COMPONENTS date_time filesystem system log thread)
include_directories(${Boost_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBRARIES ${Boost_LIBRARIES})

# Google Protobuf
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBRARIES ${PROTOBUF_LIBRARIES})

# Google's Flatbuffersj
list(APPEND THIRDPARTY_LIBRARIES /usr/local/lib/libflatbuffers.a)

# curl
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBRARIES ${CURL_LIBRARIES})

# OpenSSL
find_package(OPENSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBRARIES ${OPENSSL_LIBRARIES})

# Open MPI's Portable Hardware Locality (required by httpp)
include(${CMAKE_CURRENT_LIST_DIR}/FindHWLOC.cmake)
include_directories(${HWLOC_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBRARIES ${HWLOC_LIBRARIES})

# Intel Threading Building Blocks (a.k.a TBB)
include(${CMAKE_CURRENT_LIST_DIR}/FindTBB.cmake)
include_directories(${TBB_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBRARIES ${TBB_LIBRARIES})

# daedric's httpp
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/httpp/include/)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/httpp/build/include/)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/httpp/third_party/commonpp/include/)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/httpp/third_party/commonpp/build/include/)

list(APPEND THIRDPARTY_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/httpp/build/src/httpp/libhttpp.a)
list(APPEND THIRDPARTY_LIBRARIES ${CMAKE_SOURCE_DIR}/thirdparty/httpp/third_party/commonpp/build/src/commonpp/libcommonpp.a)
