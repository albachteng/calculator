#include "expression_node.h"
#include <math.h>
#include <stdint.h>

f64 evaluate(Expression_Node *node) {
  switch (node->type) {
  case NodeType_Number:
    return node->number;
  case NodeType_Positive:
    return evaluate(node->unary.operand);
  case NodeType_Negative:
    return -evaluate(node->unary.operand);
  case NodeType_Add:
    return evaluate(node->binary.left) + evaluate(node->binary.right);
  case NodeType_Subtract:
    return evaluate(node->binary.left) - evaluate(node->binary.right);
  case NodeType_Multiply:
    return evaluate(node->binary.left) * evaluate(node->binary.right);
  case NodeType_Divide:
    return evaluate(node->binary.left) / evaluate(node->binary.right);
  case NodeType_Power:
    return pow(evaluate(node->binary.left), evaluate(node->binary.right));
  default:
    return 0; // ?
  }
};
