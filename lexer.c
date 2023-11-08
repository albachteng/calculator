#include "lexer.h"

int is_alpha(u8 token) {
  if ((token < 'a' || token > 'z') && (token < 'A' || token > 'Z')) {
    return 0;
  }
  return 1;
}

int is_whitespace(u8 token) {
  if (token == ' ' || token == '\n' || token == '\t' || token == '\r' ||
      token == '\f' || token == '\v') {
    return 1;
  }
  return 0;
}

Token lexer_make_token(Lexer *lexer, TokenType type) {
  Token tok;
  tok.type = type;
  tok.lexeme = (string){.ptr = lexer->start,
                        .count = (u32)(lexer->current - lexer->start)};
  return tok;
}

int is_number(u8 n) {
  if (n >= '0' && n <= '9')
    return 1;
  return 0;
}

Token lexer_number(Lexer *lexer) {
  while (is_number(*lexer->current))
    lexer->current++;
  if (*lexer->current == '.') {
    lexer->current++;
    while (is_number(*lexer->current))
      lexer->current++;
  }
  return lexer_make_token(lexer, TokenType_Number);
}

Token lexer_ident(Lexer *lexer) {
  return lexer_make_token(lexer, TokenType_Ident);
}

Token lexer_next_token(Lexer *lexer) {
  while (is_whitespace(*lexer->start))
    lexer->start += 1;
  if (*lexer->current == '\0')
    return lexer_make_token(lexer, TokenType_EOF);
  lexer->current++;
  switch (*lexer->current) {
  // TODO
  case '+':
    lexer->current++;
    return lexer_make_token(lexer, TokenType_Plus);
  case '-':
    lexer->current++;
    return lexer_make_token(lexer, TokenType_Minus);
  case '*':
    lexer->current++;
    return lexer_make_token(lexer, TokenType_Star);
  case '/':
    lexer->current++;
    return lexer_make_token(lexer, TokenType_Slash);
  case '^':
    lexer->current++;
    return lexer_make_token(lexer, TokenType_Caret);
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return lexer_number(lexer);

  default:
    if (is_alpha(*lexer->current))
      return lexer_ident(lexer); // TODO
    return lexer_make_token(lexer, TokenType_Error);
  }
}
