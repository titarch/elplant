add_subdirectory(utils)
add_subdirectory(grammar)
add_subdirectory(engine)

add_executable(${EXECUTABLE_NAME} main.cpp)
set(EXECUTABLE_OUTPUT_PATH ..)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC tbb yaml-cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE grammar engine)
