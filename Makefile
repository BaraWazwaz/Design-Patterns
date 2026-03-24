# --- Configuration ---
CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -Iinclude -MMD -MP

# --- Project Structure ---
BIN_DIR  := bin
OBJ_DIR  := build
SRC_DIR  := src
INC_DIR  := include
DOC_DIR  := doc

TARGET   := $(BIN_DIR)/main.exe

# --- Source discovery (recursive, Git Bash safe) ---
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")

# Map src/... → build/...
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS    := $(OBJECTS:.o=.d)

# --- Build Rules ---
.PHONY: all clean run doc

all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencies
-include $(DEPS)

# --- Documentation ---
doc: $(DOC_DIR)/output/index.html

$(DOC_DIR)/output/index.html: $(SOURCES) $(shell find $(INC_DIR) -name "*.hpp") $(DOC_DIR)/Doxyfile
	@echo "Generating documentation..."
	doxygen $(DOC_DIR)/Doxyfile

$(DOC_DIR)/Doxyfile:
	mkdir -p $(DOC_DIR)
	doxygen -g $(DOC_DIR)/Doxyfile

# --- Utility ---
run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)