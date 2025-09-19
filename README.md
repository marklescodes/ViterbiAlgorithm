# Viterbi Convolutional Code Simulation

This project implements:
- Convolutional encoder
- Viterbi decoder
- Binary Symmetric Channel (BSC) simulation
- BER vs Error probability experiment

# Build
Clone the repo and run:  
git clone https://github.com/marklescodes/ViterbiAlgorithm.git

## For Linux:
make\
./viterbi 100000 0.0 0.1 0.01


## For Windows:
make\
.\viterbi.exe 100000 0.0 0.1 0.01

This will produce the binary viterbi

# Arguments
./viterbi [N start end step]<br />
N — number of bits in the message (default 100000)

start — starting error probability (default 0.0)

end — ending error probability (default 0.1)

step — increment of error probability (default 0.01)

# Output

The program generates a CSV file with two columns:<br />

Seed: 12345678<br />
Error probability = 0.0000 BER = 0.00000000<br />
Error probability = 0.0100 BER = 0.00007000<br />
Error probability = 0.0200 BER = 0.00025000<br />
...<br />


You can then plot it in Python/Excel/gnuplot.
