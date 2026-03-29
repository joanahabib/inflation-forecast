#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "dataset.h"
#include "arx_forecaster.h"
#include "KalmanFilter.h"

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: influx_app <path_to_csv>\n";
            return 1;
        }

        Dataset dataset = Dataset::loadCSV(argv[1]);

        int p = 2;
        std::vector<std::string> exogenousColumns = {
            "oil_change",
            "fx_change",
            "policy_rate"
        };

        ARXForecaster model(p, exogenousColumns);
        model.fit(dataset);

        std::cout << "Model: " << model.name() << "\n";
        std::cout << "Estimated coefficients:\n";

        const Vector& beta = model.coefficients();
        const std::vector<std::string>& featureNames = model.featureNames();

        for (std::size_t i = 0; i < beta.size(); ++i) {
            std::cout << "  "
                      << std::setw(14)
                      << featureNames[i]
                      << " = "
                      << beta[i]
                      << "\n";
        }

        int horizon = 6;
        std::vector<std::vector<double>> futureExog(exogenousColumns.size());

        for (std::size_t j = 0; j < exogenousColumns.size(); ++j) {
            const std::vector<double>& column = dataset.getColumn(exogenousColumns[j]);
            double lastValue = column.back();
            futureExog[j].assign(static_cast<std::size_t>(horizon), lastValue);
        }

        Forecast fc = model.forecast(horizon, exogenousColumns, futureExog);

        std::cout << "\nForecast for next " << horizon << " periods:\n";
        for (int i = 0; i < horizon; ++i) {
            std::cout << "  t+" << (i + 1) << ": "
                      << fc.yhat[static_cast<std::size_t>(i)] << "\n";
        }

        std::cout <<"\nApplying Kalman Filter...\n";
        int stateDim = 1;
        int measDim = 1;

        KalmanFilter kf(stateDim, measDim);

        std::vector<std::vector<double>> A = {{1}};
        std::vector<std::vector<double>> H = {{1}};
        std::vector<std::vector<double>> Q = {{0.01}};
        std::vector<std::vector<double>> R = {{0.1}};

        kf.setMatrices(A, H, Q, R);

        std::vector<double> x0 = {fc.yhat[0]};
        std::vector<std::vector<double>> P0 = {{1}};
        kf.initialize(x0, P0);

        std::vector<double> smoothedForecast;

        for (int i = 0; i < horizon; ++i) {
            kf.predict();
            kf.update({fc.yhat[static_cast<std::size_t>(i)>]});

            double estimate = kf.getState()[0];
            smoothedForecast.push_back(estimate);
        }
        std::cout << "\nSmoothed Forecast:\n";
        for (int i = 0; i < horizon; ++i) {
            std::cout << " t+" << (i + 1) << ": " << smoothedForecast[static_cast<std::size_t>(i)] << "\n";
        
        std::cout << "\nDone.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
}
