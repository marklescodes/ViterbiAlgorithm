CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -g
INC = -I$(SRC_DIR)

SRC_DIR = src
CODEC_DIR = $(SRC_DIR)/codec
DECODER_DIR = $(SRC_DIR)/decoder
CHANNEL_DIR = $(SRC_DIR)/channel
BUILD_DIR = build

SOURCES = $(SRC_DIR)/main.cpp \
          $(CODEC_DIR)/ConvolutionalCode.cpp \
          $(DECODER_DIR)/Viterbi.cpp

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

TARGET = viterbi

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean