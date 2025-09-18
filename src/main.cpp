#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

#include "codec/ConvolutionalCode.h"
#include "decoder/Viterbi.h"
#include "channel/BSC.h"

int main(int argc, char *argv[]) {
    size_t N = 100000;
    size_t convLength = 3;
    std::vector<int> polynoms = {7, 5};
    double start = 0.01, end = 0.10, step = 0.01;
    std::string outputFile = "ber_vs_error.csv";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--N" && i + 1 < argc) N = std::stoul(argv[++i]);
        else if (arg == "--convLength" && i + 1 < argc) convLength = std::stoul(argv[++i]);
        else if (arg == "--start" && i + 1 < argc) start = std::stod(argv[++i]);
        else if (arg == "--end" && i + 1 < argc) end = std::stod(argv[++i]);
        else if (arg == "--step" && i + 1 < argc) step = std::stod(argv[++i]);
        else if (arg == "--out" && i + 1 < argc) outputFile = argv[++i];
    }

    if (N == 0 || convLength == 0 || polynoms.empty() || step <= 0.0 || start > end) {
        std::cerr << "Bad params: "
                  << "N>0, convLength>0, polynoms non-empty, step>0, start<=end required.\n";
        return 2;
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0,1);

    std::vector<int> message(N);
    for (auto &b : message) b = dist(rng);

    ConvolutionalCode code(convLength, polynoms);
    Viterbi decode(convLength, polynoms);

    auto coded = code.code(message);
    auto zeroDecode = decode.decode(coded, message.size());
    if (zeroDecode != message) {
        std::cerr << "Decoder does not invert encoder! Abort.\n";
        return 1;
    }

    std::ofstream out(outputFile);
    if (!out) {
        std::cerr << "Cannot open output file: " << outputFile << "\n";
        return 3;
    }
    out << "ErrorProbability;BER\n";


    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Seed: " << seed << "\n";

    for (double p = start; p <= end + 1e-12; p += step) {
        auto noisy = BSC(coded, p, rng); 

        auto decoded = decode.decode(noisy, message.size());

        size_t errors = 0;
        for (size_t i = 0; i < N; ++i)
            if (decoded[i] != message[i]) ++errors;

        double BER = static_cast<double>(errors) / static_cast<double>(N);

        std::cout << "Error probability = " << p << "  BER = " << BER << "\n";
        out << p << ";" << BER << "\n";
    }

    std::cout << "CSV created: " << outputFile << "\n";
    return 0;
}
