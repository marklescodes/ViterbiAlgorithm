#include "Viterbi.h"
#include <limits>

Viterbi::Viterbi(size_t convLength, const std::vector<int> &polynoms)
    : convLength_(convLength), polynoms_(polynoms) {}

std::vector<int> Viterbi::decode(const std::vector<int> &received, size_t originalLength) const {
    size_t nStates = 1 << (convLength_ - 1);
    size_t nOutputs = polynoms_.size();
    size_t nSteps = received.size() / nOutputs;

    std::vector<std::vector<double>> metrics(nSteps + 1, std::vector<double>(nStates, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<int>> prevState(nSteps + 1, std::vector<int>(nStates, -1));
    std::vector<std::vector<int>> prevBit(nSteps + 1, std::vector<int>(nStates, -1));

    metrics[0][0] = 0.0;

    for (size_t t = 0; t < nSteps; t++) {
        for (size_t state = 0; state < nStates; state++) {
            if (metrics[t][state] == std::numeric_limits<double>::infinity()) continue;

            for (int bit = 0; bit <= 1; bit++) {
                unsigned reg = ((state << 1) | bit);
                std::vector<int> out;
                for (int p : polynoms_) {
                    int val = 0;
                    unsigned mask = reg;
                    for (size_t i = 0; i < convLength_; i++) {
                        if ((p >> i) & 1) val ^= (mask >> i) & 1u;
                    }
                    out.push_back(val);
                }

                size_t nextState = reg & ((1 << (convLength_ - 1)) - 1);
                double dist = 0.0;
                for (size_t j = 0; j < nOutputs; j++) {
                    if (out[j] != received[t * nOutputs + j]) dist += 1.0;
                }

                double newMetric = metrics[t][state] + dist;
                if (newMetric < metrics[t + 1][nextState]) {
                    metrics[t + 1][nextState] = newMetric;
                    prevState[t + 1][nextState] = state;
                    prevBit[t + 1][nextState] = bit;
                }
            }
        }
    }

    size_t bestState = 0;
    for (size_t s = 0; s < nStates; s++) {
        if (metrics[nSteps][s] < metrics[nSteps][bestState]) bestState = s;
    }

    std::vector<int> decoded(nSteps);
    for (size_t t = nSteps; t > 0; t--) {
        decoded[t - 1] = prevBit[t][bestState];
        bestState = prevState[t][bestState];
    }

    if (decoded.size() > originalLength) {
        decoded.resize(originalLength);
    }

    return decoded;
}
