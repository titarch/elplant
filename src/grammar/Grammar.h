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

using String = std::string;
using Strings = std::vector<String>;

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

class Grammar {
public:
    explicit Grammar(String axiom) : axiom_(std::move(axiom)) {
        srand((unsigned) time(0));
    }

    void add_rule(char lvalue, const String& rvalue, unsigned weight = 1);
    [[nodiscard]] String generate(int n) const;
private:
    void generate_rec(String& buffer, const String& cur_rule, int max_rec, int cur_rec) const;

    String axiom_;
    std::map<char, Rule> rules_;
};


enum class Op {
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,
    TRUE,
    FALSE
};

struct Condition {
    Op op;
    double rval;

    Condition(Op op, double rval) : op(op), rval(rval) {}

    [[nodiscard]] bool evaluate(double var) const {
        switch (op) {
            case Op::LT:
                return var < rval;
            case Op::LE:
                return var <= rval;
            case Op::GT:
                return var > rval;
            case Op::GE:
                return var >= rval;
            case Op::EQ:
                return var == rval;
            case Op::NE:
                return var != rval;
            case Op::TRUE:
                return true;
            case Op::FALSE:
                return false;
        }
        return false;
    }
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

    String evaluate(std::vector<double> const& values_);

private:
    String substitute(const String& s, const std::vector<double>& values);
    std::vector<char> params_;
    std::vector<ConditionRule> crs_;
};

class ParamGrammar {
public:
    explicit ParamGrammar(String axiom) : axiom_(std::move(axiom)), rules_() {}

    ParamGrammar& add_rule(char c, ParamRule const& pr) {
        rules_.insert({c, pr});
        return *this;
    }

    String generate(int n) {
        return generate_rec(axiom_, 0, n);
    }

private:
    String generate_rec(String const& in, int cur_rec, int max_rec);

    String axiom_;
    std::map<char, ParamRule> rules_;
};

double evaluate(String const& s);
std::vector<double> evaluate(Strings const& exps);

#endif //ELPLANT_GRAMMAR_H
