cmake_minimum_required(VERSION 3.0)
project(SquareEquationSolver)

set(SRC src/console.cpp src/helpapp.cpp src/setterapp.cpp
    src/getterapp.cpp src/solverapp.cpp)
set(TESTING_SRC test/testing.cpp)
include_directories(include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -std=c++14 -Wall -Wextra -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=gnu++14 -Wall -Wextra -Wuninitialized")

add_executable(solver src/main.cpp ${SRC})
add_executable(unit_testing test/main.cpp ${SRC} ${TESTING_SRC})
