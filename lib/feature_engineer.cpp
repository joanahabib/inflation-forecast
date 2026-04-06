#include "feature_engineer.h"

#include <stdexcept>
#include <string>
#include <vector>


SupervisedData FeatureEngineer::buildARX(
    const Dataset& dataset,
    int p,
    const std::vector<std::string>& exogenousColumns
) const {
    if (p < 1) {
        throw std::invalid_argument("FeatureEngineer::buildARX: p must be >= 1");
    }

    const std::vector<double>& ySeries = dataset.getColumn("inflation_yoy");
    const std::size_t n = dataset.size();

    if (n <= static_cast<std::size_t>(p)) {
        throw std::runtime_error("FeatureEngineer::buildARX: dataset too small for chosen lag order");
    }

    for (const std::string& col : exogenousColumns) {
        if (dataset.hasColumn(col) && dataset.getColumn(col).size() == n) {
            validExog.push_back(col);
        }
    }

    const std::size_t rows = n - static_cast<std::size_t>(p);
    const std::size_t cols = 1 + static_cast<std::size_t>(p) + exogenousColumns.size();

    Matrix X(rows, cols, 0.0);
    Vector y(rows, 0.0);

    std::vector<std::string> featureNames;
    featureNames.reserve(cols);

    featureNames.push_back("intercept");

    for (int lag = 1; lag <= p; ++lag) {
        featureNames.push_back("lag_y_" + std::to_string(lag));
    }

    for (const std::string& col : exogenousColumns) {
        featureNames.push_back(col);
    }

    
    for (std::size_t r = 0; r < rows; ++r) {
        const std::size_t t = static_cast<std::size_t>(p) + r;
        X(r, 0) = 1.0;
        for (int lag = 1; lag <= p; ++lag) {
            X(r, static_cast<std::size_t>(lag)) =
                ySeries[t - static_cast<std::size_t>(lag)];
        }

        for (std::size_t j = 0; j < exogenousColumns.size(); ++j) {
            const std::vector<double>& exogSeries = dataset.getColumn(exogenousColumns[j]);
            X(r, 1 + static_cast<std::size_t>(p) + j) = exogSeries[t];
        }

        y[r] = ySeries[t];
    }

    return {X, y, featureNames};
}
