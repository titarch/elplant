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

private:
    grid_t grid_;
};

using Mat3f = Matrix<double, 3>;

namespace RotMat3f {
    Mat3f U(double a) {
        return Mat3f(Mat3f::grid_t{
                Mat3f::row_t{cos(a), sin(a), 0},
                {-sin(a), cos(a), 0},
                {0, 0, 1}
        });
    }

    Mat3f L(double a) {
        return Mat3f(Mat3f::grid_t{
                Mat3f::row_t{cos(a), 0, -sin(a)},
                {0, 1, 0},
                {sin(a), 0, cos(a)}
        });
    }

    Mat3f H(double a) {
        return Mat3f(Mat3f::grid_t{
                Mat3f::row_t{1, 0, 0},
                {0, cos(a), -sin(a)},
                {0, sin(a), cos(a)}
        });
    }
}

#endif //ELPLANT_MATRIX_H
