//
// Created by parsy_b on 3/4/20.
//

#ifndef ELPLANT_ENGINE_H
#define ELPLANT_ENGINE_H

#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>
#include "../grammar/Grammar.h"
#include "../utils/Vector.h"

using line = std::pair<sf::Vertex, sf::Vertex>;
using lines = std::vector<line>;

struct Mesh {
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<std::array<unsigned, 3>> faces;

    void save_obj(const std::string& path) const;
    void merge_mesh(const Mesh& m);
};

struct Cylinder {
    Vec3f o;
    Vec3f d;
    double r, h;

    Cylinder(const Vec3f& o, const Vec3f& d, double r, double h) : o(o), d(d), r(r), h(h) {}

    Mesh to_mesh(unsigned n, unsigned rings) const;

    friend std::ostream& operator<<(std::ostream& os, Cylinder const& c) {
        return os << "C[O: " << c.o << ", D: " << c.d << ", R: " << c.r << ", H:" << c.h << "]";
    }
    friend YAML::Emitter& operator << (YAML::Emitter& out, Cylinder const& c) {
        return out << YAML::BeginMap
        << YAML::Key << "type" << YAML::Value << "cylinder"
        << YAML::Key << "base" << YAML::Value << c.o
        << YAML::Key << "axis" << YAML::Value << c.d * c.h
        << YAML::Key << "radius" << YAML::Value << c.r
        << YAML::EndMap;
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
    void render(const Grammar& g, int n, double angle, double length) const;
    void save(cylinders const& cls, const char* path);
protected:
    unsigned width_, height_;
};


#endif //ELPLANT_ENGINE_H
