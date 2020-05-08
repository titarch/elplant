//
// Created by mattrouss on 4/3/20.
//

#ifndef ELPLANT_SHAPES_H
#define ELPLANT_SHAPES_H

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

struct Triangle {
    Vec3f v0, v1, v2;
    unsigned color_index;

    Triangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, unsigned int colorIndex) : v0(v0), v1(v1), v2(v2),
                                                                                           color_index(colorIndex) {}

    friend YAML::Emitter& operator<<(YAML::Emitter& out, Triangle const& t) {
        return out << YAML::BeginMap
                   << YAML::Key << "type" << YAML::Value << "triangle"
                   << YAML::Key << "v0" << YAML::Value << t.v0
                   << YAML::Key << "v1" << YAML::Value << t.v1
                   << YAML::Key << "v2" << YAML::Value << t.v2
                   << YAML::Key << "tex" << YAML::Value << t.color_index
                   << YAML::EndMap;
    }
};

using TriangleMesh = std::vector<Triangle>;

struct Cylinder {
    Vec3f o;
    Vec3f d;
    double r, h;
    unsigned color_index;

    Cylinder() : o(), d(), r(), h(), color_index() {}

    Cylinder(const Vec3f& o, const Vec3f& d, double r, double h, unsigned color_index) : o(o), d(d), r(r), h(h),
                                                                                         color_index(color_index) {}

    [[nodiscard]] Mesh to_mesh(unsigned n, unsigned rings) const;

    friend std::ostream& operator<<(std::ostream& os, Cylinder const& c) {
        return os << "C[O: " << c.o << ", D: " << c.d << ", R: " << c.r << ", H:" << c.h << "]";
    }

    friend YAML::Emitter& operator<<(YAML::Emitter& out, Cylinder const& c) {
        return out << YAML::BeginMap
                   << YAML::Key << "type" << YAML::Value << "cylinder"
                   << YAML::Key << "base" << YAML::Value << c.o
                   << YAML::Key << "axis" << YAML::Value << c.d * c.h
                   << YAML::Key << "radius" << YAML::Value << c.r
                   << YAML::Key << "tex" << YAML::Value << c.color_index
                   << YAML::EndMap;
    }
};

struct Leaf {
    std::vector<Vec3f> vertices;
    unsigned color_index;

    Leaf() : vertices{}, color_index{} {}
    Leaf(unsigned color_index) : vertices{}, color_index{color_index} {}

    Leaf(std::vector<Vec3f> vertices, unsigned color_index) : vertices(std::move(vertices)), color_index(color_index) {}

    void add_vertex(Vec3f const& v);

    [[nodiscard]] Mesh to_mesh() const;
    [[nodiscard]] TriangleMesh to_triangles() const;

    friend YAML::Emitter& operator<<(YAML::Emitter& out, Leaf const& l) {
        for (auto const& t : l.to_triangles()) out << t;
        return out;
    }
};

struct IcoSphere {
    Vec3f center;
    double radius;
    unsigned color_index, n;

    IcoSphere() : center{}, radius{}, color_index{}, n{} {}

    IcoSphere(Vec3f const& center, double radius, unsigned color_index, unsigned n)
            : center(center), radius(radius), color_index(color_index), n(n) {}

    Mesh get_unit_sphere_mesh() const;
    Mesh subdivide_mesh(Mesh const& m) const;
    [[nodiscard]] Mesh to_mesh() const;

    friend YAML::Emitter& operator<<(YAML::Emitter& out, IcoSphere const& s) {
        return out << YAML::BeginMap
                   << YAML::Key << "type" << YAML::Value << "sphere"
                   << YAML::Key << "origin" << YAML::Value << s.center
                   << YAML::Key << "radius" << YAML::Value << s.radius
                   << YAML::Key << "tex" << YAML::Value << s.color_index
                   << YAML::EndMap;
    }
};

using cylinders = std::vector<Cylinder>;
using leaves = std::vector<Leaf>;
using icospheres = std::vector<IcoSphere>;


#endif //ELPLANT_SHAPES_H
