cmake_minimum_required(VERSION 4.0)

project(Bullet  VERSION 1.0
                DESCRIPTION "Performant HTTP server in C++"
                LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Werror)

add_executable(bullet src/main.cpp)
