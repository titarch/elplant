set(engine_src_files
        Engine.h
        Engine.cpp
        Plant.cpp
        Plant.hh
        Shapes.cpp
        Shapes.hh Material.cpp Material.h)
add_library(engine ${engine_src_files})
target_link_libraries(engine PUBLIC sfml-window sfml-graphics grammar)
