//
// Created by mattrouss on 4/3/20.
//

#ifndef ELPLANT_PLANT_HH
#define ELPLANT_PLANT_HH

#include "Shapes.hh"

struct Plant {
    cylinders cyls;
    leaves lvs;

    Plant() {};

    void add_cylinder(Cylinder &c);
    void add_leaf(Leaf &l);

    Mesh to_mesh(unsigned cylinder_faces, unsigned cylinder_rings) const;

};


#endif //ELPLANT_PLANT_HH
