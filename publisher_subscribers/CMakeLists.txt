cmake_minimum_required(VERSION 3.16)

project (testZMQ CXX)

find_library(ZeroMQ_LIBRARY NAMES zmq PATHS "/home/aponyatov/_libZMQ/libzmq_pc/build/lib")

include_directories(
    "/home/aponyatov/_ZMQ/zmqPC"
    "/home/aponyatov/_libZMQ/libzmq_a10/include"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++14")

set(SOURCES_SERVER
    ${CMAKE_CURRENT_SOURCE_DIR}/pub.cpp
)

set(SOURCES_CLIENT
    ${CMAKE_CURRENT_SOURCE_DIR}/sub.cpp
)

add_executable(pub ${SOURCES_SERVER})
add_executable(sub ${SOURCES_CLIENT})



target_link_libraries(pub ${ZeroMQ_LIBRARY})
target_link_libraries(sub ${ZeroMQ_LIBRARY})



