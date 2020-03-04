//
// Created by parsy_b on 3/4/20.
//

#ifndef ELPLANT_GRAMMAR_H
#define ELPLANT_GRAMMAR_H

#include <iostream>
#include <string>
#include <map>

using String = std::string;

class Grammar {
public:
    Grammar(const String axiom, float angle): axiom_(axiom), angle_(angle) {}

    void add_rule(char lvalue, const String &rvalue);
    String generate(int n) const;

private:
    void generate_rec(String &buffer, const String &cur_rule, int max_rec, int cur_rec) const;

    const String axiom_;
    std::map<char, String> rules_;
    float angle_;
};


#endif //ELPLANT_GRAMMAR_H
