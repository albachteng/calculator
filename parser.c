typedef struct Parser {
  Lexer lexer;
  Token curr;
} Parser;

#define parser_advance(parser) parser->curr = lexer_next_token(parser->lexer);

Expression_Node *parser_parse_number(Parser *parser) {
  Expression_Node *ret = alloc_node(); // TODO
  ret->type = NodeType_Number;
  ret->number = number_from_string(parser->curr.lexeme);
  parser_advance(parser);
  return ret;
}

typedef unsigned int u32;
typedef u32 Precedence;
enum {
  Precedence_Min, // NOTE: special, value - 0
  Precedence_Term,
  Precedence_Factor,
  Precedence_Power,
};

// ooh this is new:
static Precedence precedence_lookup[TokenType_MAX] = {
    [TokenType_Plus] = Precedence_Term,   [TokenType_Minus] = Precedence_Term,
    [TokenType_Star] = Precedence_Factor, [TokenType_Slash] = Precedence_Factor,
    [TokenType_Caret] = Precedence_Power,
};

Expression_Node *parser_parse_terminal_expr(Parser *parser) {
  Expression_Node *ret = 0;
  if (parser->curr.type == TokenType_Number) {
    ret = parser_parse_number(parser);
  } else if (parser->curr.type == TokenType_OpenParenthesis) {
    parser_advance(parser);
    ret = parser_parse_expression(parser, Precedence_MIN);
    if (parser->curr.type == TokenType_CloseParenthesis) {
      parser_advance(parser);
    }
  } else if (parser->curr.type == TokenType_Plus) {
    parser_advance(parser);
    ret = alloc_node();
    ret->type = NodeType_Positive;
    ret->unary.operand = parser_parse_terminal_expr(parser);
  } else if (parser->curr.type == TokenType_Minus) {
    parser_advance(parser);
    ret = alloc_node();
    ret->type = NodeType_Negative;
    ret->unary.operand = parser_parse_terminal_expr(parser);
  }
  return ret;
};

Expression_Node *parser_parse_infix_expr(Parser *parser, Token operator,
                                         Expression_Node * left) {
  Expression_Node *ret = alloc_node(); // TODO;
  switch (operator.type) {
  case TokenType_Plus:
    ret->type = NodeType_Add;
    break;
  case TokenType_Minus:
    ret->type = NodeType_Subtract;
    break;
  case TokenType_Star:
    ret->type = NodeType_Multiply;
    break;
  case TokenType_Slash:
    ret->type = NodeType_Slash;
    break;
  case TokenType_Caret:
    ret->type = NodeType_Caret;
    break;
  }
  ret->binary.left = left;
  ret->binary.right = parser_parse_expression(parser, precedence_lookup[operator.type]);
  return ret;
};

Expression_Node *parser_parse_expression(Parser *parser) {
  Expression_Node *left = parser_parse_terminal_expr(parser);
  Token curr_operator = parser->curr;
  Precedence curr_prec = precedence_lookup[curr_operator.type];
  while (curr_prec != Precedence_MIN) {
    if (prev_prec >= curr_prec) {
      break;
    } else {
      parser_advance(parser);
      left = parser_parse_infix_expr(parser, curr_operator, left);
      curr_operator = parser->curr;
      curr_prec = precedence_lookup[curr_operator.type];
    }
  }
  return left;
};
