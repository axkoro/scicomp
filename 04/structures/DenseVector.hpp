#ifndef DENSE_VECTOR_HPP
#define DENSE_VECTOR_HPP

#include <iostream>
#include <vector>

class DenseVector {
  std::vector<double> data;

public:
  // constructors

  /// @brief creates a DenseVector of size @a size where each entry has value @a
  /// default_value
  /// @param size size of the DenseVector
  /// @param default_value value of all entries in this DenseVector
  DenseVector(size_t size, double default_value);

  /// @brief creates a DenseVector and initializes it with the values in @a init
  /// @param init values of the DenseVector
  DenseVector(std::initializer_list<double> init);

  // Basic math operations - these should modify the DenseVector `this` and
  // then return a reference to `this`.
  // further reading: "Binary Arithmetic Operators"
  // https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading/4421719#4421719
  DenseVector &operator+=(const DenseVector &rhs);
  DenseVector &operator-=(const DenseVector &rhs);
  DenseVector &operator*=(double rhs);
  DenseVector &operator/=(double rhs);

  // vector-vector inner product
  double operator*(const DenseVector &rhs) const;

  // value access and modification: get value at index idx.
  // access by reference - this allows modifications: Vector(1) = 2;
  double &operator()(size_t idx);
  // const access - does not allow modifications
  double operator()(size_t idx) const;

  // size of vector
  inline size_t size() const { return data.size(); }
};

/////// derived functions (already implemented)

// output vector
inline std::ostream &operator<<(std::ostream &os, const DenseVector &vec) {
  os << "(";
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i != 0)
      os << ", ";
    os << vec(i);
  }
  os << ")^T";
  return os;
}

// derived operators

inline DenseVector operator+(const DenseVector &lhs, const DenseVector &rhs) {
  DenseVector result = lhs;
  result += rhs;
  return result;
}

inline DenseVector operator-(const DenseVector &lhs, const DenseVector &rhs) {
  DenseVector result = lhs;
  result -= rhs;
  return result;
}

inline DenseVector operator*(const DenseVector &lhs, double rhs) {
  DenseVector result = lhs;
  result *= rhs;
  return result;
}

inline DenseVector operator*(double lhs, const DenseVector &rhs) {
  return rhs * lhs;
}

inline DenseVector operator/(const DenseVector &lhs, double rhs) {
  DenseVector result = lhs;
  result /= rhs;
  return result;
}

#endif
