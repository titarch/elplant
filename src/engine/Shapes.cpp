//
// Created by mattrouss on 4/3/20.
//

#include <fstream>
#include "Shapes.hh"

void normalize(lines& ls, float width, float height, float stickiness) {
    float xmin = INFINITY, xmax = 0, ymin = INFINITY, ymax = 0;
    for (const auto& l : ls) {
        for (const auto& v : l) {
            const auto& pos = v.position;
            if (pos.x < xmin)
                xmin = pos.x;
            if (pos.x > xmax)
                xmax = pos.x;
            if (pos.y < ymin)
                ymin = pos.y;
            if (pos.y > ymax)
                ymax = pos.y;
        }
    }

    float kx = width / (xmax - xmin);
    float ky = height / (ymax - ymin);
    float k = kx < ky ? kx : ky;
    k *= 2 * stickiness - 1;
    for (auto& l : ls) {
        for (auto& v : l) {
            auto& p = v.position;
            p.x = (p.x - xmin) * k + (1 - stickiness) * width;
            p.y = (p.y - ymin) * k + (1 - stickiness) * height;
        }
    }
}

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
    for (a = 0.0f, i = 0; i < n - 1; a += da, i++) {
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

void Mesh::save_obj(std::ofstream& out, unsigned curr_vertices) const {
    for (auto const& v: vertices)
        out << "v " << v[0] << " " << v[1] << " " << v[2] << "\n";
    for (auto const& n: normals)
        out << "vn " << n[0] << " " << n[1] << " " << n[2] << "\n";
    out << "s off" << "\n";
    for (auto const& f: faces) {
        out << "f ";
        for (auto const& v: f)
            out << v + curr_vertices + 1 << " ";
        out << "\n";
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

TriangleMesh Leaf::to_triangles() const {
    TriangleMesh tm{};
    for (auto i = 1u; i < vertices.size() - 1; ++i)
        tm.emplace_back(vertices[0], vertices[i], vertices[i + 1], color_index);
    return tm;
}
