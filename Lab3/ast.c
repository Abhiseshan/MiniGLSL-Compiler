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
	  ast->scope.statements = va_arg(args, node *); 
	  break;

  	case DECLARATIONS_NODE:
	  ast->declarations.declarations = va_arg(args, node *);
	  ast->declarations.declaration = va_arg(args, node *);
	  break;

  	case DECLARATION_NODE:
	  ast->declaration.is_const = va_arg(args, bool);
	  ast->declaration.id = va_arg(args, char *);
	  ast->declaration.type = va_arg(args, node *);
	  ast->declaration.expression = va_arg(args, node *); 
	  break;

  	case STATEMENTS_NODE:
	  ast->statements.statements = va_arg(args, node *); 
	  ast->statements.statement = va_arg(args, node *);
	  break;

  	case ASSIGNMENT_NODE:
	  ast->assignment.var = va_arg(args, node *);
	  ast->assignment.expression = va_arg(args, node *);
	  break;

  	case IF_STATEMENT_NODE:
	  ast->if_statement.cond = va_arg(args, node *);
	  ast->if_statement.then_statement = va_arg(args, node *);
	  ast->if_statement.else_statement = va_arg(args, node *);
	  break;

  	case NESTED_SCOPE_NODE:
	  ast->nested_scope = va_arg(args, node *);
	  break;

  	case UNARY_EXPRESION_NODE:
	  ast->unary.op = va_arg(args, int);
	  ast->unary.right = va_arg(args, node *);
	  break;

  	case BINARY_EXPRESSION_NODE:
    	  ast->binary.op = va_arg(args, int);
          ast->binary.left = va_arg(args, node *);
          ast->binary.right = va_arg(args, node *);
          break;

  	case FUNCTION_NODE:
	  ast->function.name = va_arg(args, int);
	  ast->function.args = va_arg(args, node *);
	  break;

  	case CONSTRUCTOR_NODE:
	  ast->constructor.type = va_arg(args, node *);
	  ast->constructor.args = va_arg(args, node *);
	  break;
          
  	case ARGUMENTS_NODE:
	  ast->arguments.args = va_arg(args, node *);
	  ast->arguments.expression = va_arg(args, node *);
	  break;

  	case TYPE_NODE:
	  ast->type_node.base = va_arg(args, int);
          ast->type_node.size = va_arg(args, int);
	  break;

  	case VARIABLE_NODE:
	  ast->variable.id = va_arg(args, char *);
	  break;
          
        case ARRAY_INDEX_NODE:
	  ast->array_index.id = va_arg(args, char *);
	  ast->array_index.index = va_arg(args, int);
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

