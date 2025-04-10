#ifndef SPA_HPP
#define SPA_HPP

#include <vector>
#include <list>

/**
 * @brief Sparse Accumulator (SPA) class for SpGEMM operations.
 *
 * This class implements a Sparse Accumulator which is used to efficiently accumulate
 * non-zero elements during Sparse General Matrix-Matrix Multiplication.
 */
class SPA {
private:
    size_t size;  ///< Size of the accumulator
    std::vector<double> values;  ///< Accumulated values
    std::vector<bool> nonzeros;  ///< Flags indicating non-zero positions
    std::list<size_t> updates;  ///< List of indices that have been updated

public:
    /**
     * @brief Constructor for SPA.
     * @param size The size of the accumulator (number of columns in the (rhs or resulting) matrix).
     */
    SPA(size_t size) : size(size), values(size), nonzeros(size), updates() {}

    /**
     * @brief Accumulates a value at a specific column index.
     * @param val The value to accumulate.
     * @param col The column index where the value should be accumulated.
     */
    void accumulate(double val, size_t col);

    /**
     * @brief Outputs the accumulated non-zero values and their column indices.
     * @param JC Reference to a vector to store the column indices.
     * @param Num Reference to a vector to store the non-zero values.
     * @param row_start_index The starting index for the row in the output matrix.
     * @return The number of non-zero entries outputted.
     */
    size_t output(std::vector<size_t>& JC, std::vector<double>& Num, size_t row_start_index);
};

#endif