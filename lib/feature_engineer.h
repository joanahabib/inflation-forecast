#pragma once

#include "dataset.h"
#include "matrix.h"

#include <string>
#include <vector>

struct SupervisedData {
    Matrix X;
    Vector y;
    std::vector<std::string> featureNames;
};

class FeatureEngineer {
public:
    SupervisedData buildARX(
        const Dataset& dataset,
        int p,
        const std::vector<std::string>& exogenousColumns
    ) const;
};
