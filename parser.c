#include "parser.h"
#include "expression_node.h"
#include "stdlib.h"

Expression_Node *error_node();
Expression_Node *parser_parse_expression(Parser *parser, Precedence pre_prec);

f64 atof(const char *str);

void parser_advance(Parser *parser) {
  parser->curr = lexer_next_token(&parser->lexer);
};

Expression_Node *alloc_node() { return &(Expression_Node){}; }; // TODO
f64 number_from_string();                                       // TODO

Expression_Node *parser_parse_number(Parser *parser) {
  Expression_Node *ret = alloc_node();
  ret->type = NodeType_Number;
  ret->number = atof((const char *)parser->curr.lexeme.ptr);
  parser_advance(parser);
  return ret;
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
    ret = parser_parse_expression(parser, Precedence_Min);
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
  } else
    return error_node(); // TODO
  // implicit multiplication
  if (parser->curr.type == TokenType_Number ||
      parser->curr.type == TokenType_OpenParenthesis) {
    Expression_Node *synthetic_ret = alloc_node();
    synthetic_ret->type = NodeType_Multiply;
    synthetic_ret->binary.left = ret;
    synthetic_ret->binary.right =
        parser_parse_expression(parser, Precedence_Factor);
    ret = synthetic_ret;
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
    ret->type = NodeType_Divide;
    break;
  case TokenType_Caret:
    ret->type = NodeType_Power;
    break;
  }
  ret->binary.left = left;
  ret->binary.right = parser_parse_expression(parser, precedence_lookup[operator.type]);
  return ret;
};

Expression_Node *parser_parse_expression(Parser *parser, Precedence prev_prec) {
  Expression_Node *left = parser_parse_terminal_expr(parser);
  Token curr_operator = parser->curr;
  Precedence curr_prec = precedence_lookup[curr_operator.type];
  while (curr_prec != Precedence_Min) {
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
