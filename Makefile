ifeq ($(OS),Windows_NT)
    CXX = g++.exe
else
    CXX = g++
endif

CXXFLAGS = -std=c++17 -O2 -Wall

SRC = src/main.cpp src/codec/ConvolutionalCode.cpp src/decoder/Viterbi.cpp
OBJ = $(SRC:.cpp=.o)

ifeq ($(OS),Windows_NT)
    TARGET = viterbi.exe
else
    TARGET = viterbi
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)
