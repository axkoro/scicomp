#include "CSRMatrix.hpp"

CSRMatrix::CSRMatrix(std::initializer_list<std::initializer_list<double>> init)
    : n_rows(init.size()),
    n_cols(init.begin()->size()),
    IR(n_rows + 1)
{
    size_t row_idx = 0;
    size_t val_cnt = 0;

    for (auto &&row : init)
    {   
        if (row.size() != n_cols)
        {
            throw std::runtime_error("Row size mismatch in initializer list");
        }

        IR[row_idx] = val_cnt;

        size_t col_idx = 0;
        for (auto &&val : row)
        {
            if (val != 0)
            {
                Num.push_back(val);
                JC.push_back(col_idx);
                val_cnt++;
            }
            
            col_idx++;
        }
        row_idx++;
    }

    IR[n_rows] = val_cnt;
}

CSRMatrix::CSRMatrix(size_t rows, size_t cols, std::vector<Triplet> triplet_init)
    : n_rows(rows),
    n_cols(cols),
    IR(n_rows + 1)
{
    // count number of values in every row
    std::vector<size_t> row_cnt(n_rows, 0.0);
    for (auto &&triplet : triplet_init)
    {
        if (triplet.value != 0)
        {
            row_cnt[triplet.row] += 1;
        }
    }

    // generate IR from row counts
    IR[0] = 0.0;
    for (size_t i = 0; i < n_rows; i++)
    {
        IR[i+1] = IR[i] + row_cnt[i];
    }

    size_t n_values = IR[n_rows];
    JC.resize(n_values);
    Num.resize(n_values);

    // populate JC and Num
    for (auto &&triplet : triplet_init)
    {
        if (triplet.value == 0) continue;

        size_t row = triplet.row;
        size_t idx = IR[row + 1] - row_cnt[row]; // row start + offset

        row_cnt[row]--;

        JC[idx] = triplet.col;
        Num[idx] = triplet.value;
    }
}

DenseVector CSRMatrix::operator*(const DenseVector &rhs) const
{
    if (n_cols != rhs.size())
    {
        throw std::runtime_error("Matrix-vector size mismatch");
    }

    DenseVector res(n_rows, 0);
    for (size_t i = 0; i < n_rows; i++)
    {
        for (size_t j = IR[i]; j < IR[i+1]; j++)
        {
            res(i) += rhs(JC[j]) * Num[j];
        }
    }

    return res;
}

double CSRMatrix::operator()(size_t row, size_t col) const
{
    if (row >= n_rows || col >= n_cols)
    {
        throw std::out_of_range("Index out of bounds");
    }

    for (size_t i = IR[row]; i < IR[row+1]; i++)
    {
        if (JC[i] == col)
        {
            return Num[i];
        }
    }
    return 0.0;
}
