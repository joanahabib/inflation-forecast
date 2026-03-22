#include "matrix.h"

Matrix Matrix::identity(size_t n) {
    Matrix I(n, n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        I(i, i) = 1.0;
    }
    return I;
}

Matrix Matrix::transpose() const {
    Matrix t(c_, r_, 0.0);
    for (size_t i = 0; i < r_; ++i) {
        for (size_t j = 0; j < c_; ++j) {
            t(j, i) = (*this)(i, j);
        }
    }
    return t;
}

Vector Matrix::mul(const Vector& x) const {
    if (c_ != x.size()) {
        throw std::invalid_argument("Matrix::mul(Vector): dimension mismatch");
    }

    Vector y(r_, 0.0);
    for (size_t i = 0; i < r_; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < c_; ++j) {
            sum += (*this)(i, j) * x[j];
        }
        y[i] = sum;
    }
    return y;
}

Matrix Matrix::mul(const Matrix& b) const {
    if (c_ != b.rows()) {
        throw std::invalid_argument("Matrix::mul(Matrix): dimension mismatch");
    }

    Matrix out(r_, b.cols(), 0.0);
    for (size_t i = 0; i < r_; ++i) {
        for (size_t k = 0; k < c_; ++k) {
            const double aik = (*this)(i, k);
            for (size_t j = 0; j < b.cols(); ++j) {
                out(i, j) += aik * b(k, j);
            }
        }
    }
    return out;
}

double dot(const Vector& a, const Vector& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("dot: size mismatch");
    }

    double s = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        s += a[i] * b[i];
    }
    return s;
}

double l2norm(const Vector& v) {
    return std::sqrt(dot(v, v));
}
