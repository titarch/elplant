//
// Created by parsy_b on 3/4/20.
//

#include "Grammar.h"

void Rule::add_rule(const String &rvalue, unsigned weight) {
    rvalues_.push_back(rvalue);
    weights_.push_back(weight);

    update_summed_weights();
}

String Rule::get_rule() const {
    // FIXME: Use better random function
    unsigned rnd = (rand() % (weight_sum_));
    for(unsigned i = 0; i < weights_.size(); i++) {
        if(rnd < weights_[i])
            return rvalues_[i];
        rnd -= weights_[i];
    }
    return "";
}

void Rule::update_summed_weights() {
    weight_sum_ = std::accumulate(weights_.begin(), weights_.end(), 0u);
}

void Grammar::add_rule(char lvalue, const String &rvalue, unsigned weight) {
    auto found_rule = rules_.find(lvalue);
    if (found_rule == rules_.end())
        rules_.insert({lvalue, Rule(lvalue, rvalue)});
    else
        found_rule->second.add_rule(rvalue, weight);
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
        if (buffer.size() + cur_rule.size() > buffer.capacity())
            buffer.reserve(buffer.size() * 2);
        buffer.append(cur_rule);

        return;
    }
    for (unsigned i = 0; i < cur_rule.size(); i++)
    {
        auto found_rule = rules_.find(cur_rule[i]);
        if (found_rule == rules_.end())
            buffer.push_back(cur_rule[i]);
        else
            generate_rec(buffer, found_rule->second.get_rule(), max_rec, cur_rec + 1);
    }
}
