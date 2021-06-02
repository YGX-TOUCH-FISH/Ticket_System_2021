cmake_minimum_required(VERSION 3.16)
project(TrainStation)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Ofast ")

add_executable(code
        Alloc.hpp
        BPT.hpp
        date.hpp
        exception.hpp
        main.cpp
        Map.hpp
        Order.cpp
        Order.h
        program.cpp
        program.h
        String.hpp
        Ticket.cpp
        Ticket.h
        Train.cpp
        Train.h
        Users.cpp
        Users.h
        Vector.hpp
        )