set(grammar_src_files
        Grammar.h
        Grammar.cpp)
add_library(grammar ${grammar_src_files})
target_link_libraries(grammar PUBLIC)