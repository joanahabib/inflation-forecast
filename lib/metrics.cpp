#include "metrics.h"

#include <cmath>
#include <stdexcept>

double rmse(const std::vector<double>& yTrue, const std::vector<double>& yPred) {
    if (yTrue.size() != yPred.size()) {
        throw std::invalid_argument("rmse: size mismatch");
    }

    if (yTrue.empty()) {
        throw std::invalid_argument("rmse: input vectors must not be empty");
    }

    double sumSq = 0.0;
    for (std::size_t i = 0; i < yTrue.size(); ++i) {
        double e = yTrue[i] - yPred[i];
        sumSq += e * e;
    }

    return std::sqrt(sumSq / static_cast<double>(yTrue.size()));
}

double mae(const std::vector<double>& yTrue, const std::vector<double>& yPred) {
    if (yTrue.size() != yPred.size()) {
        throw std::invalid_argument("mae: size mismatch");
    }

    if (yTrue.empty()) {
        throw std::invalid_argument("mae: input vectors must not be empty");
    }

    double sumAbs = 0.0;
    for (std::size_t i = 0; i < yTrue.size(); ++i) {
        sumAbs += std::abs(yTrue[i] - yPred[i]);
    }

    return sumAbs / static_cast<double>(yTrue.size());
}
