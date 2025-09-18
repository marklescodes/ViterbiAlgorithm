#include "Viterbi.h"
#include <algorithm>
#include <cassert>

Viterbi::Viterbi(size_t convLength, const std::vector<int> &polynoms)
    : convLength_(convLength),
      polynoms_(polynoms),
      numStates_(1u << (convLength - 1)) {}

std::vector<int> Viterbi::decode(const std::vector<int> &received, size_t originalLength) const {
    const size_t n = polynoms_.size();
    assert(n > 0);
    assert(convLength_ >= 1);
    assert(received.size() % n == 0);

    const size_t T = received.size() / n;
    const unsigned mask = (1u << convLength_) - 1u;

    std::vector<std::vector<int>> metric(T + 1, std::vector<int>(numStates_, INF));
    std::vector<std::vector<int>> prev(T + 1, std::vector<int>(numStates_, -1));
    std::vector<std::vector<int>> bitChoice(T + 1, std::vector<int>(numStates_, 0));

    metric[0][0] = 0; 

    for (size_t t = 0; t < T; ++t) {
        for (size_t s = 0; s < numStates_; ++s) {
            const int m = metric[t][s];
            if (m >= INF) continue;

            for (int b = 0; b <= 1; ++b) {
                unsigned reg = ((static_cast<unsigned>(s) << 1) | static_cast<unsigned>(b)) & mask;

                int dist = 0;
                for (size_t g = 0; g < n; ++g) {
                    int val = 0;
                    int p = polynoms_[g];
                    for (size_t i = 0; i < convLength_; ++i) {
                        if ((p >> i) & 1) val ^= (reg >> i) & 1;
                    }
                    if (val != received[t * n + g]) ++dist;
                }

                const size_t nextState = reg >> 1; 
                const int newMetric = m + dist;
                if (newMetric < metric[t + 1][nextState]) {
                    metric[t + 1][nextState] = newMetric;
                    prev[t + 1][nextState] = static_cast<int>(s);
                    bitChoice[t + 1][nextState] = b;
                }
            }
        }
    }

    int bestState = 0;
    int bestMetric = metric[T][0];
    for (size_t s = 1; s < numStates_; ++s) {
        if (metric[T][s] < bestMetric) {
            bestMetric = metric[T][s];
            bestState = static_cast<int>(s);
        }
    }

    std::vector<int> decoded(T, 0);
    int state = bestState;
    for (int t = static_cast<int>(T); t > 0; --t) {
        decoded[static_cast<size_t>(t - 1)] = bitChoice[static_cast<size_t>(t)][static_cast<size_t>(state)];
        state = prev[static_cast<size_t>(t)][static_cast<size_t>(state)];
        if (state < 0) break;
    }


    if (decoded.size() > originalLength) decoded.resize(originalLength);
    return decoded;
}
