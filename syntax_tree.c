#include <stdint.h>
typedef struct Expression_Node Expression_Node;

typedef float f64; // I think?

typedef unsigned int u32; // I think?

typedef u32 Expression_Node_Kind;
enum {
  NodeType_Error,
  NodeType_Number,
  NodeType_Positive,
  NodeType_Negative,
  NodeType_Add,
  NodeType_Subtract,
  NodeType_Multiply,
  NodeType_Divide,
  NodeType_Power,
};

typedef struct Expression_Node Expression_Node;
struct Expression_Node {
  Expression_Node_Kind type;

  union {
    f64 number;
    struct {
      Expression_Node *operand;
    } unary;
    struct {
      Expression_Node *left;
      Expression_Node *right;
    } binary;
  };
};

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
    return evaluate(power(node->binary.left, node->binary.right));
    // TODO
  }
}
