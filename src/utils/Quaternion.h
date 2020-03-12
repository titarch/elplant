//
// Created by mattrouss on 3/11/20.
//

#pragma once

#include "Vector.h"

class Quaternion {
public:
    Quaternion(double w, Vec3f const& v): w_(w), v_(v) {}

    [[nodiscard]] Vec3f rotate(Vec3f const& v);
    [[nodiscard]] double sqrMagnitude() const;
    [[nodiscard]] double magnitude() const;
    Quaternion& normalize();
    void convertUnitNorm();

    [[nodiscard]] Quaternion conjugate() const;
    Quaternion inverse() const;

    Quaternion& operator*=(Quaternion const& rhs);
    Quaternion& operator*=(double k);

private:
    double w_;
    Vec3f v_;
};

inline Quaternion& operator*(Quaternion& lhs, Quaternion const& rhs);
inline Quaternion& operator*(Quaternion& lhs, double rhs);


namespace RotQuat3f {
    Quaternion U(double a);
    Quaternion L(double a);
    Quaternion H(double a);
}


