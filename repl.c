#include "repl.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROMPT ">> "
#define MAX_INPUT 1024

static const char *token_names[] = {[TOKEN_ILLEGAL] = "ILLEGAL",
                                    [TOKEN_EOF] = "EOF",
                                    [TOKEN_IDENT] = "IDENT",
                                    [TOKEN_INT] = "INT",
                                    [TOKEN_STRING] = "STRING",
                                    [TOKEN_ASSIGN] = "=",
                                    [TOKEN_PLUS] = "+",
                                    [TOKEN_MINUS] = "-",
                                    [TOKEN_BANG] = "!",
                                    [TOKEN_ASTERISK] = "*",
                                    [TOKEN_SLASH] = "/",
                                    [TOKEN_LT] = "<",
                                    [TOKEN_GT] = ">",
                                    [TOKEN_EQ] = "==",
                                    [TOKEN_NOT_EQ] = "!=",
                                    [TOKEN_COMMA] = ",",
                                    [TOKEN_SEMICOLON] = ";",
                                    [TOKEN_LPAREN] = "(",
                                    [TOKEN_RPAREN] = ")",
                                    [TOKEN_LBRACE] = "{",
                                    [TOKEN_RBRACE] = "}",
                                    [TOKEN_LBRACKET] = "[",
                                    [TOKEN_RBRACKET] = "]",
                                    [TOKEN_FUNCTION] = "FUNCTION",
                                    [TOKEN_LET] = "LET",
                                    [TOKEN_TRUE] = "TRUE",
                                    [TOKEN_FALSE] = "FALSE",
                                    [TOKEN_IF] = "IF",
                                    [TOKEN_ELSE] = "ELSE",
                                    [TOKEN_RETURN] = "RETURN"};

void start_repl(FILE *input, FILE *output) {
  char line[MAX_INPUT];

  while (1) {
    fprintf(output, PROMPT);
    fflush(output);

    if (fgets(line, sizeof(line), input) == NULL) {
      break;
    }

    line[strcspn(line, "\n")] = 0;

    if (strlen(line) == 0) {
      continue;
    }

    init_lexer(line);

    while (1) {
      Token token = next_token();

      if (token.type == TOKEN_EOF) {
        break;
      }

      fprintf(output, "{Type: %s Literal: %s}\n", token_names[token.type],
              token.literal);
    }
  }
}
