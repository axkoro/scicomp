#include <iostream>

#include "solvers/solvers.hpp"
#include "structures/CSRMatrix.hpp"
#include "structures/DenseVector.hpp"

int main(int, char**) {
  const std::string systems_path = "./sparse-systems/";

  const CSRMatrix A(systems_path + "system-01/", 100);
  const DenseVector a(systems_path + "system-01/");

  const CSRMatrix B(systems_path + "system-02/", 1000);
  const DenseVector b(systems_path + "system-02/");

  const CSRMatrix C(systems_path + "system-03/", 5000);
  const DenseVector c(systems_path + "system-03/");

  const CSRMatrix D(systems_path + "system-04/", 10);
  const DenseVector d(systems_path + "system-04/");

  const auto x = solvers::conjugateGradient(A, a);

  std::cout << "CG result: " << x << "\n";

  return 0;
}
