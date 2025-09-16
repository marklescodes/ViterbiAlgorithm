#ifndef VITERBI_H
#define VITERBI_H

#include <vector>
#include <cstddef>
#include <limits>

constexpr int infinite = std::numeric_limits<int>::max() / 4;

class Viterbi {
public:
    Viterbi(size_t convLength, const std::vector<int> &polynoms);

    std::vector<int> decode(const std::vector<int> &received) const;
    
private:
    size_t convLength_;
    std::vector<int> polynoms_;
    size_t numStates_;
};

#endif