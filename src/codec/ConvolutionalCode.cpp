#include "ConvolutionalCode.h"

ConvolutionalCode::ConvolutionalCode(size_t convLength, const std::vector<int> &polynoms)
    : convLength_(convLength), polynoms_(polynoms) {}

std::vector<int> ConvolutionalCode::code(const std::vector<int> &inputBits, bool terminate) const {
    std::vector<int> output;
    output.reserve(inputBits.size() * polynoms_.size() +
                   (terminate ? (convLength_ - 1) * polynoms_.size() : 0));

    unsigned reg = 0;

    auto processBit = [&](int bit) {
        reg = ((reg << 1) | (unsigned)bit);
        for (int p : polynoms_) {
            int val = 0;
            unsigned mask = reg;
            for (size_t i = 0; i < convLength_; ++i) {
                if ((p >> i) & 1) val ^= (mask >> i) & 1u;
            }
            output.push_back(val);
        }
    };

    for (int b : inputBits) processBit(b);

    if (terminate && convLength_ > 1) {
        for (size_t t = 0; t < convLength_ - 1; ++t) processBit(0);
    }

    return output;
}
