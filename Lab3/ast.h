
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

// Dummy node just so everything compiles, create your own node/nodes
//
// The code provided below is an example ONLY. You can use/modify it,
// but do not assume that it is correct or complete.
//
// There are many ways of making AST nodes. The approach below is an example
// of a descriminated union. If you choose to use C++, then I suggest looking
// into inheritance.

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum {
  UNKNOWN               = 0,

  SCOPE_NODE            = (1 << 0),
  
  EXPRESSION_NODE       = (1 << 2),
  UNARY_EXPRESION_NODE  = (1 << 2) | (1 << 3),
  BINARY_EXPRESSION_NODE= (1 << 2) | (1 << 4),
  INT_NODE              = (1 << 2) | (1 << 5), 
  FLOAT_NODE            = (1 << 2) | (1 << 6),
  IDENT_NODE            = (1 << 2) | (1 << 7),
  INDEX_NODE            = (1 << 2) | (1 << 8),
  FUNCTION_NODE         = (1 << 2) | (1 << 9),
  CONSTRUCTOR_NODE      = (1 << 2) | (1 << 10),

  STATEMENTS_NODE        = (1 << 1),
  IF_STATEMENT_NODE     = (1 << 1) | (1 << 11),
  ASSIGNMENT_NODE       = (1 << 1) | (1 << 13),
  NESTED_SCOPE_NODE     = (1 << 1) | (1 << 14),
  
  DECLARATIONS_NODE     = (1 << 14)
  DECLARATION_NODE      = (1 << 15)
} node_kind;

typedef enum {
    INT = 0,
    FLOAT = 1,
    BOOL = 2
} type_code;

typedef struct type {
    type_code base;
    bool isVec;
    int vecSize;
    bool isConst;
} type;

struct node_ {

  // an example of tagging each node with a type
  node_kind kind;

  union {
    struct {
      node *declarations;
      node *statements;
    } scope;

    struct {
      node *declarations;
      node *declaration;
    } declarations;

    struct {
      char *id;
      type *type;
      node *expression;
    } declaration;

    struct {
      node *statements;
      node *statement;
    } statements;

    struct {
      char *id;
      type *type;
      node *expression;
    } assignment;

    struct {
      node *cond;
      node *then_statement;
      node *else_statement;
    } if_statement;
    
    node *nested_scope;
  
    struct {
      int op;
      type *type;
      node *right;
    } unary;

    struct {
      int op;
      type *type;
      node *left;
      node *right;
    } binary;

    struct {
      char *id;
      type *type;
      node *index;
    } index;

    struct {
      char *name;
      node *args;
    } function;

    struct {
      type *type;
      node *args;
    } constructor;

    int int_val;
    float float_val;
    int bool_val;

    struct {
      node *args;
      node *expression;
    } arguments;
  };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);

#endif /* AST_H_ */
