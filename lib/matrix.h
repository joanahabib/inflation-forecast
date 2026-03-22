#pragma once

#include <vector>
#include <stdexcept>
#include <cstddef>
#include <cmath>

class Vector {
public:
    Vector() = default;
    explicit Vector(size_t n, double v = 0.0) : data_(n, v) {}

    size_t size() const {
        return data_.size();
    }

    double& operator[](size_t i) {
        return data_.at(i);
    }

    double operator[](size_t i) const {
        return data_.at(i);
    }

private:
    std::vector<double> data_;
};

class Matrix {
public:
    Matrix() : r_(0), c_(0) {}
    Matrix(size_t rows, size_t cols, double v = 0.0)
        : r_(rows), c_(cols), a_(rows * cols, v) {
    }

    size_t rows() const {
        return r_;
    }

    size_t cols() const {
        return c_;
    }

    double& operator()(size_t i, size_t j) {
        return a_.at(i * c_ + j);
    }

    double operator()(size_t i, size_t j) const {
        return a_.at(i * c_ + j);
    }

    static Matrix identity(size_t n);

    Matrix transpose() const;
    Vector mul(const Vector& x) const;
    Matrix mul(const Matrix& b) const;

private:
    size_t r_, c_;
    std::vector<double> a_;
};

double dot(const Vector& a, const Vector& b);
double l2norm(const Vector& v);
