CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g $(shell pkg-config --cflags cmocka)
LDFLAGS=$(shell pkg-config --libs cmocka)
SRCDIR=.
OBJDIR=obj
SOURCES=$(wildcard $(SRCDIR)/*.c)
MAIN_SOURCES=$(filter-out $(SRCDIR)/lexer-test.c $(SRCDIR)/parser-test.c, $(SOURCES))
LEXER_TEST_SOURCES=$(filter-out $(SRCDIR)/main.c $(SRCDIR)/parser-test.c, $(SOURCES))
PARSER_TEST_SOURCES=$(filter-out $(SRCDIR)/main.c $(SRCDIR)/lexer-test.c, $(SOURCES))

$(shell mkdir -p $(OBJDIR))

MAIN_OBJECTS=$(MAIN_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LEXER_TEST_OBJECTS=$(LEXER_TEST_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
PARSER_TEST_OBJECTS=$(PARSER_TEST_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

.PHONY: all clean test run test-lexer test-parser

all: monkey

monkey: $(MAIN_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

test: test-lexer test-parser

test-lexer: lexer-test
	./lexer-test

test-parser: parser-test
	./parser-test

lexer-test: $(LEXER_TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

parser-test: $(PARSER_TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	rm -f lexer-test monkey

$(OBJDIR)/lexer.o: lexer.c lexer.h
$(OBJDIR)/lexer-test.o: lexer-test.c lexer.h
$(OBJDIR)/repl.o: repl.c repl.h lexer.h
$(OBJDIR)/main.o: main.c repl.h

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all        - Build the main executable (not implemented yet)"
	@echo "  test       - Build and run tests with CMocka test framework"
	@echo "  lexer-test - Build test executable"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help message"
