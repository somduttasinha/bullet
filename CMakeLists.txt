cmake_minimum_required(VERSION 4.0)

project(Bullet  VERSION 1.0
                DESCRIPTION "Performant HTTP server in C++"
                LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(fmt REQUIRED)

add_compile_options(-Wall -Werror)

# Create a library with all non-main source files
add_library(bullet_lib
    src/http/HttpParser.cpp
    src/http/HttpMessage.cpp
    src/actor/actor.cpp
    # Add more .cpp files here as your project grows
)

target_include_directories(bullet_lib PUBLIC
    src
    src/http
    src/utils
    src/actor
)

# Main executable - just needs main.cpp and the library
add_executable(bullet src/main.cpp)
target_link_libraries(bullet bullet_lib)
target_link_libraries(bullet_lib PRIVATE fmt::fmt)

# GoogleTest setup
include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test executable - just link the library!
add_executable(parser_test
    tests/parser/parser_test.cpp
)

add_executable(http_message_test
    tests/message/http_message_test.cpp
)


target_link_libraries(parser_test
    GTest::gtest_main
    bullet_lib  # All your code is here
)

target_link_libraries(http_message_test
    GTest::gtest_main
    bullet_lib  # All your code is here
)

include(GoogleTest)
gtest_discover_tests(parser_test)
gtest_discover_tests(http_message_test)
