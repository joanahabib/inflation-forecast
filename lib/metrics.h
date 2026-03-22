#pragma once

#include <vector>

double rmse(const std::vector<double>& yTrue, const std::vector<double>& yPred);
double mae(const std::vector<double>& yTrue, const std::vector<double>& yPred);
