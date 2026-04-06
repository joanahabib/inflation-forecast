#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "json.hpp"
#include "dataset.h"
#include "arx_forecaster.h"
#include "KalmanFilter.h"

using json = nlohmann::json;

void saveResults(
    const std::vector<double>& forecast,
    const std::vector<double>& smoothed,
    const std::vector<std::string>& featureNames,
    const Vector& beta
) {
    json j;
    j["forecast"] = forecast;
    j["smoothed"] = smoothed;

    json coeffs;
    for (std::size_t i = 0; i < beta.size(); ++i) {
        coeffs[featureNames[i]] = beta[i];
    }

    j["coefficients"] = coeffs;

    std::ofstream file("output.json");
    file << j.dump(4);
}

int main(int argc, char** argv) {
    try {
        if (argc < 3) return 1;
            std::string path = argv[1];
            std::string iso = argv[2];

        Dataset dataset = Dataset::loadCSV(path, iso);

        if (dataset.size() == 0) return 1;
        
        int p = 2;
        std::vector<std::string> exogenousColumns = {
            "oil_change",
            "fx_change",
            "policy_rate"
        };

        ARXForecaster model(p, exogenousColumns);
        model.fit(dataset);

        const Vector& beta = model.coefficients();
        const std::vector<std::string>& featureNames = model.featureNames();

        int horizon = 6;
        std::vector<std::vector<double>> futureExog(exogenousColumns.size());

        for (std::size_t j = 0; j < exogenousColumns.size(); ++j) {
            const std::vector<double>& column = dataset.getColumn(exogenousColumns[j]);
            double lastValue = column.back();
            futureExog[j].assign(horizon, lastValue);
        }
        else { 
            futureExog[j].assign(horizon, 0.0);
        }

        Forecast fc = model.forecast(horizon, exogenousColumns, futureExog);

        KalmanFilter kf(1, 1);

        kf.setMatrices({{1}}, {{1}}, {{0.01}}, {{0.1}});
        kf.initialize({fc.yhat[0]}, {{1}});

        std::vector<double> smoothedForecast;

        for (int i = 0; i < horizon; ++i) {
            kf.predict();
            kf.update({fc.yhat[i]});
            smoothedForecast.push_back(kf.getState()[0]);
        }

        json j;
        j["forecast"] = fc.yhat;
        j["smoothed"] = smoothed;

        json coeffs;
        for (std::size_t i = 0; i < beta.size(); ++i) {
            coeffs[featureNames[i]] = beta[i];
        }

        j["coefficients"] = coeffs;
        std::cout << j.dump() << std::endl;

        return 0;
            
        } catch (...) {
            return 1;
        }
    }
