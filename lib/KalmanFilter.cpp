#include "KalmanFilter.h"
#include <stdexcept>
#include <cmath>

KalmanFilter::KalmanFilter(int stateDim, int measDim)
    : stateDim(stateDim), measDim(measDim){}

void KalmanFilter::setMatrices(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& H,
    const std::vector<std::vector<double>>& Q,
    const std::vector<std::vector<double>>& R) {
    
    this->A = A;
    this->H = H;
    this->Q = Q;
    this->R = R;
}

void KalmanFilter::initialize(
    const std::vector<double>& x0,
    const std::vector<std::vector<double>>& P0) {
    x = x0;
    P = P0;
}

