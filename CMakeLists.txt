cmake_minimum_required(VERSION 3.10)
project(elplant)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_NAME elplant)

set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG    "-g  -Wall -Wextra -Werror -pedantic")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(YAML-CPP REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(src)

