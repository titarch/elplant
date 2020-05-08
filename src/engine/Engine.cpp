//
// Created by parsy_b on 3/4/20.
//

#include <stack>
#include <cctype>
#include <cmath>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string/replace.hpp>
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

double get_param(std::string const& s, unsigned& i, double def) {
    if (s[i + 1] != '(') return def;
    std::string exp;
    i += 2;
    for (; s[i] != ',' && s[i] != ')'; ++i)
        exp.push_back(s[i]);
    for(; s[i] != ')'; ++i);
    return evaluate(exp);
}

double get_angle(std::string const& s, unsigned& i, double default_angle) {
    return get_param(s, i, default_angle) * M_PI / 180;
}

double get_length(std::string const& s, unsigned& i, double default_length) {
    return get_param(s, i, default_length);
}


Leaf Engine::draw_leaf(std::string const& s, unsigned& i,
                       std::stack<SeaTurtle>& turtles, double angle, double length) const {
    double real_angle = angle;
    double real_length = length;
    angle = angle * M_PI / 180;
    Leaf l(turtles.top().color_index);

    while (s[i] != '}') {
        auto& turtle = turtles.top();
        switch (s[i]) {
            case '[':
                turtles.push(turtle);
                break;
            case ']':
                turtles.pop();
                break;
            case '+':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.u, real_angle));
                break;
            case '-':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.u, -real_angle));
                break;
            case '&':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.l, real_angle));
                break;
            case '^':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.l, -real_angle));
                break;
            case '\\':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.d, real_angle));
                break;
            case '/':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.d, -real_angle));
                break;
            case '|':
                turtle.rotate(Mat3f::R(turtle.u, M_PI));
                break;
            case '`':
                turtle.color_index++;
                break;
            case '.':
                l.add_vertex(turtle.o);
                break;
            case '(':
                for (; s[i] != ')'; ++i);
                break;
            case 'G':
                real_length = get_param(s, i, length);
                turtle.o += turtle.d * real_length;
                break;
            case 'f':
                l.add_vertex(turtle.o);
                turtle.o += turtle.d * length;
                break;
            default:
                break;
        }
        i++;
    }

    return l;
}


Plant Engine::draw(const std::string& s, double angle, double length,
                   double thickness, double sph_radius, std::optional<Tropism> const& t) const {
    double real_angle = angle;
    double real_length = length;
    Plant plt;
    Leaf l;
    IcoSphere ico;
    Cylinder cyl;
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
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.u, real_angle));
                break;
            case '-':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.u, -real_angle));
                break;
            case '&':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.l, real_angle));
                break;
            case '^':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.l, -real_angle));
                break;
            case '\\':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.d, real_angle));
                break;
            case '/':
                real_angle = get_angle(s, i, angle);
                turtle.rotate(Mat3f::R(turtle.d, -real_angle));
                break;
            case '|':
                turtle.rotate(Mat3f::R(turtle.u, M_PI));
                break;
            case '$':
                turtle.l = (UnitVec3f::H ^ turtle.d).normalized();
                turtle.u = turtle.d ^ turtle.l;
                break;
            case '%':
                for (; s[i] != '\0' && s[i] != ']'; ++i);
                break;
            case '`':
                turtle.color_index++;
                break;
            case '!':
                turtle.r = get_param(s, i, 0.75 * turtle.r);
                break;
            case '{':
                l = draw_leaf(s, i, turtles, angle, length);
                plt.add_leaf(l);
                break;
            case 'b':
                ico = IcoSphere(turtle.o, sph_radius, turtle.color_index);
                plt.add_icosphere(ico);
                break;
            case 'F':
                cyl = dynamic_cast<Cylinder&>(turtle);
                real_length = get_length(s, i, length);
                cyl.h = real_length;
                plt.add_cylinder(cyl);
                [[fallthrough]];
            case 'f':
                turtle.o += turtle.d * real_length;
                if (t.has_value()) {
                    turtle.bend(t.value());
                }
                break;
            case '(':
                for (; s[i] != ')'; ++i);
                break;
            default:
                if (!isalpha(s[i]))
                    throw std::runtime_error(std::string("Bad character: `") + s[i] + "'");
        }
    }
    return plt;
}

static grammar_ptr parse_classic_rules(YAML::Node const& rules, std::string const& axiom) {
    auto gram = std::make_unique<Grammar>(axiom);
    for (YAML::const_iterator it = rules.begin(); it != rules.end(); ++it) {
        auto lhs = it->first.as<char>();
        auto rhs = it->second.as<std::string>();
        gram->add_rule(lhs, rhs);
    }
    return gram;
}

static grammar_ptr parse_parametric_rules(YAML::Node const& rules, std::string const& axiom) {
    auto gram = std::make_unique<ParamGrammar>(axiom);
    auto map = rules["map"] ? rules["map"].as<dict_t>() : dict_t{};
    for (YAML::const_iterator it = rules.begin(); it != rules.end(); ++it) {
        if (it->first.as<String>() == "map") continue;
        auto c = it->first.as<char>();
        auto const& rhs = it->second;
        auto params = rhs["parameters"] ? rhs["parameters"].as<std::vector<char>>() : std::vector<char>{};
        ParamRule rule{params};
        for (auto const& cond : rhs["conds"]) {
            auto param = cond["parameter"] ? cond["parameter"].as<char>() : (params.empty() ? '?' : params[0]);
            auto op = cond["op"] ? cond["op"].as<Op>() : Op::TRUE;
            auto val = cond["value"] ? cond["value"].as<double>() : 0.0;
            auto rval = cond["rvalue"].as<std::string>();
            for (const auto& entry : map)
                boost::replace_all(rval, entry.first, entry.second);
            try {
                rule.add_conditional_rule(param, op, val, rval);
            } catch (std::invalid_argument const& e) {
                std::stringstream ss;
                ss << "Function `" << c << "'" << ": " << e.what();
                throw YAML::Exception(YAML::Mark::null_mark(), ss.str());
            }
        }
        gram->add_rule(c, rule);
    }
    return gram;
}

std::vector<GrammarData> Engine::load_grammars(const std::string& path) const {
    std::vector<GrammarData> gds;
    YAML::Node node = YAML::LoadFile(path);
    if (!node["grammars"]) throw std::invalid_argument(path + ": no grammars where found.");
    auto gs = node["grammars"];
    for (YAML::const_iterator git = gs.begin(); git != gs.end(); ++git) {
        auto name = git->first.as<std::string>();
        auto g = git->second;
        try {
            auto type = g["type"] ? g["type"].as<std::string>() : "classic";
            auto axiom = g["axiom"].as<std::string>();
            grammar_ptr gram;
            const auto& rules = g["rules"];
            if (type == "parametric")
                gram = parse_parametric_rules(rules, axiom);
            else
                gram = parse_classic_rules(rules, axiom);
            auto angle = g["angle"] ? g["angle"].as<double>() : 30;
            auto n = g["n"].as<int>();
            double length = g["length"] ? g["length"].as<double>() : 1;
            double thickness = g["thickness"] ? g["thickness"].as<double>() : 1;
            double sph_radius = g["sph_radius"] ? g["sph_radius"].as<double>() : 1;
            materials mtls{};
            if (g["colors"]) {
                for (const auto& c : g["colors"])
                    mtls.push_back(Material::from_rgb(c["name"].as<std::string>(), c["rgb"].as<Vec3f>()));
            } else mtls.push_back(Material::White);
            Camera c{};
            if (g["camera"]) {
                const auto& cam = g["camera"];
                c = Camera(cam["origin"].as<Vec3f>(), cam["forward"].as<Vec3f>(), cam["up"].as<Vec3f>());
            }
            std::optional<Tropism> t = std::nullopt;
            if (g["tropism"]) {
                const auto& tropism = g["tropism"];
                t = Tropism(tropism["T"].as<Vec3f>(), tropism["bend"].as<double>());
            }
            gds.emplace_back(name, std::move(gram), angle, n, length, thickness, sph_radius, mtls, c, t);
        } catch (YAML::Exception const& e) {
            std::cerr << name << ": " << e.what() << std::endl;
        }
    }
    return gds;
}

void Engine::render(std::string const& path) const {
    auto gds = load_grammars(path);
    auto gidx = 0u;
    GrammarData& gd = gds[gidx];

    auto lines = draw(gd.g->generate(gd.n), gd.angle, 1);
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
                    lines = draw(gd.g->generate(gd.n), gd.angle, 1);
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

void Engine::render3D(const std::string& path) const {
    auto gds = load_grammars(path);
    for (const auto& gd : gds) {
        std::cerr << "Generating " << gd.name << "..." << std::flush;
        try {
            Plant p = draw(gd.g->generate(gd.n), gd.angle, gd.length, gd.thickness, gd.sph_radius, gd.t);
            std::string output = "../output/";
            output += gd.name + ".";
            p.save_plant(output + "obj", output + "mtl", gd.mtls);
            save(p, gd.mtls, gd.cam, output + "yaml");
            std::cerr << " OK!" << std::endl;
        } catch (std::runtime_error const& e) {
            std::cerr << " KO! " << e.what() << std::endl;
        }
    }
}

void Engine::save(const Plant& plant, materials const& mtls, Camera const& cam, std::string const& path) const {
    YAML::Emitter o;
    o << YAML::BeginMap;
    if (!cam.empty)
        o << YAML::Key << "camera" << YAML::Value << cam;
    o << YAML::Key << "textures" << YAML::Value << YAML::BeginSeq;
    for (const auto& m : mtls) o << m;
    o << YAML::EndSeq << YAML::Key << "objects" << YAML::Value << YAML::BeginMap << YAML::Key << "solids"
      << YAML::Value << YAML::BeginSeq;
    for (const auto& c : plant.cyls) o << c;
    for (const auto& s : plant.isphs) o << s;
    for (const auto& l : plant.lvs) o << l;
    o << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
    std::ofstream file(path);
    if (!file)
        throw std::runtime_error(path + ": could not open file");
    file << o.c_str();
}

