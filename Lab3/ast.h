
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
  UNKNOWN,
          
  ENTER_SCOPE_NODE,
  SCOPE_NODE,
  
  NESTED_EXPRESSION_NODE,
  UNARY_EXPRESION_NODE,
  BINARY_EXPRESSION_NODE,
  INT_NODE, 
  FLOAT_NODE,
  BOOL_NODE,
  VARIABLE_NODE,
  EXPRESSION_VARIABLE_NODE,
  ARRAY_INDEX_NODE,
  FUNCTION_NODE,
  CONSTRUCTOR_NODE,
  ARGUMENTS_NODE,

  STATEMENTS_NODE,
  IF_STATEMENT_NODE,
  ASSIGNMENT_NODE,
  NESTED_SCOPE_NODE,
  
  DECLARATIONS_NODE,
  DECLARATION_NODE,
          
  TYPE_NODE 
} node_kind;

typedef enum {
    INT = 0,
    IVEC2,
    IVEC3,
    IVEC4,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    BOOL,
    BVEC2,
    BVEC3,
    BVEC4,
} type_code;

typedef enum {
    MINUS_OP,
    NOT_OP,
    AND_OP,
    OR_OP,
    EQ_OP,
    NEQ_OP,
    LT_OP,
    LEQ_OP,
    GT_OP,
    GEQ_OP,
    ADD_OP,
    SUB_OP,
    MULT_OP,
    DIV_OP,
    POW_OP
} op_code;

struct node_ {

  // an example of tagging each node with a type
  node_kind kind;
  int line_num;

  union {
    node *enter_scope;
      
    struct {
      node *declarations;
      node *statements;
    } scope;

    struct {
      node *declarations;
      node *declaration;
    } declarations;

    struct {
      int is_const;
      char *id;
      node *type;
      node *expression;
    } declaration;
    
    struct {
      node *statements;
      node *statement;
    } statements;

    struct {
      node *var;
      node *expression;
      type_code type;  // Assigned during semantic analysis
    } assignment;

    struct {
      node *cond;
      node *then_statement;
      node *else_statement;
    } if_statement;
    
    node *nested_scope;
  
    struct {
      int op;
      node *right;
      type_code type;  // Assigned during semantic analysis
    } unary;

    struct {
      int op;
      node *left;
      node *right;
      type_code type;  // Assigned during semantic analysis
    } binary;

    struct {
      int func_code;
      node *args;
    } function;

    struct {
      node *type;
      node *args;
    } constructor;

    int int_val;
    float float_val;
    int bool_val;

    struct {
      node *args;
      node *expression;
    } arguments;
    
    struct {
      type_code type;
    } type_node;
    
    node *expression_variable;
    node *nested_expression;
    
    struct {
      char* id;
    } variable;
    
    struct {
      char* id;
      int index;
      type_code type;  // Assigned during semantic analysis
    } array_index;
  };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast, int indentLevel);
const char* getFuncString(int funcCode);
const char* getTypeString(int base, int size);

#endif /* AST_H_ */