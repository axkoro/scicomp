#include "SPA.hpp"
#include <iostream>

void SPA::accumulate(double val, size_t col) {
    if (!nonzeros[col]) { // not yet "occupied"
        nonzeros[col] = true;
        values[col] = val;
        updates.push_back(col);
    } else {
        values[col] += val;
    }
}

size_t SPA::output(std::vector<size_t>& JC, std::vector<double>& Num, size_t row_start_index) {
    size_t nnz = 0;
    for (auto& updated_col : updates) {
        JC.push_back(updated_col);
        Num.push_back(values[updated_col]);
        nnz++;
    }
    return nnz;
}