#ifndef ELPLANT_VECTOR_H
#define ELPLANT_VECTOR_H


#include <cmath>
#include <ostream>
#include <array>
#include <execution>
#include "yaml-cpp/yaml.h"

template<typename T, size_t D>
class Vector {
public:
    using Ts = std::array<T, D>;
    using iterator = typename Ts::iterator;
    using const_iterator = typename Ts::const_iterator;

    Vector() : pts_{} {}

    Vector(Ts const& pts) : pts_(pts) {}

    [[nodiscard]] T sqrMagnitude() const {
        return std::inner_product(this->cbegin(), this->cend(), this->cbegin(), 0.0);
    }

    [[nodiscard]] T magnitude() const {
        return std::sqrt(sqrMagnitude());
    }

    [[nodiscard]] Vector normalized() const {
        Vector copy(*this);
        copy /= magnitude();
        return copy;
    }

    T& operator[](size_t idx) {
        return pts_[idx];
    }

    T operator[](size_t idx) const {
        return pts_[idx];
    }

    iterator begin() { return pts_.begin(); }

    iterator end() { return pts_.end(); }

    const_iterator begin() const { return pts_.begin(); }

    const_iterator end() const { return pts_.end(); }

    const_iterator cbegin() const { return pts_.cbegin(); }

    const_iterator cend() const { return pts_.cend(); }

    Vector& operator+=(Vector const& rhs) {
        std::transform(begin(), end(), rhs.cbegin(), begin(), std::plus<T>());
        return *this;
    }

    Vector operator-() const {
        Vector copy;
        std::transform(cbegin(), cend(), copy.begin(), std::negate<T>());
        return copy;
    }

    Vector& operator-=(Vector const& rhs) {
        std::transform(begin(), end(), rhs.cbegin(), begin(), std::minus<T>());
        return *this;
    }

    Vector& operator*=(T k) {
        std::transform(begin(), end(), pts_.begin(), [&k](T const& e) { return e * k; });
        return *this;
    }

    Vector& operator/=(T k) {
        std::transform(pts_.begin(), pts_.end(), pts_.begin(), [&k](T const& e) { return e / k; });
        return *this;
    }

    Vector& operator^=(Vector const& rhs) {
        (void) rhs;
        throw "Cross product not implemented for dimension " + std::to_string(D);
        return *this;
    }


    double get_angle_U() const;
    double get_angle_L() const;
    double get_angle_H() const;

    friend std::ostream& operator<<(std::ostream& os, Vector const& v) {
        os << "( ";
        for (const T& e: v)
            os << e << ' ';
        return os << ")";
    }

    friend YAML::Emitter& operator<<(YAML::Emitter& out, Vector const& v) {
        out << YAML::Flow << YAML::BeginSeq;
        for (const T& e: v)
            out << e;
        return out << YAML::EndSeq;
    }

private:
    Ts pts_;
};


template<typename T, size_t D>
inline Vector<T, D> operator+(Vector<T, D> lhs, Vector<T, D> const& rhs) {
    lhs += rhs;
    return lhs;
}

template<typename T, size_t D>
inline Vector<T, D> operator-(Vector<T, D> lhs, Vector<T, D> const& rhs) {
    lhs -= rhs;
    return lhs;
}

template<typename T, size_t D>
inline T operator*(Vector<T, D> const& lhs, Vector<T, D> const& rhs) {
    return std::inner_product(lhs.cbegin(), lhs.cend(), rhs.cbegin(), 0.0);
}

template<typename T, size_t D>
inline Vector<T, D> operator*(Vector<T, D> lhs, T rhs) {
    lhs *= rhs;
    return lhs;
}

template<typename T, size_t D>
inline Vector<T, D> operator*(T lhs, Vector<T, D> rhs) {
    rhs *= lhs;
    return rhs;
}

template<typename T, size_t D>
inline Vector<T, D> operator/(Vector<T, D> lhs, T rhs) {
    lhs /= rhs;
    return lhs;
}

template<typename T, size_t D>
inline Vector<T, D> operator^(Vector<T, D> lhs, Vector<T, D> const& rhs) {
    lhs ^= rhs;
    return lhs;
}

using Vec2f = Vector<double, 2>;
using Vec3f = Vector<double, 3>;

namespace UnitVec3f {
    const auto U = Vec3f{{0, 1, 0}};
    const auto L = Vec3f{{1, 0, 0}};
    const auto H = Vec3f{{0, 0, 1}};
}

template<> inline Vec3f& Vec3f::operator^=(const Vec3f& rhs) {
    float x = pts_[1] * rhs.pts_[2] - pts_[2] * rhs.pts_[1];
    float y = pts_[2] * rhs.pts_[0] - pts_[0] * rhs.pts_[2];
    float z = pts_[0] * rhs.pts_[1] - pts_[1] * rhs.pts_[0];
    pts_[0] = x;
    pts_[1] = y;
    pts_[2] = z;
    return *this;
}

template<> inline double Vec3f::get_angle_U() const {
    Vec3f norm = normalized();
    double angle = acos(norm * UnitVec3f::U);
    return angle;
}

template<> inline double Vec3f::get_angle_L() const {
    Vec3f norm = normalized();
    double angle = acos(norm * UnitVec3f::L);
    return angle;
}

template<> inline double Vec3f::get_angle_H() const {
    Vec3f norm = normalized();
    double angle = acos(norm * UnitVec3f::H);
    return angle;
}


namespace YAML {
    template<>
    struct convert<Vec3f> {
        static bool decode(const Node& node, Vec3f& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs[0] = node[0].as<double>();
            rhs[1] = node[1].as<double>();
            rhs[2] = node[2].as<double>();
            return true;
        }
    };
}

#endif //ELPLANT_VECTOR_H
