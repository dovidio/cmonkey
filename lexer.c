#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void read_char();
static Lexer lexer;

void init_lexer(const char *input) {
  lexer.input = input;
  lexer.position = 0;
  lexer.read_position = 0;
  lexer.ch = 0;
  lexer.line = 1;

  read_char();
}

static void read_char() {
  if (lexer.read_position >= (int)strlen(lexer.input)) {
    lexer.ch = 0;
  } else {
    lexer.ch = lexer.input[lexer.read_position];
  }
  lexer.position = lexer.read_position;
  lexer.read_position++;
}

static char peek_char() {
  if (lexer.read_position >= (int)strlen(lexer.input)) {
    return 0;
  } else {
    return lexer.input[lexer.read_position];
  }
}

static void skip_whitespace() {
  while (lexer.ch == ' ' || lexer.ch == '\t' || lexer.ch == '\n' ||
         lexer.ch == '\r') {
    if (lexer.ch == '\n') {
      lexer.line++;
    }
    read_char();
  }
}

static Token make_token(TokenType type, const char *start, int length,
                        int line) {
  Token token;
  token.type = type;
  token.start = start;
  token.length = length;
  token.line = line;
  return token;
}

static int is_letter(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

static int is_digit(char ch) { return ch >= '0' && ch <= '9'; }

static TokenType lookup_ident(const char *start, int length) {
  if (length == 2 && strncmp(start, "fn", 2) == 0)
    return TOKEN_FUNCTION;
  if (length == 2 && strncmp(start, "if", 2) == 0)
    return TOKEN_IF;
  if (length == 3 && strncmp(start, "let", 3) == 0)
    return TOKEN_LET;
  if (length == 4 && strncmp(start, "true", 4) == 0)
    return TOKEN_TRUE;
  if (length == 4 && strncmp(start, "else", 4) == 0)
    return TOKEN_ELSE;
  if (length == 5 && strncmp(start, "false", 5) == 0)
    return TOKEN_FALSE;
  if (length == 6 && strncmp(start, "return", 6) == 0)
    return TOKEN_RETURN;
  return TOKEN_IDENT;
}

static Token read_identifier() {
  const char *start = lexer.input + lexer.position;
  int line = lexer.line;

  while (is_letter(lexer.ch) || is_digit(lexer.ch)) {
    read_char();
  }

  int length = (lexer.input + lexer.position) - start;
  TokenType type = lookup_ident(start, length);

  return make_token(type, start, length, line);
}

static Token read_number() {
  const char *start = lexer.input + lexer.position;
  int line = lexer.line;

  while (is_digit(lexer.ch)) {
    read_char();
  }

  int length = (lexer.input + lexer.position) - start;
  return make_token(TOKEN_INT, start, length, line);
}

static Token read_string() {
  const char *start = lexer.input + lexer.position + 1;
  int line = lexer.line;

  read_char();
  while (lexer.ch != '"' && lexer.ch != 0) {
    if (lexer.ch == '\n') {
      lexer.line++;
    }
    read_char();
  }

  int length = (lexer.input + lexer.position) - start;

  if (lexer.ch == '"') {
    read_char();
  }

  return make_token(TOKEN_STRING, start, length, line);
}

Token next_token() {
  Token token;

  skip_whitespace();

  switch (lexer.ch) {
  case '=':
    if (peek_char() == '=') {
      const char *start = lexer.input + lexer.position;
      int line = lexer.line;
      read_char();
      token = make_token(TOKEN_EQ, start, 2, line);
    } else {
      token =
          make_token(TOKEN_ASSIGN, lexer.input + lexer.position, 1, lexer.line);
    }
    break;
  case '+':
    token = make_token(TOKEN_PLUS, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '-':
    token =
        make_token(TOKEN_MINUS, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '!':
    if (peek_char() == '=') {
      const char *start = lexer.input + lexer.position;
      int line = lexer.line;
      read_char();
      token = make_token(TOKEN_NOT_EQ, start, 2, line);
    } else {
      token =
          make_token(TOKEN_BANG, lexer.input + lexer.position, 1, lexer.line);
    }
    break;
  case '*':
    token =
        make_token(TOKEN_ASTERISK, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '/':
    token =
        make_token(TOKEN_SLASH, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '<':
    token = make_token(TOKEN_LT, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '>':
    token = make_token(TOKEN_GT, lexer.input + lexer.position, 1, lexer.line);
    break;
  case ',':
    token =
        make_token(TOKEN_COMMA, lexer.input + lexer.position, 1, lexer.line);
    break;
  case ';':
    token = make_token(TOKEN_SEMICOLON, lexer.input + lexer.position, 1,
                       lexer.line);
    break;
  case '(':
    token =
        make_token(TOKEN_LPAREN, lexer.input + lexer.position, 1, lexer.line);
    break;
  case ')':
    token =
        make_token(TOKEN_RPAREN, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '{':
    token =
        make_token(TOKEN_LBRACE, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '}':
    token =
        make_token(TOKEN_RBRACE, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '[':
    token =
        make_token(TOKEN_LBRACKET, lexer.input + lexer.position, 1, lexer.line);
    break;
  case ']':
    token =
        make_token(TOKEN_RBRACKET, lexer.input + lexer.position, 1, lexer.line);
    break;
  case '"':
    return read_string();
  case 0:
    token = make_token(TOKEN_EOF, lexer.input + lexer.position, 0, lexer.line);
    break;
  default:
    if (is_letter(lexer.ch)) {
      return read_identifier();
    } else if (is_digit(lexer.ch)) {
      return read_number();
    } else {
      token = make_token(TOKEN_ILLEGAL, lexer.input + lexer.position, 1,
                         lexer.line);
    }
    break;
  }

  read_char();
  return token;
}
