//
// Created by mattrouss on 4/3/20.
//

#include "Plant.h"

void Plant::add_cylinder(Cylinder &c) {
    cyls.emplace_back(c);
}

void Plant::add_leaf(Leaf &l) {
    lvs.emplace_back(l);
}

void Plant::add_icosphere(IcoSphere &i) {
    isphs.emplace_back(i);
}

Mesh Plant::to_mesh(unsigned cylinder_faces, unsigned cylinder_rings) const {
    Mesh m;
    for (auto const& c: cyls)
        m.merge_mesh(c.to_mesh(cylinder_faces, cylinder_rings));
    for (auto const& l: lvs)
        m.merge_mesh(l.to_mesh());
    return m;
}

void Plant::save_plant(std::string const& obj_path, std::string const& mtl_path, std::vector<Material> const& materials) const {
    std::ofstream obj_out(obj_path);
    size_t num_vertices = 0, num_cyls = 0, num_leaves = 0, num_ico = 0;
    Mesh m;

    obj_out << "mtllib " << mtl_path << std::endl;
    for (auto const& c: cyls) {
        obj_out << "o Cylinder" << num_cyls << std::endl;
        obj_out << "usemtl " << materials[c.color_index % materials.size()].name << std::endl;
        m = c.to_mesh(10, 2);
        m.save_obj(obj_out, num_vertices);
        num_vertices += m.vertices.size();
        num_cyls++;
    }

    for (auto const& l: lvs) {
        obj_out << "o Leaf" << num_leaves << std::endl;
        obj_out << "usemtl " << materials[l.color_index].name << std::endl;
        m = l.to_mesh();
        m.save_obj(obj_out, num_vertices);
        num_vertices += m.vertices.size();
        num_leaves++;
    }

    for (auto const& ico: isphs) {
        obj_out << "o IcoSphere" << num_ico << std::endl;
        obj_out << "usemtl " << materials[ico.color_index].name << std::endl;
        m = ico.to_mesh();
        m.save_obj(obj_out, num_vertices);
        num_vertices += m.vertices.size();
        num_ico++;
    }

    std::ofstream mtl_out(mtl_path);
    for (auto const& mtl: materials)
        mtl_out << mtl;

}
