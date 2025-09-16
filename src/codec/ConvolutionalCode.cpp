#include "ConvolutionalCode.h"

ConvolutionalCode::ConvolutionalCode(size_t convLength, const std::vector<int> &polynoms)
: convLength_(convLength), polynoms_(polynoms) {}

std::vector<int> ConvolutionalCode::code(const std::vector<int> &inputBits, bool terminate) const {
    std::vector<int> output;
    output.reserve(inputBits.size() * polynoms_.size() + (terminate ? (convLength_ - 1) * polynoms_.size() : 0));

    std::vector<int> shift(convLength_, 0);

    auto processBit = [&](int bit) {
        for (size_t i = convLength_ - 1; i > 0; --i)
            shift[i] = shift[i-1];
        shift[0] = bit;

        for (int p : polynoms_) {
            int val = 0;
            for (size_t i = 0; i < convLength_; ++i)
                if ((p >> i) & 1) val ^= shift[i];
            output.push_back(val);
        }
    };

    for (int b : inputBits) processBit(b);
    if (terminate && convLength_ > 1)
        for (size_t t = 0; t < convLength_ - 1; ++t) processBit(0);
    
    return output;
}