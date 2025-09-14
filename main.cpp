#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <ctime>



class ConvolutionalCode {
public:
    int convlength;
    std::vector<int> polynoms;

    ConvolutionalCode(int k, std::vector<int> p):
    convlength(k), polynoms(p) {}

    std::vector<int> encode(const std::vector<int> &bits) {
        std::vector <int> output;
        std::vector <int> shift(convlength, 0);

        for (size_t n = 0; n < bits.size(); n++) {
            for (int i = convlength - 1; i >0 ; i--)
                shift[i] = shift[i-1];
            shift[0] = bits[n];

            for (size_t g = 0; g < polynoms.size(); g++) {
                int gen = polynoms[g];
                int val = 0;
                for (int i = 0; i < convlength; i++) {
                    if ((gen >> i) & 1) val ^= shift[i];
                }
                output.push_back(val);
            }
        }
        return output;
       
    }
};

class Viterbi{
public:
    int convlength;
    std::vector<int> polynoms;
    int numState;

    Viterbi(int k, std::vector<int> p): convlength(k), polynoms(p) {
        numState = 1 << (convlength - 1);
    }

    std::vector<int> decode(const std::vector<int> &receive) {
        int n = polynoms.size();
        int T = receive.size() /n;

        std::vector<std::vector<int>> paths(T + 1, std::vector<int>(numState, 1e9));
        std::vector<std::vector<int>> predecessor(T + 1, std::vector<int>(numState, -1));
        std::vector<std::vector<int>> inputBit(T + 1, std::vector<int>(numState, -1));

        paths[0][0] = 0;

        for (int t = 0; t < T; t++) {
            for (int state = 0; state < numState; state++) {
                if (paths[t][state] >= 1e9) continue;

                for (int bit = 0; bit <= 1; bit++) {
                    int nextState = ((state << 1) | bit) & (numState - 1);
                

                std::vector<int> shift(convlength);
                for (int i = 0; i < convlength - 1; i++)
                    shift[i] = (state >> i) & 1;
                shift[convlength - 1] = bit;

                std::vector<int> out;
                for (int g: polynoms) {
                    int val = 0;
                    for (int i = 0; i < convlength; i++) {
                        if ((g >> i) & 1) val ^= shift[i];
                    }
                    out.push_back(val);
                }

                int hamdist = 0;
                for (int k = 0; k < n; k++) {
                    if (out[k] != receive[t * n + k]) hamdist++;
                }
                
                int metric = paths[t][state] + hamdist;
                if (metric < paths[t+1][nextState]) {
                    paths[t+1][nextState] = metric;
                    predecessor[t+1][nextState] = state;
                    inputBit[t+1][nextState] = bit;
                }
            }
        }
    }
    
    int best = min_element(paths[T].begin(), paths[T].end()) - paths[T].begin();

    std::vector<int> decodeBits(T);
    for (int t = T; t > 0; t--) {
        decodeBits[t - 1] = inputBit[t][best];
        best = predecessor[t][best];
    }

    return decodeBits;
    }
};

std::vector<int> BSC(const std::vector<int> &bits, double pError) {
    std::vector<int> noise(bits.size());
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::bernoulli_distribution dist (pError);

    for (size_t i = 0; i < bits.size(); i++) {
        noise[i] = bits[i];
        if (dist(gen)) noise[i] ^= 1;
    }
    return noise;
}

int main() {

int k = 3;
std::vector<int> polynoms = {7, 5};

ConvolutionalCode coding(k, polynoms);
Viterbi decoding(k, polynoms);

int N = 1000;
std::vector<int> message(N);

std::mt19937 rndm(time(0));
std::uniform_int_distribution<int> bitDist(0, 1);
for (int i = 0; i < N; i++) message[i] = bitDist(rndm);

std::cout << "Error probability  BER" << std::endl;

for (double p = 0.0; p <= 0.2; p += 0.02) {
    std::vector<int> coded = coding.encode(message);
    std::vector<int> noise = BSC(coded, p);
    std::vector<int> decoded = decoding.decode(noise);

    int errors = 0;
    for (int i = 0; i < N; i++) {
        if (decoded[i] != message[i]) errors++;
    }
    double BER = (double)errors / N;
    std::cout << p << "  " << BER << std::endl;
}

return 0;    

}