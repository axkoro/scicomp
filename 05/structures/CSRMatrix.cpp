#include "CSRMatrix.hpp"

#include <filesystem>
#include <fstream>

CSRMatrix::CSRMatrix(std::initializer_list<std::initializer_list<double>> init)
    : n_rows(init.size()), n_cols(init.begin()->size()) {
    for (const auto& row : init) {
        if (n_cols != row.size()) throw(std::domain_error("input is not a matrix!"));
    }

    IR.push_back(0);
    for (auto row_data : init) {
        for (size_t col = 0; col < cols(); ++col) {
            double aij = row_data.begin()[col];
            if (aij != 0) {
                Num.push_back(aij);
                JC.push_back(col);
            }
        }
        IR.push_back(Num.size());
    }
}

CSRMatrix::CSRMatrix(size_t rows, size_t cols, std::vector<Triplet> triplet_init)
    : n_rows(rows),
      n_cols(cols),
      Num(triplet_init.size()),
      JC(triplet_init.size()),
      IR(n_rows + 1) {
    // count number of elements for each row
    for (auto& trip : triplet_init) ++IR[trip.row];

    // compute prefix sums. IR now contains correct data
    for (size_t i = 0, prefix_sum = 0; i < n_rows; ++i) {
        auto nnz_ith_row = IR[i];
        IR[i] = prefix_sum;
        prefix_sum += nnz_ith_row;
    }
    IR.back() = triplet_init.size();  // last entry of IR

    // compute col indices for each entry
    for (auto& trip : triplet_init) {
        auto row_start = IR[trip.row];

        JC[row_start] = trip.col;
        Num[row_start] = trip.value;

        // just for now: increase the start index of the current row so we know
        // where to insert the next value.
        // we will fix IR after this loop.
        ++IR[trip.row];
    }

    // fix IR
    for (size_t i = 0, first_idx_of_row = 0; i <= n_rows; ++i) {
        size_t next_row = IR[i];
        IR[i] = first_idx_of_row;
        first_idx_of_row = next_row;
    }
}

DenseVector CSRMatrix::operator*(const DenseVector& rhs) const {
    if (cols() != rhs.size()) throw std::logic_error("matrix and vector dimensions do not match!");

    DenseVector result(rows(), 0);

    for (size_t i = 0; i < rows(); ++i) {
        for (size_t d = IR[i]; d < IR[i + 1]; ++d) {
            size_t j = JC[d];
            double aij = Num[d];
            result(i) += aij * rhs(j);
        }
    }

    return result;
}

double CSRMatrix::operator()(size_t row, size_t col) const {
    // search row for correct col
    for (size_t d = IR[row]; d < IR[row + 1]; ++d)
        if (JC[d] == col) return Num[d];
    return 0.;
}

template <class Type>
size_t length_of_file(const std::string& path) {
    size_t length = std::filesystem::file_size(path) / sizeof(Type);
    return length;
}

// Reads a binary file that stores a sequence of values with type 'Type' and
// returns them as a std::vector<Type>
template <class Type>
std::vector<Type> read_vec(const std::string& path) {
    const size_t size = length_of_file<Type>(path);
    std::vector<Type> to_read(size);

    std::ifstream read_stream(path, std::ios::binary);
    read_stream.read(reinterpret_cast<char*>(&to_read[0]), size * sizeof(Type));
    read_stream.close();

    return to_read;
}

std::vector<Triplet> read_to_triplets(const std::string& path) {
    auto rows_data = read_vec<uint64_t>(path + "/rows.bin");
    auto cols_data = read_vec<uint64_t>(path + "/cols.bin");
    auto vals_data = read_vec<double>(path + "/vals.bin");

    std::vector<Triplet> triplets;
    triplets.reserve(rows_data.size());

    auto row_it = rows_data.begin();
    auto col_it = cols_data.begin();
    auto val_it = vals_data.begin();
    for (; row_it != rows_data.end(); ++row_it, ++col_it, ++val_it) {
        triplets.push_back({*row_it, *col_it, *val_it});
    }
    return triplets;
}

CSRMatrix::CSRMatrix(std::string path, size_t n) : CSRMatrix(n, n, read_to_triplets(path)) {}