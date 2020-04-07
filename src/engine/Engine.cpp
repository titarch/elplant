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
            case 'f':
                turtle.o += angle_dir(turtle.d) * length;
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

Leaf Engine::draw_leaf(std::string const& s, unsigned& index,
                       SeaTurtle& turtle, double angle, double length) const {
    std::vector<Vec3f> vertices;
    while (s[index] != '}') {
        switch (s[index]) {
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
            case '`':
                turtle.color_index++;
                break;
            case 'f':
                vertices.push_back(turtle.o);
                turtle.o += turtle.d * length;
                break;
        }
        index++;
    }

    return Leaf(vertices, turtle.color_index);
}

Plant Engine::draw(const std::string& s, double angle, double length, double thickness) const {
    angle = angle * M_PI / 180;
    Plant plt;
    Leaf l;
    std::stack<SeaTurtle> turtles;
    turtles.emplace(Vec3f{}, thickness, length, 0);
    for (unsigned i = 0; i < s.size(); i++) {
        auto& turtle = turtles.top();
        switch (s[i]) {
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
            case '`':
                turtle.color_index++;
                break;
            case '!':
                turtle.r *= 0.75;
                break;
            case '{':
                l = draw_leaf(s, i, turtle, angle, length);
                plt.add_leaf(l);
                break;
            case 'F':
                plt.add_cylinder(dynamic_cast<Cylinder&>(turtle));
                [[fallthrough]];
            case 'f':
                turtle.o += turtle.d * length;
                break;
            default:
                if (!isalpha(s[i]))
                    throw std::invalid_argument("Bad character " + std::to_string(s[i]));
        }
    }
    return plt;
}

void Engine::render(const Grammar& g, int n, double angle, double length) const {
    auto lines = draw(g.generate(n), angle, length);
    normalize(lines, (float) width_, (float) height_, 0.9);
    sf::RenderWindow window(sf::VideoMode(width_, height_), "sfml-elplant");

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case (sf::Event::Closed):
                    window.close();
                    break;
                case (sf::Event::KeyPressed):
                    switch (event.key.code) {
                        case sf::Keyboard::Space:
                            lines = draw(g.generate(++n), angle, length);
                            break;
                        case sf::Keyboard::BackSpace:
                            lines = draw(g.generate(--n), angle, length);
                            break;
                        default:
                            break;
                    }
                    normalize(lines, (float) width_, (float) height_, 0.9);
                    break;
                default:
                    break;
            }
        }
        window.clear();

        for (auto const& line : lines)
            window.draw(line.data(), 2, sf::Lines);

        window.display();
    }
}

void Engine::save(const cylinders& cls, const char* path) {
    YAML::Emitter o;
    o << YAML::BeginMap << YAML::Key << "objects" << YAML::Value << YAML::BeginMap << YAML::Key << "solids"
      << YAML::Value << YAML::BeginSeq << YAML::Flow << cls << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
    std::ofstream file(path);
    file << o.c_str();
}
