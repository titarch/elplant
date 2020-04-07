//
// Created by mattrouss on 4/3/20.
//

#ifndef ELPLANT_SHAPES_HH
#define ELPLANT_SHAPES_HH

#include <SFML/Graphics/Vertex.hpp>
#include <utility>
#include "../utils/Vector.h"
#include "../utils/Matrix.h"
#include "Material.h"

//using line = std::pair<sf::Vertex, sf::Vertex>;
using line = std::array<sf::Vertex, 2>;
using lines = std::vector<line>;

void normalize(lines& ls, float width, float height, float stickiness);

struct Mesh {
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<std::vector<unsigned>> faces;
    void save_obj(std::ofstream& out, unsigned curr_vertices) const;

    void merge_mesh(const Mesh& m);
};

struct Cylinder {
    Vec3f o;
    Vec3f d;
    double r, h;
    unsigned color_index;

    Cylinder(const Vec3f& o, const Vec3f& d, double r, double h, unsigned color_index) : o(o), d(d), r(r), h(h), color_index(color_index) {}

    Mesh to_mesh(unsigned n, unsigned rings) const;

    friend std::ostream& operator<<(std::ostream& os, Cylinder const& c) {
        return os << "C[O: " << c.o << ", D: " << c.d << ", R: " << c.r << ", H:" << c.h << "]";
    }

    friend YAML::Emitter& operator<<(YAML::Emitter& out, Cylinder const& c) {
        return out << YAML::BeginMap
                   << YAML::Key << "type" << YAML::Value << "cylinder"
                   << YAML::Key << "base" << YAML::Value << c.o
                   << YAML::Key << "axis" << YAML::Value << c.d * c.h
                   << YAML::Key << "radius" << YAML::Value << c.r
                   << YAML::EndMap;
    }
};

struct Leaf {
    std::vector<Vec3f> vertices;
    unsigned color_index;

    Leaf() {}

    Leaf(std::vector<Vec3f> vertices, unsigned color_index): vertices(std::move(vertices)), color_index(color_index) {}

    Mesh to_mesh() const;
};

using cylinders = std::vector<Cylinder>;
using leaves = std::vector<Leaf>;


#endif //ELPLANT_SHAPES_HH