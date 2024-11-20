#include <iostream>

#include "structures/CSRMatrix.hpp"
#include "structures/DenseVector.hpp"

int main(int, char**) {
  CSRMatrix A(4, 4, { 0, 2, 3, 4, 5 }, { 0, 3, 1, 0, 2 }, { 1, 1, 1, 1, 1 });
  CSRMatrix B(4, 4, { 0, 2, 4, 6, 8 }, { 0, 1, 0, 1, 2, 3, 2, 3 },
    { 2, -3, 4, 5, 1, -4, 9, -8 });

  // SpGEMM
  CSRMatrix C = A * B;

  // print matrices
  std::cout << "A:\n" << A << "\n";
  std::cout << "B:\n" << B << "\n";
  std::cout << "C:\n" << C << "\n";

  // /*
  // A:
  // (1, 0, 0, 1)
  // (0, 1, 0, 0)
  // (1, 0, 0, 0)
  // (0, 0, 1, 0)
  // B:
  // (2, -3, 0, 0)
  // (4, 5, 0, 0)
  // (0, 0, 1, -4)
  // (0, 0, 9, -8)
  // C:
  // (2, -3, 9, -8)
  // (4, 5, 0, 0)
  // (2, -3, 0, 0)
  // (0, 0, 1, -4)
  // */

  CSRMatrix expected(4, 4, { 0, 4, 6, 8, 10 }, { 0, 1, 2, 3, 0, 1, 0, 1, 2, 3 },
    { 2, -3, 9, -8, 4, 5, 2, -3, 1, -4 });
  if (C != expected) {
    std::cout << "error: result are not equal!";
    return 1;
  }
  return 0;
}
