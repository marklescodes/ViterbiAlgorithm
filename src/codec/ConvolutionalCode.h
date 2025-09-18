#ifndef CONVOLUTIONAL_CODE_H
#define CONVOLUTIONAL_CODE_H

#include <vector>
#include <cstddef>

class ConvolutionalCode {
public:
    ConvolutionalCode(size_t convLength, const std::vector<int> &polynoms);
    std::vector<int> code(const std::vector<int> &inputBits) const;

private:
    size_t convLength_;
    std::vector<int> polynoms_;
};

#endif