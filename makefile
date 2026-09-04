# Compiler
CXX := g++
CC := gcc

# Directories
SRC_DIR := src
INC_DIR := include
GLAD_DIR := glad
BUILD_DIR := build

# Output executable
TARGET := play

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
GLAD_SOURCE := $(GLAD_DIR)/src/glad.c

# Object files
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
GLAD_OBJECT := $(BUILD_DIR)/glad.o

# Compiler flags
CXXFLAGS := -Wall -Wextra -std=c++17 \
            -I$(INC_DIR) \
            -I$(GLAD_DIR)/include

CFLAGS := -Wall -Wextra \
          -I$(GLAD_DIR)/include

# GLFW + OpenGL libraries
LDLIBS := -lglfw -lGL -ldl -lpthread

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS) $(GLAD_OBJECT)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) $(GLAD_OBJECT) -o $@ $(LDLIBS)

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Compile GLAD
$(GLAD_OBJECT): $(GLAD_SOURCE)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Include generated header dependencies
-include $(OBJECTS:.o=.d)

# Build and run
run: $(TARGET)
	./$(TARGET)

# Remove all build files
clean:
	rm -rf $(BUILD_DIR)
	rm $(TARGET)

# Clean, rebuild, and run
reset: clean
	$(MAKE) run

.PHONY: all run clean reset
