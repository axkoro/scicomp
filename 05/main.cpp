#include <chrono>
#include <iostream>

#include "solvers/solvers.hpp"
#include "structures/CSRMatrix.hpp"
#include "structures/DenseVector.hpp"

void printCGvsGaussian(const CSRMatrix& A, const DenseVector& a) {
    const auto x = solvers::conjugateGradient(A, a);
    std::cout << "\n";

    std::cout << "CG result: " << x << "\n";

    const auto x_true = solvers::gaussianElimination(A, a);
    double error = (x - x_true).euclidean();

    std::cout << "Error: " << error << "\n";
}

// Times conjugateGradient
// @return time in milliseconds
double timeCG(const CSRMatrix& matrix, const DenseVector& vector) {
    auto start = std::chrono::high_resolution_clock::now();

    solvers::conjugateGradient(matrix, vector);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

void printStats(const std::vector<double>& times, const std::string& name) {
    double sum = 0;
    double min_time = times[0];
    double max_time = times[0];

    for (double time : times) {
        sum += time;
        min_time = std::min(min_time, time);
        max_time = std::max(max_time, time);
    }

    double avg = sum / times.size();

    std::cout << "System " << name << ":\n";
    std::cout << "  Average time: " << avg << " ms\n";
    std::cout << "  Min time: " << min_time << " ms\n";
    std::cout << "  Max time: " << max_time << " ms\n\n";
}

int main(int, char**) {
    const std::string systems_path = "../sparse-systems/";

    const CSRMatrix A(systems_path + "system-01", 100);
    const DenseVector a(systems_path + "system-01");

    const CSRMatrix B(systems_path + "system-02", 1000);
    const DenseVector b(systems_path + "system-02");

    const CSRMatrix C(systems_path + "system-03", 5000);
    const DenseVector c(systems_path + "system-03");

    const CSRMatrix D(systems_path + "system-04", 10);
    const DenseVector d(systems_path + "system-04");

    // Compare CG with True solution
    std::cout << "### A ###" << "\n";
    printCGvsGaussian(A, a);
    std::cout << "\n";

    std::cout << "### B ###" << "\n";
    printCGvsGaussian(B, b);
    std::cout << "\n";

    std::cout << "### C ###" << "\n";
    printCGvsGaussian(C, c);
    std::cout << "\n";

    // std::cout << "### D ###" << "\n";
    // printCGvsGaussian(D, d);
    // std::cout << "\n";

    // std::cout << D << "\n";
    // std::cout << solvers::gaussianElimination(D, d);

    // // Time
    // const int NUM_RUNS = 100;

    // std::vector<double> timesA, timesB, timesC;

    // for (int i = 0; i < NUM_RUNS; i++) {
    //     timesA.push_back(timeCG(A, a));
    // }

    // for (int i = 0; i < NUM_RUNS; i++) {
    //     timesB.push_back(timeCG(B, b));
    // }

    // for (int i = 0; i < NUM_RUNS; i++) {
    //     timesC.push_back(timeCG(C, c));
    // }

    // printStats(timesA, "A");
    // printStats(timesB, "B");
    // printStats(timesC, "C");

    return 0;
}
