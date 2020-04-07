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
    turtles.emplace(Vec2f{}, 0);
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

std::vector<GrammarData> Engine::load_grammars(const std::string& path) const {
    std::vector<GrammarData> gds;
    YAML::Node node = YAML::LoadFile(path);
    for (const auto& g : node["grammars"]) {
        auto name = g["name"].as<std::string>();
        auto axiom = g["axiom"].as<std::string>();
        Grammar gram(axiom);
        const auto& rules = g["rules"];
        for (YAML::const_iterator it = rules.begin(); it != rules.end(); ++it) {
            auto lhs = it->first.as<char>();
            auto rhs = it->second.as<std::string>();
            gram.add_rule(lhs, rhs);
        }
        auto angle = g["angle"].as<double>();
        auto n = g["n"].as<int>();
        gds.emplace_back(name, gram, angle, n);
    }
    return gds;
}

void Engine::render(std::string const& path) const {
    auto gds = load_grammars(path);
    if (gds.empty())
        throw std::invalid_argument("No grammar found in Yaml file");
    auto gidx = 0u;
    auto gd = gds[gidx];

    auto lines = draw(gd.g.generate(gd.n), gd.angle, 1);
    normalize(lines, (float) width_, (float) height_, 0.9);
    sf::RenderWindow window(sf::VideoMode(width_, height_), "sfml-elplant");

    sf::Font font;
    font.loadFromFile("../font/term.ttf");
    sf::Text tname(gd.name + " n=" + std::to_string(gd.n), font, 42);
    tname.setFillColor(sf::Color::White);


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
                            ++gd.n;
                            break;
                        case sf::Keyboard::BackSpace:
                            --gd.n;
                            if (gd.n < 0) gd.n = 0;
                            break;
                        case sf::Keyboard::N:
                            gidx = (gidx + 1) % gds.size();
                            gd = gds[gidx];
                            break;
                        default:
                            break;
                    }
                    lines = draw(gd.g.generate(gd.n), gd.angle, 1);
                    normalize(lines, (float) width_, (float) height_, 0.9);
                    tname.setString(gd.name + " n=" + std::to_string(gd.n));
                    break;
                default:
                    break;
            }
        }
        window.clear();

        for (auto const& line : lines)
            window.draw(line.data(), 2, sf::Lines);
        window.draw(tname);
        window.display();
    }
}

void Engine::save(const Plant& plant, materials const& mtls, const char* path) {
    YAML::Emitter o;
    o << YAML::BeginMap << YAML::Key << "textures" << YAML::Value << YAML::BeginSeq;
    for (const auto& m : mtls) o << m;
    o << YAML::EndSeq << YAML::Key << "objects" << YAML::Value << YAML::BeginMap << YAML::Key << "solids"
      << YAML::Value << YAML::BeginSeq;
    for (const auto& c : plant.cyls) o << c;
    for (const auto& l : plant.lvs) o << l;
    o << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
    std::ofstream file(path);
    file << o.c_str();
}

