#include "CSRMatrix.hpp"
#include "SPA.hpp"

CSRMatrix CSRMatrix::operator*(const CSRMatrix& rhs) const {
    std::vector<size_t> res_IR(n_rows + 1);
    std::vector<size_t> res_JC;
    std::vector<double> res_NUM;

    for (size_t i = 0; i < n_rows; i++) { // rows of A
        SPA spa(rhs.n_cols);

        size_t A_row_start = IR[i];
        size_t A_next_row = IR[i + 1];
        for (size_t k = A_row_start; k < A_next_row; k++) { // values in row of A
            double A_val = Num[k];
            size_t A_col = JC[k];

            size_t B_row_start = rhs.IR[A_col];
            size_t B_next_row = rhs.IR[A_col + 1];
            for (size_t j = B_row_start; j < B_next_row; j++) { // values in respective row of B
                double B_val = rhs.Num[j];
                size_t B_col = rhs.JC[j];

                double val = A_val * B_val;
                spa.accumulate(val, B_col);
            }
        }

        size_t newnz = spa.output(res_JC, res_NUM, res_IR[i]);
        res_IR[i + 1] = res_IR[i] + newnz;
    }

    CSRMatrix result_matrix(n_rows, rhs.n_cols, res_IR, res_JC, res_NUM);
    return result_matrix;
}