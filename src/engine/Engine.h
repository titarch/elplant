//
// Created by parsy_b on 3/4/20.
//

#ifndef ELPLANT_ENGINE_H
#define ELPLANT_ENGINE_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../utils/Vector.h"

using line = std::pair<sf::Vertex, sf::Vertex>;
using lines = std::vector<line>;

struct Cylinder {
    Vec3f o;
    Vec3f d;
    double r;

    Cylinder(const Vec3f& o, const Vec3f& d, double r) : o(o), d(d), r(r) {}

    friend std::ostream& operator<<(std::ostream& os, Cylinder const& c) {
        return os << "C[O: " << c.o << ", D: " << c.d << ", R: " << c.r << "]";
    }
};

using cylinders = std::vector<Cylinder>;

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
    [[nodiscard]] cylinders draw(std::string const& s, double angle, double length, double thickness) const;

    void render(lines const& lines) const;

protected:
    unsigned width_, height_;
};


#endif //ELPLANT_ENGINE_H
