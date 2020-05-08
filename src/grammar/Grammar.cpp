//
// Created by parsy_b on 3/4/20.
//

#include "Grammar.h"

void Rule::add_rule(const String& rvalue, unsigned weight) {
    rvalues_.push_back(rvalue);
    weights_.push_back(weight);

    update_summed_weights();
}

String Rule::get_rule() const {
    // FIXME: Use better random function
    unsigned rnd = (rand() % (weight_sum_));
    for (unsigned i = 0; i < weights_.size(); i++) {
        if (rnd < weights_[i])
            return rvalues_[i];
        rnd -= weights_[i];
    }
    return "";
}

void Rule::update_summed_weights() {
    weight_sum_ = std::accumulate(weights_.begin(), weights_.end(), 0u);
}

Grammar& Grammar::add_rule(char lvalue, const String& rvalue, unsigned weight) {
    auto found_rule = rules_.find(lvalue);
    if (found_rule == rules_.end())
        rules_.insert({lvalue, Rule(lvalue, rvalue)});
    else
        found_rule->second.add_rule(rvalue, weight);
    return *this;
}

String Grammar::generate(int n) const {
    String buffer;
    generate_rec(buffer, axiom_, n, 0);

    return buffer;
}

void Grammar::generate_rec(String& buffer, const String& cur_rule, int max_rec,
                           int cur_rec) const {
    if (cur_rec >= max_rec) {
        if (buffer.size() + cur_rule.size() > buffer.capacity())
            buffer.reserve(buffer.size() * 2);
        buffer.append(cur_rule);

        return;
    }
    for (unsigned i = 0; i < cur_rule.size(); i++) {
        auto found_rule = rules_.find(cur_rule[i]);
        if (found_rule == rules_.end())
            buffer.push_back(cur_rule[i]);
        else
            generate_rec(buffer, found_rule->second.get_rule(), max_rec, cur_rec + 1);
    }
}

String ParamRule::substitute(String const& s, std::vector<double> const& values) const {
    String os;
    for (auto const& c : s) {
        if (auto it = std::find(params_.cbegin(), params_.cend(), c); it != params_.cend()) {
            os += std::to_string(values[it - params_.cbegin()]);
        } else
            os.push_back(c);
    }
    return os;
}

String ParamRule::evaluate(std::vector<double> const& values_) const {
    for (auto const& c : crs_) {
        if (values_.empty())
            return c.rvalue;
        if (c.evaluate(values_[c.lpos]))
            return substitute(c.rvalue, values_);
    }
    std::stringstream ss;
    ss << "Rule mismatch";
    for(auto i = 0u; i < params_.size(); ++i)
        ss << ", " << params_[i] << " = " << values_[i];
    throw std::runtime_error(ss.str());
}

String ParamGrammar::generate_rec(String const& in, int cur_rec, int max_rec) const {
    String out{};
    if (cur_rec == max_rec) return in;
    for (auto c = in.cbegin(); c != in.cend();) {
        if (rules_.contains(*c)) {
            auto const& rule_c = *c;
            auto const& rule = rules_.at(rule_c);
            ++c;
            Strings exps;
            if (c != in.cend() && *c == '(') {
                String exp{};
                ++c;
                for (; *c != ')'; ++c) {
                    if (*c == ',') {
                        exps.push_back(exp);
                        exp.clear();
                    } else {
                        exp.push_back(*c);
                    }
                }
                exps.push_back(exp);
                ++c;
            }
            std::vector<double> value = evaluate(exps);
            try { out += rule.evaluate(value); } catch (std::runtime_error const& e) {
                std::stringstream ss;
                ss << "Function " << rule_c << ": " << e.what() << "; rec_n = " << cur_rec << " (maybe try to set n lower than this value)";
                throw std::runtime_error(ss.str());
            }
        } else {
            out.push_back(*c);
            ++c;
        }
    }
    return generate_rec(out, cur_rec + 1, max_rec);
}

double evaluate(String const& s) {
    static exprtk::expression<double> expr;
    static exprtk::parser<double> parser;
    if (!parser.compile(s, expr))
        throw std::runtime_error(s + ": could not evaluate expression (maybe some parameters where not declared and could not be substituted)");
    return expr.value();
}

std::vector<double> evaluate(Strings const& exps) {
    std::vector<double> v{};
    v.reserve(exps.size());
    std::transform(exps.cbegin(), exps.cend(), std::back_inserter(v),
                   [](String const& s) -> double { return evaluate(s); });
    return v;
}
