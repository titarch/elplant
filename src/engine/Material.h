//
// Created by mattrouss on 4/4/20.
//

#pragma once

#include "../utils/Vector.h"

struct Material {
    std::string name;
    Vec3f Kd, Ks, Ka, Ke;
    double Ns, Ni, d;

    Material() {}
    Material(const std::string &name, Vec3f const& Kd, Vec3f const& Ks,
            Vec3f const& Ka = {{1.0, 1.0, 1.0}},
            Vec3f const& Ke = {{0.0, 0.0, 0.0}},
            double Ns = 100.0, double Ni = 1.45, double d = 1.0 ): name(name), Kd(Kd), Ks(Ks), Ka(Ka), Ke(Ke), Ns(Ns), Ni(Ni), d(d){}

    Material(Material const& m) = default;
    Material& operator=(const Material &m);

    std::string get_name() const;
    friend std::ostream& operator <<(std::ostream& out, Material const& m);

};

namespace DefaultMtl {
    const auto White = Material("white", {{1.0, 1.0, 1.0}}, {{0.5, 0.5, 0.5}});
    const auto Red = Material("red", {{1.0, 0.0, 0.0}}, {{0.5, 0.5, 0.5}});
    const auto Green = Material("green", {{0.0, 1.0, 0.0}}, {{0.5, 0.5, 0.5}});
    const auto Blue = Material("blue", {{0.0, 0.0, 1.0}}, {{0.5, 0.5, 0.5}});
    const auto Yellow = Material("yellow", {{1.0, 1.0, 0.0}}, {{0.5, 0.5, 0.5}});
}


using materials = std::vector<Material>;

