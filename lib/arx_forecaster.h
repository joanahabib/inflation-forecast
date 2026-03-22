#pragma once

#include "forecaster.h"
#include "feature_engineer.h"
#include "ols.h"

#include <string>
#include <vector>

class ARXForecaster : public Forecaster {
public:
    ARXForecaster(int p, const std::vector<std::string>& exogenousColumns);

    std::string name() const override;
    void fit(const Dataset& dataset) override;

    Forecast forecast(
        int horizon,
        const std::vector<std::string>& exogenousColumns,
        const std::vector<std::vector<double>>& futureExog
    ) const override;

    const Vector& coefficients() const;
    const std::vector<std::string>& featureNames() const;

private:
    int p_;
    std::vector<std::string> exogenousColumns_;
    Vector beta_;
    std::vector<std::string> featureNames_;
    std::vector<double> lastY_;
    bool fitted_;
};
