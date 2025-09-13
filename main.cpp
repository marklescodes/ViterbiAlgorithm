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

    vector<int> code(const vector<int> &bits) {
        vector <int> output;
        vector <int> shift(convlength, 0);

        for (size_t n = 0; n < bits.size(); n++) {
            for (int i = convlength - 1; i >0 ; i--)
                shift[i] = shift[i-1];
            shift[0] = bits[n];
        }

        for (size_t g = 0; g < polynoms.size(); g++) {
            int gen = polynoms[g];
            int val = 0;
            for (int i = 0; i < convlength; i++) {
                if ((gen >> i) & 1) val ^= shift[i];
            }
        }

        return output;
    }
};



int main() {


    return 0;
}

