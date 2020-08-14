cmake_minimum_required(VERSION 3.15)
project(ppt-fe59)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DDEBUG)

add_compile_options(-O1 -g -Wall -Wpedantic -Wnull-dereference -Wnon-virtual-dtor)

add_executable(ppt_fe59
        part.cpp
        part.hpp
        inventory.cpp
        inventory.hpp
        controller.cpp
        controller.hpp
        exception.hpp
        main.cpp
        utility.cpp
        utility.hpp)
