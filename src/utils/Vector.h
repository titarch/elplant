#ifndef ELPLANT_VECTOR_H
#define ELPLANT_VECTOR_H


#include <cmath>
#include <ostream>
#include <array>
#include <execution>

template<typename T, size_t D>
class Vector {
public:
    using Ts = std::array<T, D>;
    using iterator = typename Ts::iterator;
    using const_iterator = typename Ts::const_iterator;

    Vector() : pts_{} {}

    Vector(Ts const& pts) : pts_(pts) {}

    [[nodiscard]] T sqrMagnitude() const {
        return std::reduce(std::execution::par, pts_.cbegin(), pts_.cend(), 0.0,
                           [](T const& x, T const& y) { return x * x + y * y; });
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

template<> inline Vector<double, 3>& Vector<double, 3>::operator^=(const Vector<double, 3>& rhs) {
    float x = pts_[1] * rhs.pts_[2] - pts_[2] * rhs.pts_[1];
    float y = pts_[2] * rhs.pts_[0] - pts_[0] * rhs.pts_[2];
    float z = pts_[0] * rhs.pts_[1] - pts_[1] * rhs.pts_[0];
    pts_[0] = x;
    pts_[1] = y;
    pts_[2] = z;
    return *this;
}

#endif //ELPLANT_VECTOR_H
