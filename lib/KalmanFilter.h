#indef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include <vector>

class KalmanFilter {
private:
    int stateDim;
    int measDim;
    
    std::vector<std::vector<double>> A;
    std::vector<std::vector<double>> H;
    std::vector<std::vector<double>> Q;
    std::vector<std::vector<double>> R;
    std::vector<std::vector<double>> P
    std::vector<double> x;
    
public:
    KalmanFilter(int stateDim, int measDim);
    void setMatrices(const std::vector<std::vector<double>>& A,
                     const std::vector<std::vector<double>>& H,
                     const std::vector<std::vector<double>>& Q,
                     const std::vector<std::vector<double>>& R);
    
    void initialize(const std::vector<double>>& x0,
                    const std::vector<std::vector<double>>& P0);
    
    void predict();
    void update(const std::vector<double>& z);
    std::vector<double> getState() const;
    
private:
    std::vector<std::vector<double>> multiply(const std::vector<std::vector<double>>& A,
                                              const std::vector<std::vector<double>>& B);
    
    std::vector<double> multiply(const std::vector<std::vector<double>>& A,
                                 const std::vector<double>& x);
    
    std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>>& A);
    
    std::vector<std::vector<double>> add(const std::vector<std::vector<double>>& A,
                                         const std::vector<std::vector<double>>& B);
    
    std::vector<std::vector<double>> subtract(const std::vector<std::vector<double>>& A,
                                              const std::vector<std::vector<double>>& B);
    
    std::vector<std::vector<double>> identity(int size);
    
    std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>>& matrix);
};

#endif
