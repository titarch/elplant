#ifndef ELPLANT_VECTOR_H
#define ELPLANT_VECTOR_H


#include <cmath>
#include <ostream>
#include <array>
#include <execution>

template<typename T, size_t D = 2>
class Vector {
public:
    using Ts = std::array<T, D>;
    using iterator = typename Ts::iterator;
    using const_iterator = typename Ts::const_iterator;

    Vector() : pts_{} {}

    explicit Vector(Ts const& pts) : pts_(pts) {}

    [[nodiscard]] T sqrMagnitude() const {
        return std::reduce(std::execution::par, pts_.cbegin(), pts_.cend(),
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
        // Implement cross product
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, Vector const& v) {
        os << "( ";
        for (const T& e: v)
            os << e << ' ';
        return os << ")";
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
    return std::inner_product(lhs.cbegin(), lhs.cend(), rhs.cbegin(), 0);
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

#endif //RAYTRACER_VECTOR_H