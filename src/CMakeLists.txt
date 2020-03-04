# Add new subdirectories here

add_executable(${EXECUTABLE_NAME} main.cpp)
set(EXECUTABLE_OUTPUT_PATH ..)

target_link_libraries(${EXECUTABLE_NAME} sfml-window sfml-graphics)