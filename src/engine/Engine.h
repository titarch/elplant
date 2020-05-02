//
// Created by parsy_b on 3/4/20.
//

#ifndef ELPLANT_ENGINE_H
#define ELPLANT_ENGINE_H

#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <utility>
#include <yaml-cpp/yaml.h>
#include "../grammar/Grammar.h"
#include "../utils/Vector.h"
#include "../utils/Matrix.h"
#include "Material.h"
#include "Shapes.hh"
#include "Plant.hh"

struct SeaTurtle : public Cylinder {
    Vec3f l, u;

    SeaTurtle(const Vec3f& o, double r, double h, unsigned color_index) : Cylinder(o, UnitVec3f::H, r, h, color_index),
                                                                          l(UnitVec3f::L), u(UnitVec3f::U) {}

    void rotate(const Mat3f& r) {
        d = r * d;
        u = r * u;
        l = r * l;
    }
};

struct Turtle {
    Vec2f o;
    double d;

    Turtle(const Vec2f& o, double d) : o(o), d(d) {}

    friend std::ostream& operator<<(std::ostream& os, Turtle const& t) {
        return os << t.o << ' ' << t.d << "°";
    }
};

struct Camera {
    Vec3f origin, forward, up;
    bool empty;

    Camera() : empty(true) {};

    Camera(const Vec3f& origin, const Vec3f& forward, const Vec3f& up) : origin(origin), forward(forward), up(up),
                                                                         empty(false) {}

    friend YAML::Emitter& operator<<(YAML::Emitter& out, Camera const& c) {
        return out << YAML::BeginMap
                   << YAML::Key << "origin" << c.origin
                   << YAML::Key << "forward" << c.forward
                   << YAML::Key << "up" << c.up
                   << YAML::EndMap;
    }
};

struct GrammarData {
    std::string name;
    BaseGrammar* g;
    double angle;
    int n;
    double length, thickness, sph_radius;
    materials mtls;
    Camera cam;

    GrammarData(std::string name, BaseGrammar* g, double angle, int n, double length, double thickness,
                double sph_radius, materials mtls, const Camera& cam)
            : name(std::move(name)), g(g), angle(angle), n(n), length(length), thickness(thickness),
              sph_radius(sph_radius), mtls(std::move(mtls)), cam(cam) {}
};

class Engine {
public:
    Engine(unsigned width, unsigned height) : width_(width), height_(height) {}

    [[nodiscard]] lines draw(std::string const& s, double angle, double length) const;
    [[nodiscard]] Leaf
    draw_leaf(std::string const& s, unsigned& index, SeaTurtle& turtle, double angle, double length) const;
    [[nodiscard]] Plant
    draw(std::string const& s, double angle, double length, double thickness, double sph_radius) const;
    [[nodiscard]] std::vector<GrammarData> load_grammars(std::string const& path) const;
    void render(std::string const& path) const;
    void render3D(std::string const& path) const;
    void save(Plant const& plant, materials const& mtls, Camera const& cam, std::string const& path) const;
protected:
    unsigned width_, height_;
};

#endif //ELPLANT_ENGINE_H
