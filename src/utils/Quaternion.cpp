//
// Created by mattrouss on 3/11/20.
//

#include "Quaternion.h"

Vec3f Quaternion::rotate(Vec3f const &v) {
    Quaternion pure(0, v);

    Quaternion inv = inverse();
    Quaternion rotated = *this * pure * inv;

    return rotated.v_;

}

double Quaternion::sqrMagnitude() const {
    return w_ * w_ + v_ * v_;
}

double Quaternion::magnitude() const {
    return sqrt(sqrMagnitude());
}

Quaternion& Quaternion::normalize() {
    double m = magnitude();
    w_ /= m;
    v_ /= m;

    return *this;
}

void Quaternion::convertUnitNorm() {
    v_ = v_.normalized() * (double)sinf(w_ * 0.5f);
    w_ = cosh(w_ * 0.5);
}

Quaternion &Quaternion::operator*=(Quaternion const &rhs) {
    double s = w_ * rhs.w_ - (v_ * rhs.v_);
    Vec3f v = w_ * rhs.v_ + rhs.w_ * v_ + (v_ ^ rhs.v_);

    w_ = s;
    v_ = v;
    return *this;
}

Quaternion &Quaternion::operator*=(double k) {
    w_ *= k;
    v_ *= k;
    return *this;
}

Quaternion Quaternion::conjugate() const {
    return Quaternion(w_, -v_);
}

Quaternion Quaternion::inverse() const {
    double n = 1 / sqrMagnitude();
    Quaternion conj = conjugate();
    return conj * n;
}


Quaternion& operator*(Quaternion& lhs, Quaternion const& rhs) {
    lhs *= rhs;
    return lhs;
}

Quaternion& operator*(Quaternion& lhs, double rhs) {
    lhs *= rhs;
    return lhs;
}


namespace RotQuat3f {
    Quaternion U(double a) {
        return Quaternion(cosf(a * 0.5f), UnitVec3f::U * (double)sinf(a * 0.5f));
    }

    Quaternion L(double a) {
        return Quaternion(cosf(a * 0.5f), UnitVec3f::L * (double)sinf(a * 0.5f));
    }

    Quaternion H(double a) {
        return Quaternion(cosf(a * 0.5f), UnitVec3f::H * (double)sinf(a * 0.5f));
    }
}