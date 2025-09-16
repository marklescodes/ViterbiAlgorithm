#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <string>
#include "codec/ConvolutionalCode.h"
#include "decoder/Viterbi.h"
#include "channel/BSC.h"

constexpr double epsilon = 1e-12;

int main(int argc, char *argv[]) {
    size_t convLength = 3;
    std::vector<int> polynoms = {7, 5};
    size_t N = 1000;
    double start = 0.0, end = 0.2, step = 0.02;
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

    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);

    ConvolutionalCode code(convLength, polynoms);
    Viterbi decode(convLength, polynoms);

    std::vector<int> message(N);
    std::uniform_int_distribution<int> bitDist(0,1);
    for (size_t i = 0; i < N; ++i)
        message[i] = bitDist(rng);

    std::ofstream out(outputFile);
    if (!out) {
        std::cerr << "Error: cannot open file " << outputFile << "\n";
        return 1;
    }
    out << "ErrorProbability; BER\n";
    
    auto zeroCoded = code.code(message, true);
    auto zeroDecode = decode.decode(zeroCoded);
    if (zeroCoded != message) {
        std::cerr << "Check FAILED (decoder does not invert encoder correctly).\n";
        return 2;
    }
    

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Seed: " << seed << "\n";

    for (double errorProbability = start; errorProbability <= end + epsilon; errorProbability += step) {
        auto coded = code.code(message, true);
        auto noise = BSC(coded, errorProbability, rng);
        auto decoded = decode.decode(noise);

        size_t errors = 0;
        size_t compareLength = std::min(decoded.size(), message.size());
        for (size_t i = 0; i < compareLength; ++i)
            if (decoded[i] != message[i]) ++errors;
        errors += (message.size() > compareLength) ? (message.size() - compareLength) : 0;

        double BER = static_cast<double>(errors) / static_cast<double>(message.size());
        out << errorProbability << ";" << BER << "\n";

        std::cout << "Error probability = " << std:: setw(5) << errorProbability << " Ber = " << BER << "\n";
    }

    std::cout << "CSV file created: " << outputFile << "\n";
    return 0;
}