//
// Created by parsy_b on 3/4/20.
//

#ifndef ELPLANT_GRAMMAR_H
#define ELPLANT_GRAMMAR_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <utility>
#include <random>
#include "exprtk.hpp"
#include "conditional.h"

using String = std::string;
using Strings = std::vector<String>;


class BaseGrammar {
public:
    BaseGrammar() = default;
    [[nodiscard]] virtual String generate(int n) const = 0;
};


class Rule {
public:
    Rule(char lvalue, const String& rvalue, unsigned weight = 1) : lvalue_(lvalue) {
        rvalues_ = std::vector<String>();
        rvalues_.push_back(rvalue);

        weights_ = std::vector<unsigned>();
        weights_.push_back(weight);

        update_summed_weights();
    }

    void add_rule(const String& rvalue, unsigned weight = 1);
    [[nodiscard]] String get_rule() const;

private:
    char lvalue_;
    std::vector<String> rvalues_;
    std::vector<unsigned> weights_;

    unsigned weight_sum_ = 0;

    void update_summed_weights();
};

class Grammar : public BaseGrammar {
public:
    explicit Grammar(String axiom) : BaseGrammar(), axiom_(std::move(axiom)) {
        srand((unsigned) time(nullptr));
    }

    Grammar& add_rule(char lvalue, const String& rvalue, unsigned weight = 1);
    [[nodiscard]] String generate(int n) const override;
private:
    void generate_rec(String& buffer, const String& cur_rule, int max_rec, int cur_rec) const;

    String axiom_;
    std::map<char, Rule> rules_;
};

struct ConditionRule {
    Condition c;
    size_t lpos;
    String rvalue;

    ConditionRule(const Condition& c, size_t lpos, String rvalue) : c(c), lpos(lpos), rvalue(std::move(rvalue)) {}

    [[nodiscard]] bool evaluate(double var) const {
        return c.evaluate(var);
    }
};

class ParamRule {
public:
    explicit ParamRule(std::vector<char> params) : params_(std::move(params)), crs_() {}

    ParamRule& add_conditional_rule(char param, Op op, double value, String const& rvalue) {
        auto it = std::find(params_.cbegin(), params_.cend(), param);
        if (it == params_.cend()) throw std::invalid_argument(String("Parameter ") + param + " was not found");
        crs_.emplace_back(Condition{op, value}, it - params_.cbegin(), rvalue);
        return *this;
    }

    [[nodiscard]] String evaluate(std::vector<double> const& values_) const;

private:
    [[nodiscard]] String substitute(const String& s, const std::vector<double>& values) const;
    std::vector<char> params_;
    std::vector<ConditionRule> crs_;
};

class ParamGrammar : public BaseGrammar {
public:
    explicit ParamGrammar(String axiom) : BaseGrammar(), axiom_(std::move(axiom)), rules_() {}

    ParamGrammar& add_rule(char c, ParamRule const& pr) {
        rules_.insert({c, pr});
        return *this;
    }

    [[nodiscard]] String generate(int n) const override {
        return generate_rec(axiom_, 0, n);
    }

private:
    [[nodiscard]] String generate_rec(String const& in, int cur_rec, int max_rec) const;

    String axiom_;
    std::map<char, ParamRule> rules_;
};

double evaluate(String const& s);
std::vector<double> evaluate(Strings const& exps);

#endif //ELPLANT_GRAMMAR_H
