//
// Created by parsy_b on 3/4/20.
//

#include "Grammar.h"

void Grammar::add_rule(char lvalue, const String &rvalue) {
    rules_.insert({lvalue, rvalue});

}

String Grammar::generate(int n) const {
    String buffer;
    generate_rec(buffer, axiom_, n, 0);

    return buffer;
}

void Grammar::generate_rec(String &buffer, const String &cur_rule, int max_rec,
                             int cur_rec) const {
    if (cur_rec >= max_rec)
    {
        buffer.append(cur_rule);
        return;
    }
    for (unsigned i = 0; i < cur_rule.size(); i++)
    {
        auto found_rule = rules_.find(cur_rule[i]);
        if (found_rule == rules_.end())
            buffer.push_back(cur_rule[i]);
        else
            generate_rec(buffer, found_rule->second, max_rec, cur_rec + 1);
    }
}
