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

std::vector<std::vector<double>> KalmanFilter::multiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {

    int n = A.size(), m = B[0].size(), p = B.size();
    std::vector<std::vector<double>> result(n, std::vector<double>(m, 0));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            for (int k = 0; k < p; k++)
                result[i][j] += A[i][k] * B[k][j];
    return result;
}
std::vector<double> KalmanFilter::multiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<double>& x) {

    int n = A.size, m = x.size
    std::vector<double> result(n, 0);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            result[i] += A[i][j] * x[j];
    return result;
}

std::vector<double> KalmanFilter::multiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<double>& x) {

    int n = A.size(), m = x.size();
    std::vector<double> result(n, 0);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            result[i] += A[i][j] * x[j];
    return result;
}

std::vector<std::vector<double>> KalmanFilter::transpose(
    const std::vector<std::vector<double>>& A) {

    int n = A.size(), m = A[0].size();
    std::vector<std::vector<double>> T(m, std::vector<double>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T[j][i] = A[i][j];
    return T;
}

std::vector<std::vector<double>> KalmanFilter::add(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {

    int n = A.size(), m = A[0].size();
    std::vector<std::vector<double>> result(n, std::vector<double>(m));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            result[i][j] = A[i][j] + B[i][j];
    return result;
}

std::vector<std::vector<double>> KalmanFilter::subtract(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {

    int n = A.size(), m = A[0].size();
    std::vector<std::vector<double>> result(n, std::vector<double>(m));

    for (int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            result[i][j] = A[i][j] - B[i][j];
    return result;
}

std::vector<std::vector<double>> KalmanFilter::identity(int size) {
    std::vector<std::vector<double>> I(size, std::vector<double>(size, 0));
    for (int i = 0; i < size; i++)
        I[i][i] = 1;
    return I;
}

std::vector<std::vector<double>> KalmanFilter::identity(int size) {
    std::vector<std::vector<double>> I(size, std::vector<double>(size, 0));
    for (int i = 0; i < size; i++)
        I[i][i] = 1;
    return I;
}
std::vector<std::vector<double>> KalmanFilter::inverse(
    const std::vector<std::vector<double>>& matrix) {\

    int n = matrix.size();
    auto A = matrix;
    auto I = identity(n);

        for (int i = 0; i < n; i++) {
            double diag = A[i][i];
            if (fabs(diag) < 1e-9)
                throw std::runtime_error("Matrix not invertible");
            
        for (int i = 0; i <n; i++)
            A[i][j] /= diag;
            I[i][j] /= diag;
        }
    
        for (int k = 0; k < n; k++) {
            if (k == i) continue;
            double factor = A[k][i];
            for (int j = 0; j < n; j++)
                A[k][j] -= factor * A[i][j];
                I[k][j] -= factor * I[i][j];
            }
        }
    }
    return I;
}

    





