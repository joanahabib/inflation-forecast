#include "arx_forecaster.h"

#include <stdexcept>
#include <utility>

ARXForecaster::ARXForecaster(int p, const std::vector<std::string>& exogenousColumns)
    : p_(p),
      exogenousColumns_(exogenousColumns),
      beta_(),
      featureNames_(),
      lastY_(),
      fitted_(false) {
    if (p_ < 1) {
        throw std::invalid_argument("ARXForecaster: p must be >= 1");
    }
}

std::string ARXForecaster::name() const {
    return "ARX(" + std::to_string(p_) + ")";
}

void ARXForecaster::fit(const Dataset& dataset) {
    FeatureEngineer featureEngineer;
    SupervisedData supervised = featureEngineer.buildARX(dataset, p_, exogenousColumns_);

    OLSRegressor ols;
    OLSResult result = ols.fit(supervised.X, supervised.y);

    beta_ = result.beta;
    featureNames_ = supervised.featureNames;

    const std::vector<double>& ySeries = dataset.getColumn("inflation_yoy");
    if (ySeries.size() < static_cast<std::size_t>(p_)) {
        throw std::runtime_error("ARXForecaster::fit: not enough history for lag order");
    }

    lastY_.assign(ySeries.end() - p_, ySeries.end());
    fitted_ = true;
}

Forecast ARXForecaster::forecast(
    int horizon,
    const std::vector<std::string>& exogenousColumns,
    const std::vector<std::vector<double>>& futureExog
) const {
    if (!fitted_) {
        throw std::runtime_error("ARXForecaster::forecast: model has not been fitted");
    }

    if (horizon < 1) {
        throw std::invalid_argument("ARXForecaster::forecast: horizon must be >= 1");
    }

    if (exogenousColumns != exogenousColumns_) {
        throw std::runtime_error("ARXForecaster::forecast: exogenous column list does not match fitted model");
    }

    if (futureExog.size() != exogenousColumns_.size()) {
        throw std::runtime_error("ARXForecaster::forecast: futureExog size mismatch");
    }

    for (std::size_t j = 0; j < futureExog.size(); ++j) {
        if (futureExog[j].size() != static_cast<std::size_t>(horizon)) {
            throw std::runtime_error("ARXForecaster::forecast: each exogenous future path must have length = horizon");
        }
    }

    std::vector<double> history = lastY_;
    std::vector<double> out;
    out.reserve(static_cast<std::size_t>(horizon));

    for (int step = 0; step < horizon; ++step) {
        Vector x(1 + static_cast<std::size_t>(p_) + exogenousColumns_.size(), 0.0);

        x[0] = 1.0;

        for (int lag = 1; lag <= p_; ++lag) {
            x[static_cast<std::size_t>(lag)] =
                history[history.size() - static_cast<std::size_t>(lag)];
        }

        for (std::size_t j = 0; j < exogenousColumns_.size(); ++j) {
            x[1 + static_cast<std::size_t>(p_) + j] = futureExog[j][static_cast<std::size_t>(step)];
        }

        double yPred = dot(beta_, x);
        out.push_back(yPred);

        history.push_back(yPred);
        if (history.size() > static_cast<std::size_t>(p_)) {
            history.erase(history.begin());
        }
    }

    Forecast result;
    result.yhat = out;
    return result;
}

const Vector& ARXForecaster::coefficients() const {
    return beta_;
}

const std::vector<std::string>& ARXForecaster::featureNames() const {
    return featureNames_;
}
