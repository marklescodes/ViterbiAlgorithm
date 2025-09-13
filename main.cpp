#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <ctime>

using namespace std;

class ConvolutionalCode {
public:
    int convlength;
    vector<int> polynoms;

    ConvolutionalCode(int k, vector<int> p):
    convlength(k), polynoms(p) {}

    vector<int> encode(const vector<int> &bits) {
        vector <int> output;
        vector <int> shift(convlength, 0);

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
    vector<int> polynoms;
    int numState;

    Viterbi(int k, vector<int> p): convlength(k), polynoms(p) {
        numState = 1 << (convlength - 1);
    }

    vector<int> decode(const vector<int> &receive) {
        int n = polynoms.size();
        int T = receive.size() /n;

        vector<vector<int>> paths(T + 1, vector<int>(numState, 1e9));
        vector<vector<int>> predecessor(T + 1, vector<int>(numState, -1));
        vector<vector<int>> inputBit(T + 1, vector<int>(numState, -1));

        paths[0][0] = 0;

        for (int t = 0; t < T; t++) {
            for (int state = 0; state < numState; state++) {
                if (paths[t][state] >= 1e9) continue;

                for (int bit = 0; bit <= 1; bit++) {
                    int nextState = ((state << 1) | bit) & (numState - 1);
                

                vector<int> shift(convlength);
                for (int i = 0; i < convlength - 1; i++)
                    shift[i] = (state >> i) & 1;
                shift[convlength - 1] = bit;

                vector<int> out;
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

    vector<int> decodeBits(T);
    for (int t = T; t > 0; t--) {
        decodeBits[t - 1] = inputBit[t][best];
        best = predecessor[t][best];
    }

    return decodeBits;
    }
};

vector<int> BSC(const vector<int> &bits, double pError) {
    vector<int> noise(bits.size());
    static random_device rd;
    static mt19937 gen(rd());
    bernoulli_distribution dist (pError);

    for (size_t i = 0; i < bits.size(); i++) {
        noise[i] = bits[i];
        if (dist(gen)) noise[i] ^= 1;
    }
    return noise;
}

int main() {

int k = 3;
vector<int> polynoms = {7, 5};

ConvolutionalCode coding(k, polynoms);
Viterbi decoding(k, polynoms);

int N = 1000;
vector<int> message(N);

mt19937 rndm(time(0));
uniform_int_distribution<int> bitDist(0, 1);
for (int i = 0; i < N; i++) message[i] = bitDist(rndm);



return 0;    

}