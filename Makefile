CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g $(shell pkg-config --cflags cmocka)
LDFLAGS=$(shell pkg-config --libs cmocka)
OBJDIR=obj

# Core library sources (no main functions)
LIB_SOURCES=lexer.c parser.c ast.c repl.c memory.c sds.c
LIB_OBJECTS=$(LIB_SOURCES:%.c=$(OBJDIR)/%.o)

# Create obj directory if it doesn't exist
$(shell mkdir -p $(OBJDIR))

.PHONY: all clean test test-lexer test-parser test-ast help

all: monkey

# Main executable
monkey: $(LIB_OBJECTS) $(OBJDIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^

# Test targets
test: test-lexer test-parser test-ast

test-lexer: lexer-test
	./lexer-test

test-parser: parser-test
	./parser-test

test-ast: ast-test
	./ast-test

# Test executables
lexer-test: $(LIB_OBJECTS) $(OBJDIR)/lexer-test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

parser-test: $(LIB_OBJECTS) $(OBJDIR)/parser-test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ast-test: $(LIB_OBJECTS) $(OBJDIR)/ast-test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Object file compilation rule
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJDIR)
	rm -f lexer-test parser-test ast-test monkey

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the main executable"
	@echo "  test       - Run all tests"
	@echo "  test-lexer - Run lexer tests"
	@echo "  test-parser - Run parser tests" 
	@echo "  test-ast   - Run AST tests"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help message"
