//
// Created by mattrouss on 4/3/20.
//

#include "Plant.hh"

void Plant::add_cylinder(Cylinder &c) {
    cyls.emplace_back(c);
}

void Plant::add_leaf(Leaf &l) {
    lvs.emplace_back(l);
}

Mesh Plant::to_mesh(unsigned cylinder_faces, unsigned cylinder_rings) const {
    Mesh m;
    for (auto const& c: cyls)
        m.merge_mesh(c.to_mesh(cylinder_faces, cylinder_rings));
    for (auto const& l: lvs)
        m.merge_mesh(l.to_mesh());
    return m;
}

void Plant::save_plant(std::string const &path) const {
    std::ofstream out(path);
    out << "o Cylinders" << std::endl;
    Mesh cylinder_mesh;
    for (auto const& c: cyls)
        cylinder_mesh.merge_mesh(c.to_mesh(10, 2));
    cylinder_mesh.save_obj(out, 0);

    out << "o Leaves" << std::endl;
    Mesh leaves_mesh;
    for (auto const& l: lvs) {
        leaves_mesh.merge_mesh(l.to_mesh());
    }
    leaves_mesh.save_obj(out, cylinder_mesh.vertices.size());
}
