//
// Created by parsy_b on 3/4/20.
//

#ifndef ELPLANT_GRAMMAR_H
#define ELPLANT_GRAMMAR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>

using String = std::string;

class Rule {
public:
    Rule(char lvalue, const String &rvalue, unsigned weight = 1): lvalue_(lvalue)
    {
        rvalues_ = std::vector<String>();
        rvalues_.push_back(rvalue);

        weights_ = std::vector<unsigned>();
        weights_.push_back(weight);

        update_summed_weights();
    }

    void add_rule(const String &rvalue, unsigned weight = 1);
    String get_rule() const;

private:
    char lvalue_;
    std::vector<String> rvalues_;
    std::vector<unsigned> weights_;

    unsigned weight_sum_ = 0;

    void update_summed_weights();
};


class Grammar {
public:
    Grammar(const String axiom): axiom_(axiom){}

    void add_rule(char lvalue, const String &rvalue, unsigned weight = 1);
    String generate(int n) const;

private:
    void generate_rec(String &buffer, const String &cur_rule, int max_rec, int cur_rec) const;

    const String axiom_;
    std::map<char, Rule> rules_;
};


#endif //ELPLANT_GRAMMAR_H
