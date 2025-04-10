#ifndef CSRMATRIX_HPP
#define CSRMATRIX_HPP

#include <vector>

#include "DenseVector.hpp"

/// @brief used to initialize a CSRMatrix. A triplet describes a value in a
/// matrix at index (row, col).
struct Triplet {
  size_t row;
  size_t col;
  double value;
};

class CSRMatrix {
private:
  const size_t n_rows, n_cols;
  std::vector<size_t> IR;
  std::vector<size_t> JC;
  std::vector<double> Num;

public:
  // Blatt 4: SpGEMM
  /// @brief computes the matrix-matrix product using SpGEMM
  CSRMatrix operator*(const CSRMatrix &rhs) const {
    return CSRMatrix{0, 0, {}, {}, {}};
  };

  /// @brief creates a CSRMatrix from three vectors IR, JC, Num
  CSRMatrix(size_t rows, size_t cols, std::vector<size_t> IR,
            std::vector<size_t> JC, std::vector<double> Num)
      : n_rows(rows), n_cols(cols), IR(IR), JC(JC), Num(Num) {}

  auto operator<=>(const CSRMatrix &) const = default;

  /////// Blatt 3

  // Aufgabe 1
  // constructors

  /// @brief creates a CSRMatrix from a 2d list @a init.
  /// @param init contains values for each matrix entry, where the value at row
  /// i and column j is init[i][j]
  CSRMatrix(std::initializer_list<std::initializer_list<double>> init);

  /// @brief creates a CSRMatrix from a vector of Triplets, each containing a
  /// value with its row and column index, and creates a matrix of size @a rows
  /// and @a columns
  /// @param rows number of rows of the CSRMatrix
  /// @param cols number of columns of the CSRMatrix
  /// @param triplet_init values of the matrix with their indices
  CSRMatrix(size_t rows, size_t cols, std::vector<Triplet> triplet_init);

  // Blatt 5
  // Read CSCMatrix from files. Creates a square matrix of size system_size.
  CSRMatrix(std::string path, size_t system_size);

  /// @brief computes the matrix-vector product using SpMV
  DenseVector operator*(const DenseVector &rhs) const;

  /// @brief computes v^T * A * w
  double bilinear(const DenseVector &vT, const DenseVector &w) const {
    return vT * (*this * w);
  }

  // value access
  double operator()(size_t row, size_t col) const;

  // size of matrix
  size_t rows() const { return n_rows; }
  size_t cols() const { return n_cols; }
};

/////// derived functions (already implemented)

// output matrix
inline std::ostream &operator<<(std::ostream &os, const CSRMatrix &mat) {
  for (size_t i = 0; i < mat.rows(); ++i) {
    if (i != 0)
      os << "\n";
    os << "(";
    for (size_t j = 0; j < mat.cols(); ++j) {
      if (j != 0)
        os << ", ";
      os << mat(i, j);
    }
    os << ")";
  }
  return os;
}

#endif