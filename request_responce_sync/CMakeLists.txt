project (testZMQ CXX )

find_library(ZeroMQ_LIBRARY NAMES zmq PATHS "/home/aponyatov/_libZMQ/libzmq_pc/build/lib")

include_directories(
    "/home/aponyatov/_ZMQ/zmqPC"
    "/home/aponyatov/_libZMQ/libzmq_pc/include"
)

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(Threads REQUIRED)

set(SOURCES_SERVER
    ${CMAKE_CURRENT_SOURCE_DIR}/server_responce.cpp
)

set(SOURCES_CLIENT
    ${CMAKE_CURRENT_SOURCE_DIR}/client_request.cpp
)

add_executable(server_responce ${SOURCES_SERVER})
add_executable(client_request ${SOURCES_CLIENT})



target_link_libraries(server_responce ${ZeroMQ_LIBRARY} Threads::Threads)
target_link_libraries(client_request ${ZeroMQ_LIBRARY})




