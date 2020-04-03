//
// Created by parsy_b on 3/4/20.
//

#include <stack>
#include <cctype>
#include <cmath>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "Engine.h"

static Vec2f angle_dir(double t) {
    return Vec2f{{std::sin(M_PI * t / 180), -std::cos(M_PI * t / 180)}};
}

static sf::Vertex convert(Vec2f const& v) {
    return sf::Vertex{sf::Vector2f{(float) v[0], (float) v[1]}};
}

lines Engine::draw(const std::string& s, double angle, double length) const {
    lines lines;
    std::stack<Turtle> turtles;
    turtles.emplace(Vec2f{{(double) width_ / 2, (double) height_ / 2}}, 0);
    for (char const& c : s) {
        auto& turtle = turtles.top();
        switch (c) {
            case '[':
                turtles.push(turtle);
                break;
            case ']':
                turtles.pop();
                break;
            case '+':
                turtle.d += angle;
                while (turtle.d > 360) turtle.d -= 360;
                break;
            case '-':
                turtle.d -= angle;
                while (turtle.d < -360) turtle.d += 360;
                break;
            default:
                if (!isalpha(c))
                    throw std::invalid_argument("Bad character " + std::to_string(c));
                auto f = turtle.o + angle_dir(turtle.d) * length;
                line line = {convert(turtle.o), convert(f)};
                lines.push_back(line);
                turtle.o = f;
        }
    }
    return lines;
}

cylinders Engine::draw(const std::string& s, double angle, double length, double thickness) const {
    angle = angle * M_PI / 180;
    cylinders cyls;
    std::stack<SeaTurtle> turtles;
    turtles.emplace(Vec3f{}, thickness, length);
    for (char const& c : s) {
        auto& turtle = turtles.top();
        switch (c) {
            case '[':
                turtles.push(turtle);
                break;
            case ']':
                turtles.pop();
                break;
            case '+':
                turtle.rotate(Mat3f::R(turtle.u, angle));
                break;
            case '-':
                turtle.rotate(Mat3f::R(turtle.u, -angle));
                break;
            case '&':
                turtle.rotate(Mat3f::R(turtle.l, angle));
                break;
            case '^':
                turtle.rotate(Mat3f::R(turtle.l, -angle));
                break;
            case '\\':
                turtle.rotate(Mat3f::R(turtle.d, angle));
                break;
            case '/':
                turtle.rotate(Mat3f::R(turtle.d, -angle));
                break;
            case '|':
                turtle.rotate(Mat3f::R(turtle.u, M_PI));
                break;
            case '!':
                turtle.r *= 0.75;
                break;
            case 'F':
                cyls.push_back(dynamic_cast<Cylinder&>(turtle));
                // fall through
            case 'f':
                turtle.o += turtle.d * length;
                break;
            default:
                if (!isalpha(c))
                    throw std::invalid_argument("Bad character " + std::to_string(c));
        }
    }
    return cyls;
}

void Engine::render(const Grammar& g, int n, double angle, double length) const {
    auto lines = draw(g.generate(n), angle, length);
    sf::RenderWindow window(sf::VideoMode(width_, height_), "sfml-elplant");

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case (sf::Event::Closed):
                    window.close();
                    break;
                case (sf::Event::KeyPressed):
                    lines = draw(g.generate(n), angle, length);
                    break;
                default:
                    break;
            }
        }
        window.clear();

        for (auto const& line : lines) {
            sf::Vertex vline[2] = {line.first, line.second};
            window.draw(vline, 2, sf::Lines);
        }

        window.display();
    }
}

void Engine::save(const cylinders& cls, const char *path) {
    YAML::Emitter o;
    o << YAML::BeginMap << YAML::Key << "objects" << YAML::Value << YAML::BeginMap << YAML::Key << "solids"
      << YAML::Value << YAML::BeginSeq << YAML::Flow << cls << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
    std::ofstream file(path);
    file << o.c_str();
}

Mesh Cylinder::to_mesh(unsigned n, unsigned rings) const {
    Mesh m;
    unsigned i, ring;
    double a, da, c, s, z;
    da = 2 * M_PI / double(n - 1);
    double dh = h / (rings - 1);
    double angle_H = d.get_angle_H();
    auto h_axis = (UnitVec3f::H ^ d).normalized();
    // Don't count beginning and end vertices twice
    unsigned num_vertices = n * rings - rings;
    for (a = 0.0f, i = 0; i < n-1; a += da, i++) {
        c = r * cos(a);
        s = r * sin(a);
        for (z = 0.0f, ring = 0; ring < rings; z += dh, ring++) {
            Vec3f vertex = Vec3f{{c, s, z}};
            if (angle_H != 0)
                vertex = Mat3f::R(h_axis, angle_H) * vertex;
            m.vertices.emplace_back(vertex + o);
            m.normals.emplace_back(Vec3f{{c - o[0], s - o[1], 0}}.normalized());
            if (ring == rings - 1)
                continue;
            unsigned e0, e1, e2, e3;
            e0 = i * rings + ring;
            e1 = ((i + 1) % n * rings + ring) % num_vertices;
            e2 = i * rings + ring + 1;
            e3 = ((i + 1) % n * rings + ring + 1) % num_vertices;
            m.faces.emplace_back(std::array<unsigned, 4>{e0, e1, e3, e2});
        }
    }
    return m;
}

void Mesh::save_obj(const std::string& path) const {
    std::ofstream out(path);
    for (auto const& v: vertices)
        out << "v " << v[0] << " " << v[1] << " " << v[2] << "\n";
    out << "\n";

    for (auto const& n: normals)
        out << "vn " << n[0] << " " << n[1] << " " << n[2] << "\n";
    out << "\n";

    for (auto const& f: faces) {
        out << "f ";
        for (auto const& v: f)
            out << v + 1 << " ";
        out << "\n";
    }

}

void Mesh::merge_mesh(const Mesh& m) {
    unsigned faces_size = vertices.size();
    vertices.insert(vertices.end(), m.vertices.begin(), m.vertices.end());
    normals.insert(normals.end(), m.normals.begin(), m.normals.end());
    for (auto face : m.faces)
        faces.emplace_back(std::array<unsigned, 4>{face[0] + faces_size, face[1] + faces_size, face[2] + faces_size, face[3] + faces_size});

}
