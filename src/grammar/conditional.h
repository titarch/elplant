//
// Created by bparsy on 5/1/20.
//

#ifndef ELPLANT_CONDITIONAL_H
#define ELPLANT_CONDITIONAL_H

#include <yaml-cpp/yaml.h>

enum class Op {
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,
    TRUE
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
        }
        return false;
    }
};

namespace YAML {
    template<>
    struct convert<Op> {
        static bool decode(const Node& node, Op& rhs) {
            auto opstr = node.as<std::string>();

            if (opstr == "lt")
                rhs = Op::LT;
            else if (opstr == "le")
                rhs = Op::LE;
            else if (opstr == "gt")
                rhs = Op::GT;
            else if (opstr == "ge")
                rhs = Op::GE;
            else if (opstr == "eq")
                rhs = Op::EQ;
            else if (opstr == "ne")
                rhs = Op::NE;
            else if (opstr == "true")
                rhs = Op::TRUE;
            else {
                std::cerr << opstr << ": wrong op type; " << std::flush;
                return false;
            }
            return true;
        }
    };
}

#endif //ELPLANT_CONDITIONAL_H
