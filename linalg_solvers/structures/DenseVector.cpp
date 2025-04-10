#include "DenseVector.hpp"

#include <cassert>
#include <cmath>
#include <fstream>

DenseVector::DenseVector(size_t size, double default_value) : data(size, default_value) {}

DenseVector::DenseVector(std::initializer_list<double> init) : data(init) {}

DenseVector::DenseVector(std::string path) {
    path += "/b.bin";
    const size_t size = std::filesystem::file_size(path) / sizeof(double);
    data.resize(size);

    std::ifstream read_stream(path, std::ios::binary);
    read_stream.read(reinterpret_cast<char *>(&data[0]), size * sizeof(double));
    read_stream.close();
}

// Basic math operations
DenseVector &DenseVector::operator+=(const DenseVector &rhs) {
    assert(data.size() == rhs.size());
    for (size_t i = 0; i < size(); ++i) data[i] += rhs.data[i];
    return *this;
}

DenseVector &DenseVector::operator-=(const DenseVector &rhs) {
    assert(data.size() == rhs.size());
    for (size_t i = 0; i < size(); ++i) data[i] -= rhs.data[i];
    return *this;
}
DenseVector &DenseVector::operator*=(double rhs) {
    for (size_t i = 0; i < size(); ++i) data[i] *= rhs;
    return *this;
}
DenseVector &DenseVector::operator/=(double rhs) {
    for (size_t i = 0; i < size(); ++i) data[i] /= rhs;
    return *this;
}

// vector-vector inner product
double DenseVector::operator*(const DenseVector &rhs) const {
    assert(data.size() == rhs.size());
    double res = 0;
    for (size_t i = 0; i < size(); ++i) res += data[i] * rhs.data[i];
    return res;
}

// value access and modification: get value at index idx.
double &DenseVector::operator()(size_t idx) { return data.at(idx); }
double DenseVector::operator()(size_t idx) const { return data.at(idx); }

double DenseVector::euclidean() const {
    double res = 0;

    for (auto &&val : data) {
        res += val * val;
    }

    res = std::sqrt(res);

    return res;
}

double DenseVector::dot_product(const DenseVector &v) const {
    double res = 0;

    if (size() != v.size())
        throw std::runtime_error("Can't take dot product of vectors of different sizes");

    for (size_t i = 0; i < size(); i++) {
        res += data[i] * v(i);
    }

    return res;
}