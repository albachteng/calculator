#include "lexer.h"

typedef struct Parser {
  Lexer lexer;
  Token curr;
} Parser;

typedef unsigned int u32;
typedef u32 Precedence;
enum {
  Precedence_Min, // NOTE: special, value - 0
  Precedence_Term,
  Precedence_Factor,
  Precedence_Power,
};
