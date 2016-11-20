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
      
    case ENTER_SCOPE_NODE:
      ast->enter_scope = va_arg(args, node *);
      break;

  	case SCOPE_NODE:
	  ast->scope.declarations = va_arg(args, node *);
	  ast->scope.statements = va_arg(args, node *); 
	  break;
          
    case NESTED_EXPRESSION_NODE:
	  ast->nested_expression = va_arg(args, node *);
	  break;

  	case DECLARATIONS_NODE:
	  ast->declarations.declarations = va_arg(args, node *);
	  ast->declarations.declaration = va_arg(args, node *);
	  break;

  	case DECLARATION_NODE:
	  ast->declaration.is_const = va_arg(args, int);
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
	  ast->function.func_code = va_arg(args, int);
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
	  ast->type_node.base = (type_code) va_arg(args, int); 
      ast->type_node.size = va_arg(args, int);
	  break;

  	case VARIABLE_NODE:
	  ast->variable.id = va_arg(args, char *);
	  break;
          
    case EXPRESSION_VARIABLE_NODE:
	  ast->expression_variable = va_arg(args, node *);
	  break;
          
    case ARRAY_INDEX_NODE:
	  ast->array_index.id = va_arg(args, char *);
	  ast->array_index.index = va_arg(args, int);
	  break;
          
        case INT_NODE:
          ast->int_val = va_arg(args, int);
          break;
        case BOOL_NODE:
          ast->bool_val = va_arg(args, int);
          break;
        case FLOAT_NODE:
          ast->float_val = (float)va_arg(args, double);
          break;

  	default: break;
  }
  
  ast->line_num = va_arg(args, int);
  va_end(args);

  return ast;
}

void ast_free(node *ast) {

}

void printIndent(int indentLevel) {
    while(indentLevel > 0) {
        printf("\t");
        indentLevel--;
    }
}

const char* getOpString(int opCode) {
    switch(opCode) {
        case MINUS_OP:
            return "-";
        case NOT_OP:
            return "!";  
        case AND_OP:
            return "&&";
        case OR_OP:
            return "||";  
        case EQ_OP:
            return "=";
        case NEQ_OP:
            return "!=";  
        case LT_OP:
            return "<";
        case LEQ_OP:
            return "<=";
        case GT_OP:
            return ">";
        case GEQ_OP:
            return ">=";  
        case ADD_OP:
            return "+";
        case SUB_OP:
            return "-";
        case MULT_OP:
            return "*";
        case DIV_OP:
            return "/";  
        case POW_OP:
            return "^";
        default:
            return "";  
    }
}

const char* getFuncString(int funcCode) {
    switch(funcCode) {
        case 0:
            return "DP3";
        case 1:
            return "RSQ";
        case 2:
            return "LIT";
        default:
            return "";
    }
}

char* appendChar(const char* str, char c) {
    char* result = (char*)malloc(strlen(str)+2);
    strcpy(result, str);
    result[strlen(str)] = c;
    return result;
}

const char* getTypeString(int base, int size) {
    switch(base) {
        case INT:
            if(size == 1)
                return "int";
            else
                return appendChar("ivec", size - 1 + '1');
        case FLOAT:
            if(size == 1)
                return "float";
            else
                return appendChar("vec", size - 1 + '1');
        case BOOL:
            if(size == 1)
                return "bool";
            else
                return appendChar("bvec", size - 1 + '1');
        default:
            return "";
    }
}

void ast_print(node * ast, int indentLevel) {
    if(ast == NULL)
        return;
    
    switch(ast->kind) {
        case ENTER_SCOPE_NODE:
            ast_print(ast->enter_scope, indentLevel);
            break;
        case SCOPE_NODE:
            printIndent(indentLevel); printf("(SCOPE\n");
            ast_print(ast->scope.declarations, indentLevel + 1);
            ast_print(ast->scope.statements, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case NESTED_EXPRESSION_NODE:
            ast_print(ast->nested_expression, indentLevel);
            break;
        case DECLARATIONS_NODE:
            printIndent(indentLevel); printf("(DECLARATIONS\n");
            ast_print(ast->declarations.declarations, indentLevel + 1);
            ast_print(ast->declarations.declaration, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case STATEMENTS_NODE:
            printIndent(indentLevel); printf("(STATEMENTS\n");
            ast_print(ast->statements.statements, indentLevel + 1);
            ast_print(ast->statements.statement, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case ASSIGNMENT_NODE:
            printIndent(indentLevel); printf("(ASSIGN\n");
            printIndent(indentLevel + 1); printf("type placeholder"); printf("\n");
            ast_print(ast->assignment.var, indentLevel + 1);
            ast_print(ast->assignment.expression, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case IF_STATEMENT_NODE:
            printIndent(indentLevel); printf("(IF\n");
            ast_print(ast->if_statement.cond, indentLevel + 1);
            ast_print(ast->if_statement.then_statement, indentLevel + 1);
            ast_print(ast->if_statement.else_statement, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case NESTED_SCOPE_NODE:
            ast_print(ast->nested_scope, indentLevel);
            break;
        case UNARY_EXPRESION_NODE:
            printIndent(indentLevel); printf("(UNARY\n");
            printIndent(indentLevel + 1); printf("type placeholder"); printf("\n");
            printIndent(indentLevel + 1); printf("%s", getOpString(ast->unary.op)); printf("\n");
            ast_print(ast->unary.right, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case BINARY_EXPRESSION_NODE:
            printIndent(indentLevel); printf("(BINARY\n");
            printIndent(indentLevel + 1); printf("type placeholder"); printf("\n");
            printIndent(indentLevel + 1); printf("%s",getOpString(ast->binary.op)); printf("\n");
            ast_print(ast->binary.left, indentLevel + 1);
            ast_print(ast->binary.right, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case FUNCTION_NODE:
            printIndent(indentLevel); printf("(CALL\n");
            printIndent(indentLevel + 1); printf("%s", getFuncString(ast->function.func_code)); printf("\n");
            ast_print(ast->function.args, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case CONSTRUCTOR_NODE:
            printIndent(indentLevel); printf("(CALL\n");
            ast_print(ast->constructor.type, indentLevel + 1);
            ast_print(ast->constructor.args, indentLevel + 1);
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        case INT_NODE:
            printIndent(indentLevel); printf("%d\n", ast->int_val);
            break;
        case BOOL_NODE:
            printIndent(indentLevel); ast->bool_val ? printf("true\n") : printf("false\n");
            break;
        case FLOAT_NODE:
            printIndent(indentLevel); printf("%f\n", ast->float_val);
            break;
        case ARGUMENTS_NODE:
            ast_print(ast->arguments.args, indentLevel + 1);
            ast_print(ast->arguments.expression, indentLevel + 1);
            break;
        case TYPE_NODE:
            printIndent(indentLevel); printf("%s", getTypeString(ast->type_node.base, ast->type_node.size)); printf("\n");
            break;
        case EXPRESSION_VARIABLE_NODE:
            ast_print(ast->expression_variable, indentLevel);
            break;
        case VARIABLE_NODE:
            printIndent(indentLevel); printf("%s", ast->variable.id); printf("\n");
            break;
        case ARRAY_INDEX_NODE:
            printIndent(indentLevel); printf("(INDEX\n");
            printIndent(indentLevel + 1); printf("type placeholder"); printf("\n");
            printIndent(indentLevel); printf("%s", ast->array_index.id); printf("\n");
            printIndent(indentLevel); printf("%d", ast->array_index.index); printf("\n");
            printf("\n"); printIndent(indentLevel); printf(")\n");
            break;
        //Humm eric look at these
        case DECLARATION_NODE:
            break;
        case UNKNOWN:
            break;
    }
}

