# --- Configuration ---
SHELL    := cmd.exe
CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -Iinclude

# --- Project Structure ---
BIN_DIR  := bin
OBJ_DIR  := build
SRC_DIR  := src
DOC_DIR  := docs

TARGET   := $(BIN_DIR)/main.exe
SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# --- Build Rules ---
.PHONY: all clean run doc

all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Directory creation (Windows-safe)
$(BIN_DIR) $(OBJ_DIR) $(DOC_DIR):
	@if not exist "$@" mkdir "$@"

# --- Documentation ---
doc: $(DOC_DIR)/output/index.html

$(DOC_DIR)/output/index.html: $(SOURCES) include/*.hpp $(DOC_DIR)/Doxyfile
	@echo "Generating documentation..."
	doxygen $(DOC_DIR)/Doxyfile

$(DOC_DIR)/Doxyfile: | $(DOC_DIR)
	@echo "Configuring the $(DOC_DIR)/Doxyfile..."
	doxygen -g $(DOC_DIR)/Doxyfile

# --- Utility Commands ---
run: all
	@.\$(TARGET)

clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)