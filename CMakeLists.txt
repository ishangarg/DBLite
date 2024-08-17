cmake_minimum_required(VERSION 3.27)
project(DBLite)

set(CMAKE_CXX_STANDARD 23)

add_executable(DBLite main.cpp
        table.h
        pager.h)
