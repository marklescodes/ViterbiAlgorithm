# Viterbi Convolutional Code Simulation

This project implements:
- Convolutional encoder
- Viterbi decoder
- Binary Symmetric Channel (BSC) simulation
- BER vs Error probability experiment

# Build
Clone the repo and run:

git clone https://github.com/YOURNAME/viterbi_project.git
cd viterbi_project
make

This will produce the binary viterbi

# Run

Example:

./viterbi --N 10000 --start 0.0 --end 0.3 --step 0.05 --out results.csv

# Arguments

--N <int> : message length (default 1000)

--convLength <int> : convolutional code constraint length (default 3)

--start <double> : start channel error probability (default 0.0)

--end <double> : end channel error probability (default 0.2)

--step <double> : increment for error probability (default 0.02)

--out <file> : output CSV file (default ber_vs_error.csv)

# Output

The program generates a CSV file with two columns:

ErrorProbability; BER
0.00; 0.0000
0.02; 0.0012
...

You can then plot it in Python/Excel/gnuplot.