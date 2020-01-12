cmake_minimum_required(VERSION 3.0.0)
project(cppthings VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_compile_options("-O3")
add_compile_options("-mtune=native")
add_compile_options("-march=native")

include_directories(thirdparty/ctre/include thirdparty include)

add_executable(cppthings main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
