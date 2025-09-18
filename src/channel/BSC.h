#ifndef BSC_H
#define BSC_H

#include <vector>
#include <random>

inline std::vector<int> BSC(const std::vector<int> &bits, double pError, std::mt19937& rng) {
    std::vector<int> noisy(bits.size());
    std::bernoulli_distribution flip(pError);
    for (size_t i = 0; i < bits.size(); ++i) {
        noisy[i] = bits[i] ^ static_cast<int>(flip(rng));
    }
    return noisy;
}

#endif
