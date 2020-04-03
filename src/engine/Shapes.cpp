//
// Created by mattrouss on 4/3/20.
//

#include <fstream>
#include "Shapes.hh"

Mesh Cylinder::to_mesh(unsigned n, unsigned rings) const {
    Mesh m;
    unsigned i, ring;
    double a, da, c, s, z;
    da = 2 * M_PI / double(n - 1);
    double dh = h / (rings - 1);
    double angle_H = d.get_angle_H();
    auto h_axis = (UnitVec3f::H ^ d).normalized();
    // Don't count beginning and end vertices twice
    unsigned num_vertices = n * rings - rings;
    for (a = 0.0f, i = 0; i < n-1; a += da, i++) {
        c = r * cos(a);
        s = r * sin(a);
        for (z = 0.0f, ring = 0; ring < rings; z += dh, ring++) {
            Vec3f vertex = Vec3f{{c, s, z}};
            if (angle_H != 0)
                vertex = Mat3f::R(h_axis, angle_H) * vertex;
            m.vertices.emplace_back(vertex + o);
            m.normals.emplace_back(Vec3f{{c - o[0], s - o[1], 0}}.normalized());
            if (ring == rings - 1)
                continue;
            unsigned e0, e1, e2, e3;
            e0 = i * rings + ring;
            e1 = ((i + 1) % n * rings + ring) % num_vertices;
            e2 = i * rings + ring + 1;
            e3 = ((i + 1) % n * rings + ring + 1) % num_vertices;
            m.faces.emplace_back(std::vector<unsigned>{e0, e1, e3, e2});
        }
    }
    return m;
}

void Mesh::save_obj(const std::string& path) const {
    std::ofstream out(path);
    for (auto const& v: vertices)
        out << "v " << v[0] << " " << v[1] << " " << v[2] << "\n";
    out << "\n";

    for (auto const& n: normals)
        out << "vn " << n[0] << " " << n[1] << " " << n[2] << "\n";
    out << "\n";

    for (auto const& f: faces) {
        out << "f ";
        for (auto const& v: f)
            out << v + 1 << " ";
        out << "\n";
    }

}

void Mesh::merge_mesh(const Mesh& m) {
    unsigned faces_size = vertices.size();
    vertices.insert(vertices.end(), m.vertices.begin(), m.vertices.end());
    normals.insert(normals.end(), m.normals.begin(), m.normals.end());
    for (auto face : m.faces) {
        std::vector<unsigned> new_face;
        for (auto v : face)
            new_face.emplace_back(v + faces_size);
        faces.emplace_back(new_face);
    }

}

Mesh Leaf::to_mesh() const {
    Mesh m;
    unsigned n = vertices.size();
    std::vector<Vec3f> normals;
    std::vector<unsigned> face;
    for (unsigned i = 0; i < n; i++) {
        m.vertices.emplace_back(vertices[i]);
        face.emplace_back(i);
        if (i == 0) {
            m.normals.emplace_back(vertices[n - 1] ^ vertices[1]);
        } else if (i == n - 1) {
            m.normals.emplace_back(vertices[n - 2] ^ vertices[0]);
        } else {
            m.normals.emplace_back(vertices[i - 1] ^ vertices[i + 1]);
        }
    }
    m.faces.emplace_back(face);

    return m;
}
