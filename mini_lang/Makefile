CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wno-write-strings -I$(SRC_DIR) -I$(BUILD_DIR)
FLEX = flex
BISON = bison

SRC_DIR = src
BUILD_DIR = build

TARGET = $(BUILD_DIR)/parser

PARSER_SRC = $(SRC_DIR)/parser.y
LEXER_SRC = $(SRC_DIR)/lexer.l
AST_SRC = $(SRC_DIR)/ast.cpp

PARSER_GEN = $(BUILD_DIR)/parser.tab.c
PARSER_HDR = $(BUILD_DIR)/parser.tab.h
LEXER_GEN = $(BUILD_DIR)/lex.yy.c

PARSER_OBJ = $(BUILD_DIR)/parser.tab.o
LEXER_OBJ = $(BUILD_DIR)/lex.yy.o
AST_OBJ = $(BUILD_DIR)/ast.o

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(PARSER_OBJ) $(LEXER_OBJ) $(AST_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: $(TARGET)"

$(PARSER_GEN) $(PARSER_HDR): $(PARSER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(BISON) -d -o $(PARSER_GEN) $(PARSER_SRC)

$(LEXER_GEN): $(LEXER_SRC) $(PARSER_HDR)
	@mkdir -p $(BUILD_DIR)
	$(FLEX) -o $(LEXER_GEN) $(LEXER_SRC)

$(BUILD_DIR)/parser.tab.o: $(PARSER_GEN)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/lex.yy.o: $(LEXER_GEN)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/ast.o: $(AST_SRC) $(SRC_DIR)/ast.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"

test: $(TARGET)
	@echo "Running parser..."
	@$(TARGET)
