//
// Created by parsy_b on 3/5/20.
//

#include <array>
#include <cmath>
#include "Vector.h"

#ifndef ELPLANT_MATRIX_H
#define ELPLANT_MATRIX_H

template<typename T, size_t N>
class Matrix {
public:
    using row_t = std::array<T, N>;
    using grid_t = std::array<row_t, N>;

    Matrix(grid_t const& grid) : grid_(grid) {}

    Matrix t() {
        Matrix t{{}};
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                t.grid_[i][j] = grid_[j][i];
        return t;
    }

    friend Vector<T, N> operator*(Matrix const& m, Vector<T, N> const& v) {
        Vector<T, N> vp;
        for (size_t i = 0; i < N; ++i)
            vp[i] = Vector<T, N>{m.grid_[i]} * v;
        return vp;
    }

    friend Matrix operator*(Matrix const& lhs, Matrix const& rhs) {
        Matrix p{{}}, t_rhs = rhs.t();
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                p[i][j] = Vector<T, N>{lhs.grid_[i]} * Vector<T, N>{t_rhs.grid_[j]};
        return p;
    }

    static Matrix R(Vector<T, N> const& u, double angle) {
        throw "Not implemented";
    }

private:
    grid_t grid_;
};

using Mat3f = Matrix<double, 3>;

template<>
inline Mat3f Mat3f::R(Vec3f const& u, double angle) {
    double c = cos(angle);
    double s = sin(angle);

    return Mat3f{Mat3f::grid_t{
            Mat3f::row_t{u[0] * u[0] * (1 - c) + c, u[0] * u[1] * (1 - c) - u[2] * s, u[0] * u[2] * (1 - c) + u[1] * s},
            {u[0] * u[1] * (1 - c) + u[2] * s, u[1] * u[1] * (1 - c) + c, u[1] * u[2] * (1 - c) - u[0] * s},
            {u[0] * u[2] * (1 - c) - u[1] * s, u[1] * u[2] * (1 - c) + u[0] * s, u[2] * u[2] * (1 - c) + c}
    }};
}

#endif //ELPLANT_MATRIX_H
