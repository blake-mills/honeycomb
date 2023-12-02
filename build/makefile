# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -std=c++17 -Wall -Wextra

# Directories
SRC_DIR := ../src
OBJ_DIR := objects
EXE_DIR := executable

# Find all CPP files
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# Generate object file names by replacing 'src' with 'build/objects'
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Find all HPP files
HDRS := $(shell find $(SRC_DIR) -name '*.hpp')

# Makefile targets
.PHONY: all clean

all: $(EXE_DIR)/honeycomb

# Rule to build the executable
$(EXE_DIR)/honeycomb: $(OBJS) | $(EXE_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to generate object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS) | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directories if they don't exist
$(OBJ_DIR) $(EXE_DIR):
	mkdir -p $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(EXE_DIR)
