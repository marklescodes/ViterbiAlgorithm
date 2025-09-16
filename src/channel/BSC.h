#ifndef BSC_H
#define BSC_H

#include <vector>
#include <random>

inline std::vector<int> BSC(const std::vector<int> &bits, double pError, std::mt19937& rng) {
    std::vector<int> noise(bits.size());
    std::bernoulli_distribution flip (pError);
    for (size_t i = 0; i < bits.size(); ++i) {
        noise[i] = bits[i];
        if (flip(rng)) noise[i] ^= 1;
    }
    return noise;
}

#endif