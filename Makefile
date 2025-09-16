CXX = g++.exe
CXXFLAGS = -std=c++17 -O2 -Wall

SRC = src/main.cpp src/codec/ConvolutionalCode.cpp src/decoder/Viterbi.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = viterbi.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
