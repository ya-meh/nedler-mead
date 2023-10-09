cmake_minimum_required(VERSION 3.25)
project(nedler_mead)

set(CMAKE_CXX_STANDARD 20)

add_executable(nedler_mead main.cpp utility.h nelder_mead.h)
