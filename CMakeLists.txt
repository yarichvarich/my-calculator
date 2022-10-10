cmake_minimum_required(VERSION 3.22)

project(my-calculator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB calculator_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} ${calculator_SRC})

