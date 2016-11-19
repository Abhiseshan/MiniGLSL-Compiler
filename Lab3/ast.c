#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

#define DEBUG_PRINT_TREE 0

node *ast = NULL;

node *ast_allocate(node_kind kind, ...) {
  va_list args;

  node *ast = (node *) malloc(sizeof(node));
  memset(ast, 0, sizeof *ast);
  ast->kind = kind;

  va_start(args, kind); 

  switch(kind) {

  	case SCOPE_NODE:
	  ast->scope.declarations = va_arg(args, node *);
	  ast->scope.stmts = va_arg(args, node *); 
	  break;

  	case DECLARATIONS_NODE:
	  ast->declarations.declarations = va_arg(args, node *);
	  ast->declarations.declaration = va_arg(args, node *);
	  break;

  	case STATEMENTS_NODE:
	  ast->stmts.stmts = va_arg(args, node *); 
	  ast->stmts.stmt = va_arg(args, node *);
	  break;

  	case DECLARATION_NODE:
	  ast->declaration.is_const = va_arg(args, bool);
	  ast->declaration.id = va_arg(args, char *);
	  ast->declaration.type_node = va_arg(args, node *);
	  ast->declaration.expr = va_arg(args, node *); 
	  break;

  	case ASSIGNMENT_NODE:
	  ast->assignment.variable = va_arg(args, node *);
	  ast->assignment.expression = va_arg(args, node *);
	  break;

  	case IF_STATEMENT_NODE:
	  ast->if_stmt.condition_expr = va_arg(args, node *);
	  ast->if_stmt.if_blk_stmt = va_arg(args, node *);
	  ast->if_stmt.else_blk_stmt = va_arg(args, node *);
	  break;

  	case NESTED_SCOPE_NODE:
	  ast->nested_scope = va_arg(args, node *);
	  break;

  	case TYPE_NODE:
	  ast->type.type_code = va_arg(args, int);
	  ast->type.vec = va_arg(args, int) + 1;
	  break;

  	case CONSTRUCTOR_NODE:
	  ast->ctor.type_node = va_arg(args, node *);
	  ast->ctor.args = va_arg(args, node *);
	  break;

  	case FUNCTION_NODE:
	  ast->func.name = va_arg(args, int);
	  ast->func.args = va_arg(args, node *);
	  break;

  	case UNARY_EXPRESION_NODE:
	  ast->unary_expr.op = va_arg(args, int);
	  ast->unary_expr.right = va_arg(args, node *);
	  break;

  	case BOOL_NODE:
	  ast->type.is_const = 1;
	  ast->type.type_code = BOOL_T;
	  ast->type.vec = 1;
	  ast->bool_val = va_arg(args, int);
	  break;

  	case INT_NODE:
	  ast->type.is_const = 1;
	  ast->type.type_code = INT_T;
	  ast->type.vec = 1;
	  ast->int_val = va_arg(args, int);
	  break;

  	case FLOAT_NODE:
	  ast->type.is_const = 1;
	  ast->type.type_code = FLOAT_T;
	  ast->type.vec = 1;
  	  ast->int_val = va_arg(args, int);
  	  break;

  	case NESTED_EXPRESSION_NODE:
	  ast->nested_expr.expr = va_arg(args, node *);
	  break;

  	case EXP_VAR_NODE:
	  ast->exp_var_node.var_node= va_arg(args, node *);
	  break;

  	case VAR_NODE:
	  ast->var_node.id = va_arg(args, char *);
	  ast->var_node.is_array = va_arg(args, int);
	  ast->var_node.index = va_arg(args, int);
	  break;

  	case ARGUMENTS_NODE:
	  ast->args.args = va_arg(args, node *);L
	  ast->args.expr = va_arg(args, node *);
	  break;

  	case BINARY_EXPRESSION_NODE:
    	  ast->binary_expr.op = va_arg(args, int);
          ast->binary_expr.left = va_arg(args, node *);
          ast->binary_expr.right = va_arg(args, node *);
          break;

  	default: break;
  }

  va_end(args);

  return ast;
}

void ast_free(node *ast) {

}

void ast_print(node * ast) {

}

