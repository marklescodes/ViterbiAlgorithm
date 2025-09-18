#include "ConvolutionalCode.h"

ConvolutionalCode::ConvolutionalCode(size_t convLength, const std::vector<int> &polynoms)
    : convLength_(convLength), polynoms_(polynoms) {}

std::vector<int> ConvolutionalCode::code(const std::vector<int> &inputBits) const {
    std::vector<int> output;
    output.reserve((inputBits.size() + convLength_-1) * polynoms_.size());

    unsigned reg = 0;
    const unsigned mask = (convLength_ >= 32) ? 0xFFFFFFFFu : ((1u << convLength_) - 1u);

    for (int bit : inputBits) {
        reg = ((reg << 1u) | static_cast<unsigned>(bit)) & mask;
        for (int p : polynoms_) {
            int val = 0;
            for (size_t i = 0; i < convLength_; ++i)
                if ((p >> i) & 1) val ^= (reg >> i) & 1;
            output.push_back(val);
        }
    }

    for (size_t k = 0; k < convLength_ - 1; ++k) {
        reg = (reg << 1u) & mask;
        for (int p : polynoms_) {
            int val = 0;
            for (size_t i = 0; i < convLength_; ++i)
                if ((p >> i) & 1) val ^= (reg >> i) & 1;
            output.push_back(val);
        }
    }
    
    return output;
}
