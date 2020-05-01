set(grammar_src_files
        Grammar.h
        Grammar.cpp conditional.h)
add_library(grammar ${grammar_src_files})
add_library(expr exprtk.hpp)
set_target_properties(expr PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(grammar PRIVATE expr)