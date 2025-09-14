#include <iostream> 
#include <random> 
#include <vector>
#include <chrono>
#include <iomanip>
#include "coding.h"

int main() {

    const std::size_t convLength = 3;
    const std::vector<int> polynoms = {7, 5};
    const std::size_t N = 1000;
    const double start = 0.0, end = 0.2, step = 0.02;
    const unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();

    std::mt19937 rng(seed);

    ConvolutionalCode code(convLength, polynoms);
    Viterbi decode (convLength, polynoms);

    std::vector<int> message(N);
    std::uniform_int_distribution<int> bitDist(0,1);
    for (std::size_t i = 0; i < N; ++i) message[i] = bitDist(rng);

    auto zeroCoded = code.code(message, true);
    auto zeroDecode = decode.decode(zeroCoded);
    if (zeroDecode != message) {
        std::cerr << "Check FAILED.\n";
        std::cerr << "message size = " << message.size() << ", coded size = " <<zeroCoded.size() << ", decoded size = " << zeroDecode.size() << "\n";
        return 2;
    }   

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Seed: " << seed << "\n";

    for (double errorProbability = start; errorProbability <= end + 1e-12; errorProbability += step) {
        auto coded = code.code(message, true);
        auto noise = BSC(coded, errorProbability, rng);
        auto decoded = decode.decode(noise);

        if (decoded.size() != message.size()) {
            std::cerr << "WARNING: Decoded length (" << decoded.size() << ") != message length (" << message.size() << ") at errorProbability = " << errorProbability << "\n";
        }

        std::size_t errors = 0;
        std::size_t compareLength = std::min(decoded.size(), message.size());
        for (std::size_t i = 0; i < compareLength; ++i) if (decoded[i] != message[i]) ++errors;
        errors += (message.size() > compareLength) ? (message.size() - compareLength) : 0;
    
        double BER = static_cast<double>(errors) / static_cast<double>(message.size());
        std::cout << errorProbability << "," << BER << "\n";

        std::cout << "p = " << std::setw(5) << errorProbability << " BER = " << BER << "\n";
    }

    return 0;    

}

