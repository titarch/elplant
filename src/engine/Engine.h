//
// Created by parsy_b on 3/4/20.
//

#ifndef ELPLANT_ENGINE_H
#define ELPLANT_ENGINE_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../grammar/Grammar.h"
#include "../utils/Vector.h"

using line = std::pair<sf::Vertex, sf::Vertex>;
using lines = std::vector<line>;

struct Turtle {
    Vec2f o;
    double d;

    Turtle(const Vec2f& o, double d) : o(o), d(d) {}
    friend std::ostream& operator<<(std::ostream& os, Turtle const& t) {
        return os << t.o << ' ' << t.d << "°";
    }
};

class Engine {
public:
    Engine(unsigned width, unsigned height) : width_(width), height_(height) {}

    [[nodiscard]] lines draw(std::string const& s, double angle, double length) const;
    void render(const Grammar &g, int n,  double angle, double length) const;
protected:
    unsigned width_, height_;
};


#endif //ELPLANT_ENGINE_H
