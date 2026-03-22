#pragma once

#include "dataset.h"

#include <string>
#include <vector>

struct Forecast {
    std::vector<double> yhat;
};

class Forecaster {
public:
    virtual ~Forecaster() = default;

    virtual std::string name() const = 0;
    virtual void fit(const Dataset& dataset) = 0;

    virtual Forecast forecast(
        int horizon,
        const std::vector<std::string>& exogenousColumns,
        const std::vector<std::vector<double>>& futureExog
    ) const = 0;
};
