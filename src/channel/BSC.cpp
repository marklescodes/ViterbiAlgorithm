#include "BSC.h"

std::vector<int> BSC(const std::vector<int> &input, double p, std::mt19937 &rng) {
    std::bernoulli_distribution dist(p);
    std::vector<int> output = input;
    for (auto &b : output) {
        if (dist(rng)) b ^= 1;
    }
    return output;
}
