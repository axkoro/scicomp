#ifndef SPA_HPP
#define SPA_HPP

#include <vector>
#include <list>

class SPA {
private:
    size_t size;
    std::vector<double> values;
    std::vector<bool> nonzeros;
    std::list<size_t> updates;

public:
    SPA(size_t size) : size(size), values(size), nonzeros(size), updates() {}

    // Might save alloc operations but otherwise unecessary:
    // void reset();

    void accumulate(double val, size_t col);

    size_t output(std::vector<size_t>& JC, std::vector<double>& Num, size_t row_start_index);
};

#endif