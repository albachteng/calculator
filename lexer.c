typedef struct string {
  char *ptr;
  int count;
} string;

typedef unsigned int u32;

typedef u32 TokenType;
enum {
  TokenType_EOF,
  TokenType_Plus,
  TokenType_Minus,
  TokenType_Star,
  TokenType_Slash,
  TokenType_Caret,
  TokenType_Number,
  TokenType_Error,
}; // union?

typedef struct Token {
  TokenType type;
  string *lexeme;
} Token;

typedef unsigned short int u8; // I think?

typedef struct Lexer { // represents the current token
  u8 *start;
  u8 *current;
} Lexer;

int is_whitespace(u8 token) { return 0; } // TODO

Token lexer_make_token(Lexer *lexer, TokenType type) {
  Token tok;
  tok.type = type;
  tok.lexeme = "";
  return tok;
}

int is_number(u8 n) {
  return 0; // TODO
}

Token lexer_number(Lexer *lexer) {
  while (is_number(lexer->current))
    lexer->current++;
  if (*lexer->current == '.') {
    lexer->current++;
    while (is_number(lexer->current))
      lexer->current++;
  }
  return Token{type : TokenType_Number, lexeme : lexer->current};
}

Token lexer_ident(Lexer *lexer) {
  // TODO
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
    return lexer_make_token(lexer, TokenType_Error)
  }
}
