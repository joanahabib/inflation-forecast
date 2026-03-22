#pragma once

#include "matrix.h"

struct OLSResult {
    Vector beta;
    double sigma2;
};

class OLSRegressor {
public:
    OLSResult fit(const Matrix& X, const Vector& y) const;
};
