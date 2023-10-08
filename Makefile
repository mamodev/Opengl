CC = g++
TARGET = main

FLAGS = -Wall -Wextra  -g -I/usr/local/include -I./include  -std=c++11
LDFLAGS = -lglfw -lGLEW -framework OpenGL

BUILD_DIR = ./build
$(shell mkdir -p $(BUILD_DIR))

SRCS = $(wildcard src/*.cpp)
SRCS += main.cpp

OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))
$(shell mkdir -p $(BUILD_DIR))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)