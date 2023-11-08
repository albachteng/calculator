typedef struct Expression_Node Expression_Node;

typedef double f64; // I think? -

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
    f64 number; // leaf
    struct {
      Expression_Node *operand;
    } unary; // operator
    struct {
      Expression_Node *left;
      Expression_Node *right;
    } binary; // operator
  };
};

f64 evaluate(Expression_Node *node);
