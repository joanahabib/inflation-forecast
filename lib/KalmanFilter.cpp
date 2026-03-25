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

void KalmanFilter::predict() {
    x = multiply(A, x);
    P = add(multiply(A, P), transpose(A)), Q);
}

void KalmanFilter::update(const std::vector<double>& z) {
    auto Ht = transpose(H);
    auto S = add(multiply(multiply(H, P), Ht), R);
    auto K = multiply(multiply(P, Ht), inverse(S));

    auto y = z;
    auto Hx = multiply(H, x);
    for (size_t i = 0; i < y.size(), i++) {
        y[i] -= Hx[i];
    }
    auto K_y = multiply(K,y);
    for (size_t i = 0; i < x.size(), i++) {
        x[i] = K_y[i];
    }
    auto I = identity(stateDim);
    P = multiply(subtract(I, multiply(K, H)), P);
}

std::vector<double> KalmanFilter::getState() const {
    return x;
}



