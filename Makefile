CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

SRC = src/main.cpp src/codec/ConvolutionalCode.cpp src/decoder/Viterbi.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = viterbi

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)