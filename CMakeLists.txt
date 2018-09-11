cmake_minimum_required(VERSION 3.0)
project(SquareEquationSolver)

set(SRC src/main.cpp src/console.cpp)
include_directories(include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14 -Wall -Wextra")

add_executable(solver ${SRC})
