#ifndef CODING_H
#define CODING_H

#include <limits> 
#include <random> 
#include <vector>
#include <cstddef>

constexpr int infinite = std::numeric_limits<int>::max() / 4;

class ConvolutionalCode {
public:
    ConvolutionalCode(std::size_t convLength, const std::vector<int>& polynoms)
    : convLength_(convLength), polynoms_(polynoms) {}
    std::vector<int> code(const std:: vector<int> &inputBits, bool terminate = true) const {
        std::vector<int> output;
        output.reserve(inputBits.size() * polynoms_.size() + (terminate ? (convLength_ - 1) * polynoms_.size() : 0));

        std::vector<int> shift(convLength_, 0);

        auto processBit = [&](int bit) {
            for (std::size_t i = convLength_ - 1; i > 0; --i) {
                shift[i] = shift[i - 1];
            }
            shift[0] = bit;

            for (int p : polynoms_) {
                int val = 0;
                for (std::size_t i = 0; i < convLength_; i++) {
                    if ((p >> i) & 1) val ^= shift[i];
                }
                output.push_back(val);
            }  
        };

        for (int b : inputBits) processBit(b);

        if (terminate && convLength_ > 1) {
            for (std::size_t t = 0; t < convLength_ - 1; ++t) processBit(0);
        }

        return output;
    }

    std::size_t convLength() const { return convLength_; }
    std::size_t outputBitsPerInput() const { return polynoms_.size(); }

private:
    std::size_t convLength_;
    std::vector<int> polynoms_;
    
};

class Viterbi{
public:
    Viterbi(std::size_t convLength, const std::vector<int>& polynoms)
    : convLength_(convLength), polynoms_(polynoms), numStates_(1u << (convLength_ - 1)) {}

    std::vector<int> decode(const std::vector<int> &received) const {
        const std::size_t n = polynoms_.size();
        if (n == 0) return {};
        if (received.empty()) return {};
        const std::size_t T = received.size() / n;
        
        std::vector<std::vector<int>> pathMetric(T+1, std::vector<int>(numStates_, infinite));
        std::vector<std::vector<int>> predecessor(T+1, std::vector<int>(numStates_, -1));
        std::vector<std::vector<int>> inputBit (T+1, std::vector<int>(numStates_, -1));

        pathMetric[0][0] = 0;

        for (std::size_t t = 0; t < T; ++t) {
            for (std::size_t state = 0; state < numStates_; ++state) {
                if (pathMetric[t][state] >= infinite) continue;

                for (int bit = 0; bit <= 1; ++bit) {
                    const std::size_t nextState = (((unsigned)state << 1) | (unsigned)bit) & (numStates_ - 1);

                    std::vector<int> shift(convLength_, 0);
                    for (std::size_t i = 0; i < convLength_ - 1; ++i) shift[i] = (state >> i) & 1;
                    shift[convLength_ - 1] = bit;

                    int hamdist = 0;
                    for (std::size_t gi = 0; gi < n; ++gi) {
                        int p = polynoms_[gi];
                        int outBit = 0;
                        for (std::size_t i = 0; i < convLength_; ++i) {
                            if ((p >> i) & 1) outBit ^= shift[i];
                        }

                        const int receivedBit = received[t * n + gi];
                        if (outBit != receivedBit) ++hamdist;
                    }

                    int metric = pathMetric[t][state] + hamdist;
                    if (metric < pathMetric[t+1][nextState]) {
                        pathMetric[t+1][nextState] = metric;
                        predecessor[t+1][nextState] = int(state);
                        inputBit[t+1][nextState] = bit;
                    }
                }
            }
        }

        int bestState = 0;
        int bestMetric = pathMetric[T][0];
        for (std::size_t s = 1; s < numStates_; ++s) {
            if (pathMetric[T][s] < bestMetric) {
                bestMetric = pathMetric[T][s];
                bestState = (int)s;
            }
        }

        std::vector<int> decode(T,0);
        int currentState = bestState;
        for (int t = (int)T; t > 0; --t) {
            decode[t-1] = inputBit[t][currentState];
            currentState = predecessor[t][currentState];
            if (currentState < 0) break;
        }

        if (convLength_ > 1 && decode.size() >= convLength_ - 1) {
            decode.resize(decode.size() - (convLength_ - 1) * polynoms_.size());
        }

        return decode;

    }

private:
    std::size_t convLength_;
    std::vector<int> polynoms_;
    std::size_t numStates_;
};

inline std::vector<int> BSC(const std::vector<int> &bits, double pError, std::mt19937& p) {
    std::vector<int> noise(bits.size());
    std::bernoulli_distribution flip (pError);
    for (std::size_t i = 0; i < bits.size(); i++) {
        noise[i] = bits[i];
        if (flip(p)) noise[i] ^= 1;
    }
    return noise;
}

#endif