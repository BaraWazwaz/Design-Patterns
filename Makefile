# --- Windows Configuration ---
# Force Make to use CMD.exe as the shell to avoid conflicts if sh.exe is in PATH
SHELL    := cmd.exe
TARGET   := bin/main.exe
CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++23 -Iinclude
SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := bin

# --- File Discovery ---
# Use forward slashes for discovery (G++ understands them fine)
SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# --- Build Rules ---

all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile .cpp to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Windows-specific directory creation
$(BIN_DIR) $(OBJ_DIR):
	@if not exist "$@" mkdir "$@"

# --- Utility Commands ---

.PHONY: clean run

# Windows 'del' needs backslashes to work properly
clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

run: all
	@.\$(TARGET)

docs/output/index.html: $(SOURCES) include/*.hpp docs/Doxyfile
	@echo "Configure the docs/Doxyfile to your preferences..."
	@pause
	doxygen docs/Doxyfile

docs/Doxyfile: docs
	@doxygen -g docs/Doxyfile

docs:
	@mkdir docs

doc: docs/output/index.html