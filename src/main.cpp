#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

#include "codec/ConvolutionalCode.h"
#include "decoder/Viterbi.h"
#include "channel/BSC.h"

int main(int argc, char **argv) {
    size_t N = 100000;         
    double start = 0.0;        
    double end = 0.1;         
    double step = 0.01;        

    if (argc == 5) {
        N = std::stoul(argv[1]);
        start = std::stod(argv[2]);
        end = std::stod(argv[3]);
        step = std::stod(argv[4]);
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> bitDist(0, 1);

    std::cout << "Seed: " << rng() << "\n";

    ConvolutionalCode code(3, {7, 5});
    Viterbi decode(3, {7, 5});

    std::vector<int> message(N);
    for (auto &b : message) b = bitDist(rng);

    auto zeroCoded = code.code(message, true);
    auto zeroDecode = decode.decode(zeroCoded, message.size());
    if (zeroDecode.size() > message.size()) {
        zeroDecode.resize(message.size());
    }
    if (zeroDecode != message) {
        std::cerr << "Decoder does not invert encoder! Abort.\n";
        return 1;
    }

    for (double errorProbability = start; errorProbability <= end + 1e-12; errorProbability += step) {
        auto coded = code.code(message, true);
        auto noisy = BSC(coded, errorProbability, rng);
        auto decoded = decode.decode(noisy, message.size());

        if (decoded.size() > message.size()) {
            decoded.resize(message.size());
        }

        size_t errors = 0;
        for (size_t i = 0; i < message.size(); i++) {
            if (decoded[i] != message[i]) errors++;
        }

        double BER = static_cast<double>(errors) / message.size();

        std::cout << "Error probability = " << std::fixed << std::setprecision(4) << errorProbability
                  << " BER = " << std::fixed << BER << "\n";
    }

    return 0;
}
