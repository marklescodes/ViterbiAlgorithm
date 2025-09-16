#include "Viterbi.h"

Viterbi::Viterbi(size_t convLength, const std::vector<int> &polynoms)
: convLength_(convLength), polynoms_(polynoms), numStates_(1u << (convLength -  1)) {}

std::vector<int> Viterbi::decode(const std::vector<int> &received) const {
    const size_t n = polynoms_.size();
    if (n == 0 || received.empty()) return {};
    const size_t T = received.size() / n;

    std::vector<std::vector<int>> pathMetric(T + 1, std::vector<int>(numStates_, infinite));
    std::vector<std::vector<int>> predecessor(T + 1, std::vector<int>(numStates_, -1));
    std::vector<std::vector<int>> inputBit(T + 1, std::vector<int>(numStates_, -1));

    pathMetric[0][0] = 0;

    for (size_t t = 0; t < T; ++t) {
        for (size_t state = 0; state < numStates_; ++state) {
            if (pathMetric[t][state] >= infinite) continue;

            for (int bit = 0; bit <= 1; ++bit) {
                const size_t nextState = (((unsigned)state << 1) | (unsigned)bit) & (numStates_ - 1);

                std::vector<int> shift(convLength_, 0);
                shift[0] = bit;
                for (std::size_t i = 1; i < convLength_; ++i)
                    shift[i] = (state >> (i-1)) & 1;

                int hamDist = 0;
                for (size_t gi = 0; gi < n; ++gi) {
                    int p = polynoms_[gi];
                    int outBit = 0;
                    for (size_t j = 0; j < convLength_; ++j) {
                        if ((p >> j) & 1) outBit ^= shift[j];
                    }
                    int receivedBit = received[t * n + gi];
                    if (outBit != receivedBit) ++hamDist;
                }

                int metric = pathMetric[t][state] + hamDist;
                if (metric < pathMetric[t + 1][nextState]) {
                    pathMetric[t + 1][nextState] = metric;
                    predecessor[t + 1][nextState] = (int)state;
                }
            }
        }
    }

    int bestState = 0; 
    
    std::vector<int> decode(T, 0);
    int currentState = bestState;
    for (int t = (int)T; t > 0; --t) {
        decode[t - 1] = inputBit[t][currentState];
        currentState = predecessor[t][currentState];
        if (currentState < 0) break;
    }

    if (convLength_ > 1 && decode.size() >= convLength_ - 1) {
        decode.resize(decode.size() - (convLength_ - 1));
    }

    return decode;
}