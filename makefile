# Compiler
CXX := g++
CC := gcc

# Directories
SRC_DIR := src
INC_DIR := include
GLAD_DIR := glad
IMGUI_DIR := $(INC_DIR)/imgui
BUILD_DIR := build

# Output executable
TARGET := play

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
GLAD_SOURCE := $(GLAD_DIR)/src/glad.c

# ImGui core + GLFW/OpenGL3 backend sources
IMGUI_SOURCES := $(wildcard $(IMGUI_DIR)/*.cpp) \
                  $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
                  $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# Object files
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
GLAD_OBJECT := $(BUILD_DIR)/glad.o
IMGUI_OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/imgui/%.o,$(notdir $(IMGUI_SOURCES)))

# Compiler flags
CXXFLAGS := -Wall -Wextra -std=c++17 \
            -I$(INC_DIR) \
            -I$(IMGUI_DIR) \
            -I$(IMGUI_DIR)/backends \
            -I$(GLAD_DIR)/include

CFLAGS := -Wall -Wextra \
          -I$(GLAD_DIR)/include

# GLFW + OpenGL libraries
LDLIBS := -lglfw -lGL -ldl -lpthread

# Let make find imgui .cpp files whether they're in IMGUI_DIR or its backends subdir
vpath %.cpp $(IMGUI_DIR):$(IMGUI_DIR)/backends

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS) $(GLAD_OBJECT) $(IMGUI_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) $(GLAD_OBJECT) $(IMGUI_OBJECTS) -o $@ $(LDLIBS)

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Compile ImGui source files (found via vpath above)
$(BUILD_DIR)/imgui/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)/imgui
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Compile GLAD
$(GLAD_OBJECT): $(GLAD_SOURCE)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Include generated header dependencies
-include $(OBJECTS:.o=.d)
-include $(IMGUI_OBJECTS:.o=.d)

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