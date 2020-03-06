cmake_minimum_required(VERSION 3.10)
project(elplant)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_NAME elplant)

set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG    "-g  -Wall -Wextra")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_subdirectory(src)

