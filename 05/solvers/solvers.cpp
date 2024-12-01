#include "solvers.hpp"

#include <cmath>
#include <vector>

namespace solvers {
DenseVector conjugateGradient(const CSRMatrix& A, const DenseVector& b, double tolerance) {
    DenseVector x(b.size(), 0);
    DenseVector r = b - A * x;  // in-place probably possible

    DenseVector p = r;
    double alpha = std::pow(r.euclidean(), 2);

    int t = 0;
    while (alpha != 0) {
        DenseVector v = A * p;
        double lambda = alpha / (v.dot_product(p));

        for (size_t i = 0; i < x.size(); i++) {  // x = x + lambda * p (in-place)
            x(i) += lambda * p(i);
        }

        for (size_t i = 0; i < r.size(); i++) {  // r = r - lambda * v (in-place)
            r(i) -= lambda * v(i);
        }

        double r_norm = r.euclidean();
        std::cout << "t=" << t++ << ": " << r_norm << "\n";
        if (r_norm < tolerance) break;

        double alpha_new = std::pow(r.euclidean(), 2);
        double alpha_change = alpha_new / alpha;

        for (size_t i = 0; i < p.size(); i++) {  // p = r + (alpha_new / alpha) * p (in-place)
            p(i) *= alpha_change;
            p(i) += r(i);
        };

        alpha = alpha_new;
    }

    return x;
}

// Only for testing purposes
DenseVector gaussianElimination(const CSRMatrix& matrix, const DenseVector& vector) {
    int n = matrix.cols();

    // Create augmented matrix [A|b] using provided access methods
    std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug[i][j] = matrix(i, j);
        }
        aug[i][n] = vector(i);
    }

    // Forward elimination
    for (int k = 0; k < n - 1; k++) {
        // Find pivot
        int maxRow = k;
        double maxVal = abs(aug[k][k]);
        for (int i = k + 1; i < n; i++) {
            if (abs(aug[i][k]) > maxVal) {
                maxVal = abs(aug[i][k]);
                maxRow = i;
            }
        }

        // Swap rows if necessary
        if (maxRow != k) {
            for (int j = k; j <= n; j++) {
                std::swap(aug[k][j], aug[maxRow][j]);
            }
        }

        // Check for singular matrix
        if (abs(aug[k][k]) < 1e-10) {
            throw std::runtime_error("Matrix is singular or nearly singular");
        }

        // Eliminate column k
        for (int i = k + 1; i < n; i++) {
            double factor = aug[i][k] / aug[k][k];
            for (int j = k; j <= n; j++) {
                aug[i][j] -= factor * aug[k][j];
            }
        }
    }

    // Back substitution
    DenseVector solution(n, 0);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += aug[i][j] * solution(j);
        }
        solution(i) = (aug[i][n] - sum) / aug[i][i];
    }

    return solution;
}

}  // namespace solvers