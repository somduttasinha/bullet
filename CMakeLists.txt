cmake_minimum_required(VERSION 4.0)

project(Bullet  VERSION 1.0
                DESCRIPTION "Performant HTTP server in C++"
                LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
    src/http/HttpParser.cpp)


add_executable(bullet ${SOURCES})

target_include_directories(bullet PRIVATE
    src
    src/http
    src/utils)

add_compile_options(-Wall -Werror)
