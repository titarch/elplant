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
