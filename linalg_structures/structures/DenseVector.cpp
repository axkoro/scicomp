#include "DenseVector.hpp"

DenseVector::DenseVector(size_t size, double default_value) : data(size, default_value) {}

DenseVector::DenseVector(std::initializer_list<double> init) : data(init) {}

DenseVector& DenseVector::operator+=(const DenseVector& rhs)
{
    size_t this_size = size();
    if (rhs.size() != this_size)
    {
        throw std::runtime_error("SizeMismatchException");
    }

    for (size_t i = 0; i < this_size; i++)
    {
        data[i] += rhs(i);
    }

    return *this;
}

DenseVector& DenseVector::operator-=(const DenseVector& rhs)
{
    size_t this_size = size();
    if (rhs.size() != this_size)
    {
        throw std::runtime_error("SizeMismatchException");
    }

    for (size_t i = 0; i < this_size; i++)
    {
        data[i] -= rhs(i);
    }

    return *this;
}

DenseVector& DenseVector::operator*=(double rhs)
{
    for (auto &val : data)
    {
        val *= rhs;
    }
    
    return *this;
}

DenseVector& DenseVector::operator/=(double rhs)
{
    for (auto &val : data)
    {
        val /= rhs;
    }

    return *this;
}

double DenseVector::operator*(const DenseVector& rhs) const
{
    size_t this_size = size();
    if (rhs.size() != this_size)
    {
        throw std::runtime_error("SizeMismatchException");
    }

    double result = 0;
    for (size_t i = 0; i < this_size; i++)
    {
        result += data[i] * rhs(i);
    }

    return result;
}

double& DenseVector::operator()(size_t idx)
{
    return data[idx];
}

double DenseVector::operator()(size_t idx) const
{
    return data[idx];
}