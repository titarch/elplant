//
// Created by mattrouss on 4/3/20.
//

#include <fstream>
#include <iostream>
#include "Shapes.h"

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

void Leaf::add_vertex(Vec3f const& v) {
    vertices.emplace_back(v);
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
    if (vertices.empty()) return tm;
    for (auto i = 1u; i < vertices.size() - 1; ++i)
        tm.emplace_back(vertices[0], vertices[i], vertices[i + 1], color_index);
    return tm;
}

Mesh IcoSphere::get_unit_sphere_mesh() const {
    Mesh m;
    const float horizontal_angle = M_PI / 180 * 72;    // 72 degree = 360 / 5
    const float elevation = atanf(1.0f / 2);  // elevation = 26.565 degree
    int i1, i2;
    float z, xy;
    float hAngle1 = -M_PI / 2 - horizontal_angle / 2;  // start from -126 deg at 1st row
    float hAngle2 = -M_PI / 2;                // start from -90 deg at 2nd row

    m.vertices.resize(12);
    // First top vertex at (0, 0, r)
    m.vertices[0] = Vec3f{{0, 0, radius}};

    // Compute 10 vertices at 1st and 2nd rows
    for (int i = 1; i <= 5; ++i) {
        i1 = i;         // index for 1st row
        i2 = i + 5;   // index for 2nd row

        z = radius * sinf(elevation);
        xy = radius * cosf(elevation);
        m.vertices[i1] = Vec3f{{xy * cosf(hAngle1), xy * sinf(hAngle1), z}};
        m.vertices[i2] = Vec3f{{xy * cosf(hAngle2), xy * sinf(hAngle2), -z}};

        // Next horizontal angles
        hAngle1 += horizontal_angle;
        hAngle2 += horizontal_angle;
    }
    // Last bottom vertex at (0, 0, -r)
    m.vertices[11] = Vec3f{{0, 0, -radius}};

    // Get surface normals
    for (auto& v: m.vertices) {
        m.normals.emplace_back((v - center));
    }

    // Add 20 triangles
    static constexpr unsigned faces[20][3] = {
            {0,  1,  2},
            {0,  2,  3},
            {0,  3,  4},
            {0,  4,  5},
            {0,  5,  1},
            {11, 7,  6},
            {11, 8,  7},
            {11, 9,  8},
            {11, 10, 9},
            {11, 6,  10},
            {6,  1,  10},
            {1,  6,  2},
            {1,  5,  10},
            {6,  7,  2},
            {3,  2,  7},
            {3,  7,  8},
            {4,  3,  8},
            {4,  8,  9},
            {5,  4,  9},
            {5,  9,  10}
    };

    for (auto i = 0u; i < 20u; ++i)
        m.faces.push_back(std::vector<unsigned>{faces[i][0], faces[i][1], faces[i][2]});

    return m;
}

Mesh IcoSphere::subdivide_mesh(Mesh const& m) const {
    Mesh res;
    auto faces = m.faces;
    auto idx = 0u;
    for (auto i = 0u; i < faces.size(); i++) {
        auto v1 = m.vertices[faces[i][0]];
        auto v2 = m.vertices[faces[i][1]];
        auto v3 = m.vertices[faces[i][2]];

        // Compute center of each triangle edge, rescaled to radius of sphere
        auto mid1 = radius * (v1 + v2).normalized();
        auto mid2 = radius * (v2 + v3).normalized();
        auto mid3 = radius * (v1 + v3).normalized();

        const Vec3f verts[12] = {
            v1, mid1, mid3,
            mid1, v2, mid2,
            mid1, mid2, mid3,
            mid3, mid2, v3
        };

        // Add vertices and normals
        for (auto& v: verts) {
            res.vertices.emplace_back(v);
            res.normals.emplace_back((v - center));
        }

        // Add new triangles in the correct order
        for(auto j = 0u; j < 4; j++)
            res.faces.emplace_back(std::vector<unsigned>{idx + j * 3, idx + j * 3 + 1, idx + j * 3 + 2});

        idx += 12;

    }

    return res;
}

Mesh IcoSphere::to_mesh() const {
    Mesh m = get_unit_sphere_mesh();

    for (auto i = 0u; i < n; i++)
        m = subdivide_mesh(m);

    for (auto& v: m.vertices)
        v += center;

    return m;
}
