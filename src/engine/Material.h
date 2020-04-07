//
// Created by mattrouss on 4/4/20.
//

#pragma once

#include "../utils/Vector.h"

struct Material {
    std::string name;
    Vec3f Kd, Ks, Ka, Ke;
    double Ns, Ni, d;

    static const Material White;
    static const Material Red;
    static const Material Green;
    static const Material Blue;
    static const Material Yellow;

    Material() {}

    Material(const std::string& name, Vec3f const& Kd, Vec3f const& Ks,
             Vec3f const& Ka = {{1.0, 1.0, 1.0}},
             Vec3f const& Ke = {{0.0, 0.0, 0.0}},
             double Ns = 100.0, double Ni = 1.45, double d = 1.0) : name(name), Kd(Kd), Ks(Ks), Ka(Ka), Ke(Ke), Ns(Ns),
                                                                    Ni(Ni), d(d) {}

    Material(Material const& m) = default;
    Material& operator=(const Material& m);

    static Material from_rgb(std::string const& color_name, Vec3f const& rgb);

    std::string get_name() const;
    friend std::ostream& operator<<(std::ostream& out, Material const& m);
    friend YAML::Emitter& operator<<(YAML::Emitter& out, Material const& mat);
};


using materials = std::vector<Material>;

