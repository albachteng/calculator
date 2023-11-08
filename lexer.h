typedef unsigned int u32;
typedef unsigned char u8;

typedef struct string {
  u8 *ptr;
  u32 count;
} string;

typedef u32 TokenType;
enum {
  TokenType_EOF,
  TokenType_MIN,
  TokenType_Plus,
  TokenType_Minus,
  TokenType_Star,
  TokenType_Slash,
  TokenType_Caret,
  TokenType_Number,
  TokenType_OpenParenthesis,
  TokenType_CloseParenthesis,
  TokenType_Error,
  TokenType_Ident,
  TokenType_MAX,
}; // union?

typedef struct Token {
  TokenType type;
  string lexeme;
} Token;

typedef struct Lexer { // represents the current token
  u8 *start;
  u8 *current;
} Lexer;

int is_alpha(u8 token);
int is_whitespace(u8 token);
int is_number(u8 n);

Token lexer_make_token(Lexer *lexer, TokenType type);
Token lexer_number(Lexer *lexer);
Token lexer_ident(Lexer *lexer);
Token lexer_next_token(Lexer *lexer);
