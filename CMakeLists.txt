cmake_minimum_required(VERSION 3.27)

project(
        lab_8
        VERSION 1.0
        DESCRIPTION "template c++"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)