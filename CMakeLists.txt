cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 20)

add_executable(code main.cpp
        src/sentenceslice.h
        stl/mymap.h)

add_compile_options(-O3)