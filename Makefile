CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -Iinclude
LIBS = -lzstd

# Source files
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Test files
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_TARGETS = $(TEST_FILES:$(TEST_DIR)/%.c=$(OBJ_DIR)/test_%)

# Default target
all: $(TEST_TARGETS)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile test targets
$(OBJ_DIR)/test_%: $(TEST_DIR)/%.c $(OBJ_FILES) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(OBJ_FILES) $(LIBS) -o $@

# Test the FEN board functionality
test_fen: $(OBJ_DIR)/test_test_fen_board
	./$(OBJ_DIR)/test_test_fen_board

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)

# Run all tests
test: test_fen
	@echo "All tests completed!"

.PHONY: all clean test test_fen 