#include "linsolve.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

Vector solveLinearSystem(Matrix A, Vector b) {
    const size_t n = A.rows();

    if (A.cols() != n) {
        throw std::invalid_argument("solveLinearSystem: matrix A must be square");
    }

    if (b.size() != n) {
        throw std::invalid_argument("solveLinearSystem: vector b size must match A");
    }

    for (size_t col = 0; col < n; ++col) {
        size_t pivotRow = col;
        double maxAbs = std::abs(A(col, col));

        for (size_t row = col + 1; row < n; ++row) {
            double currentAbs = std::abs(A(row, col));
            if (currentAbs > maxAbs) {
                maxAbs = currentAbs;
                pivotRow = row;
            }
        }

        if (maxAbs < 1e-12) {
            throw std::runtime_error("solveLinearSystem: singular or nearly singular matrix");
        }

        if (pivotRow != col) {
            for (size_t j = 0; j < n; ++j) {
                std::swap(A(col, j), A(pivotRow, j));
            }
            std::swap(b[col], b[pivotRow]);
        }

        for (size_t row = col + 1; row < n; ++row) {
            double factor = A(row, col) / A(col, col);
            A(row, col) = 0.0;

            for (size_t j = col + 1; j < n; ++j) {
                A(row, j) -= factor * A(col, j);
            }

            b[row] -= factor * b[col];
        }
    }
    
    Vector x(n, 0.0);

    for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
        double sum = b[static_cast<size_t>(i)];

        for (size_t j = static_cast<size_t>(i) + 1; j < n; ++j) {
            sum -= A(static_cast<size_t>(i), j) * x[j];
        }

        x[static_cast<size_t>(i)] =
            sum / A(static_cast<size_t>(i), static_cast<size_t>(i));
    }

    return x;
}
