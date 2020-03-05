//
// Created by parsy_b on 3/4/20.
//

#include <stack>
#include <cctype>
#include <cmath>
#include <iostream>
#include "Engine.h"
#include "../utils/Vector.h"
#include "../utils/Matrix.h"

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
    std::stack<Cylinder> turtles;
    turtles.emplace(Vec3f{}, UnitVec3f::U, thickness);
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
                turtle.d = RotMat3f::U(angle) * turtle.d;
                break;
            case '-':
                turtle.d = RotMat3f::U(-angle) * turtle.d;
                break;
            case '&':
                turtle.d = RotMat3f::L(angle) * turtle.d;
                break;
            case '^':
                turtle.d = RotMat3f::L(-angle) * turtle.d;
                break;
            case '\\':
                turtle.d = RotMat3f::H(angle) * turtle.d;
                break;
            case '/':
                turtle.d = RotMat3f::H(-angle) * turtle.d;
                break;
            case '|':
                turtle.d = RotMat3f::U(M_PI) * turtle.d;
                break;
            default:
                if (!isalpha(c))
                    throw std::invalid_argument("Bad character " + std::to_string(c));
                const auto& dl = turtle.d * length;
                Cylinder cyl(turtle.o, dl, turtle.r);
                cyls.push_back(cyl);
                turtle.o += dl;
        }
    }
    return cyls;
}

void Engine::render(const lines& lines) const {
    sf::RenderWindow window(sf::VideoMode(width_, height_), "sfml-elplant");

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (auto const& line : lines) {
            sf::Vertex vline[2] = {line.first, line.second};
            window.draw(vline, 2, sf::Lines);
        }

        window.display();
    }
}