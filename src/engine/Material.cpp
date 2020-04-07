//
// Created by mattrouss on 4/4/20.
//

#include "Material.h"

std::ostream &operator<<(std::ostream &out, Material const& m) {
    out.precision(6);
    out << "newmtl " << std::fixed << m.name << "\n";
    out << "Ns "  << m.Ns << "\n";
    out << "Ka "  << m.Ka[0] << " " << m.Ka[1] << " " << m.Ka[2] << "\n";
    out << "Kd "  << m.Kd[0] << " " << m.Kd[1] << " " << m.Kd[2] << "\n";
    out << "Ks "  << m.Ks[0] << " " << m.Ks[1] << " " << m.Ks[2] << "\n";
    out << "Ke "  << m.Ke[0] << " " << m.Ke[1] << " " << m.Ke[2] << "\n";
    out << "Ni "  << m.Ni << "\n";
    out << "d " << m.d << "\n";
    out << "illum 2" << "\n";

    return out;
}

std::string Material::get_name() const {
    return name;
}

Material& Material::operator=(Material const& m) {
    name = m.name;
    Kd = m.Kd;
    Ks = m.Ks;
    Ka = m.Ka;
    Ke = m.Ke;
    Ns = m.Ns;
    Ni = m.Ni;
    d = m.d;

    return *this;
}
