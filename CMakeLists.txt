cmake_minimum_required(VERSION 3.16)
project(TrainStation)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Ofast ")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/datastructure)

add_executable(code
        src/main.cpp
        src/program.cpp
        src/Order.cpp
        src/Ticket.cpp
        src/Train.cpp
        src/Users.cpp)