//
// Created by mattrouss on 4/3/20.
//

#ifndef ELPLANT_PLANT_HH
#define ELPLANT_PLANT_HH

#include "Shapes.hh"
#include <fstream>

struct Plant {
    cylinders cyls;
    leaves lvs;
    icospheres isphs;

    Plant() {};

    void add_cylinder(Cylinder &c);
    void add_leaf(Leaf &l);
    void add_icosphere(IcoSphere &i);

    Mesh to_mesh(unsigned cylinder_faces, unsigned cylinder_rings) const;
    void save_plant(std::string const& obj_path, std::string const& mtl_path, std::vector<Material> const& materials) const;

};


#endif //ELPLANT_PLANT_HH
