#include "ols.h"
#include "linsolve.h"

#include <stdexcept>

OLSResult OLSRegressor::fit(const Matrix& X, const Vector& y) const {
    if (X.rows() != y.size()) {
        throw std::invalid_argument("OLSRegressor::fit: X rows must match y size");
    }

    const size_t n = X.rows();
    const size_t d = X.cols();

    if (n == 0 || d == 0) {
        throw std::invalid_argument("OLSRegressor::fit: X must be non-empty");
    }

    if (n < d) {
        throw std::invalid_argument("OLSRegressor::fit: not enough observations for regression");
    }

    Matrix Xt = X.transpose();
    Matrix XtX = Xt.mul(X);
    Vector Xty = Xt.mul(y);

    Vector beta = solveLinearSystem(XtX, Xty);

    Vector yHat = X.mul(beta);

    double rss = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double residual = y[i] - yHat[i];
        rss += residual * residual;
    }

    double sigma2 = 0.0;
    if (n > d) {
        sigma2 = rss / static_cast<double>(n - d);
    } else {
        sigma2 = rss;
    }

    return {beta, sigma2};
}
