set(engine_src_files
        Engine.h
        Engine.cpp)
add_library(engine ${engine_src_files})
target_link_libraries(engine PUBLIC sfml-window sfml-graphics)
