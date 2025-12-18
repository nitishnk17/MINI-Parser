# Compiler settings
CC = gcc
CFLAGS = -Wall -I.

# Target executable name
TARGET = parser

# Source files
SRC_DIR = src
LEX_SRC = $(SRC_DIR)/lexer.l
YACC_SRC = $(SRC_DIR)/parser.y
MAIN_SRC = $(SRC_DIR)/main.c

# Generated files
LEX_OUT = lex.yy.c
YACC_OUT = y.tab.c
YACC_HEADER = y.tab.h

# Default target: build the parser
all: $(TARGET)

# Link everything together
$(TARGET): $(LEX_OUT) $(YACC_OUT) $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(YACC_OUT) $(LEX_OUT) $(MAIN_SRC)

# Generate Lexer C code
$(LEX_OUT): $(LEX_SRC) $(YACC_HEADER)
	flex $(LEX_SRC)

# Generate Parser C code and Header (y.tab.h)
# -d generates the header file, -y enables yacc compatibility (y.tab.c naming)
$(YACC_OUT) $(YACC_HEADER): $(YACC_SRC)
	bison -dy $(YACC_SRC)

# Clean up generated files
clean:
	rm -f $(TARGET) $(LEX_OUT) $(YACC_OUT) $(YACC_HEADER)

.PHONY: all clean