%{
/***********************************************************************
 * --YOUR GROUP INFO SHOULD GO HERE--
 * 
 *   Interface to the parser module for CSC467 course project.
 * 
 *   Phase 2: Implement context free grammar for source language, and
 *            parse tracing functionality.
 *   Phase 3: Construct the AST for the source language program.
 ***********************************************************************/

/***********************************************************************
 *  C Definitions and external declarations for this module.
 *
 *  Phase 3: Include ast.h if needed, and declarations for other global or
 *           external vars, functions etc. as needed.
 ***********************************************************************/

#include <string.h>
#include "common.h"
//#include "ast.h"
//#include "symbol.h"
//#include "semantic.h"
#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(const char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */

%}

/***********************************************************************
 *  Yacc/Bison declarations.
 *  Phase 2:
 *    1. Add precedence declarations for operators (after %start declaration)
 *    2. If necessary, add %type declarations for some nonterminals
 *  Phase 3:
 *    1. Add fields to the union below to facilitate the construction of the
 *       AST (the two existing fields allow the lexical analyzer to pass back
 *       semantic info, so they shouldn't be touched).
 *    2. Add <type> modifiers to appropriate %token declarations (using the
 *       fields of the union) so that semantic information can by passed back
 *       by the scanner.
 *    3. Make the %type declarations for the language non-terminals, utilizing
 *       the fields of the union as well.
 ***********************************************************************/

%{
#define YYDEBUG 1
%}


// TODO:Modify me to add more data types
// Can access me from flex useing yyval

%union {
  int num;
}
// TODO:Replace myToken with your tokens, you can use these tokens in flex
%token TOKEN_BOOL TOKEN_BVEC2 TOKEN_BVEC3 TOKEN_BVEC4 TOKEN_INT TOKEN_IVEC2 TOKEN_IVEC3 TOKEN_IVEC4 TOKEN_FLOAT TOKEN_VEC2 TOKEN_VEC3 TOKEN_VEC4 TOKEN_VARIABLE TOKEN_INTEGER_VAL TOKEN_IDENTIFIER TOKEN_LIT TOKEN_RSQ TOKEN_DP3 TOKEN_IF TOKEN_ELSE TOKEN_WHILE TOKEN_TRUE TOKEN_FALSE TOKEN_PLUS TOKEN_MINUS TOKEN_MUL TOKEN_DIV TOKEN_EXP TOKEN_NOT TOKEN_AND TOKEN_OR TOKEN_EQUAL TOKEN_NOT_EQUAL TOKEN_LTHAN TOKEN_LTHAN_EQUAL TOKEN_GTHAN TOKEN_GTHAN_EQUAL TOKEN_CONST TOKEN_FLOAT_VAL TOKEN_ASSIGN TOKEN_VOID TOKEN_EOL TOKEN_OPEN_SB TOKEN_CLOSE_SB TOKEN_OPEN_CB TOKEN_CLOSE_CB TOKEN_OPEN_SQB TOKEN_CLOSE_SQB TOKEN_COMMA

%start    program

%%

/***********************************************************************
 *  Yacc/Bison rules
 *  Phase 2:
 *    1. Replace grammar found here with something reflecting the source
 *       language grammar
 *    2. Implement the trace parser option of the compiler
 *  Phase 3:
 *    1. Add code to rules for construction of AST.
 ***********************************************************************/
program
  :   tokens       
  ;
tokens
  :  tokens token  
  |      
  ;
// TODO: replace myToken with the token the you defined.
	token
  :      TOKEN_BOOL  
  |      TOKEN_BVEC2
  |      TOKEN_BVEC3
  |      TOKEN_BVEC4
  |      TOKEN_INT
  |      TOKEN_IVEC3
  |      TOKEN_IVEC2
  |      TOKEN_IVEC4
  |      TOKEN_VEC2
  |      TOKEN_FLOAT
  |      TOKEN_VEC4
  |      TOKEN_VEC3
  |      TOKEN_IDENTIFIER
  |      TOKEN_INTEGER_VAL
  |      TOKEN_LIT
  |      TOKEN_VARIABLE
  |      TOKEN_RSQ
  |      TOKEN_DP3
  |      TOKEN_WHILE
  |      TOKEN_ELSE
  |      TOKEN_TRUE
  |      TOKEN_IF
  |      TOKEN_FALSE
  |      TOKEN_PLUS
  |      TOKEN_MINUS
  |      TOKEN_DIV
  |      TOKEN_MUL
  |      TOKEN_EXP
  |      TOKEN_NOT
  |      TOKEN_OR
  |      TOKEN_AND
  |      TOKEN_EQUAL
  |      TOKEN_NOT_EQUAL
  |      TOKEN_LTHAN
  |      TOKEN_LTHAN_EQUAL
  |      TOKEN_GTHAN
  |      TOKEN_GTHAN_EQUAL
  |      TOKEN_CONST
  |      TOKEN_FLOAT_VAL
  |      TOKEN_ASSIGN
  |      TOKEN_EOL
  |      TOKEN_VOID
  |      TOKEN_OPEN_SB
  |      TOKEN_CLOSE_SB
  |      TOKEN_OPEN_CB
  |      TOKEN_CLOSE_CB
  |      TOKEN_CLOSE_SQB
  |      TOKEN_OPEN_SQB
  |      TOKEN_COMMA
  ;


%%

/***********************************************************************ol
 * Extra C code.
 *
 * The given yyerror function should not be touched. You may add helper
 * functions as necessary in subsequent phases.
 ***********************************************************************/
void yyerror(const char* s) {
  if (errorOccurred)
    return;    /* Error has already been reported by scanner */
  else
    errorOccurred = 1;
        
  fprintf(errorFile, "\nPARSER ERROR, LINE %d",yyline);
  if (strcmp(s, "parse error")) {
    if (strncmp(s, "parse error, ", 13))
      fprintf(errorFile, ": %s\n", s);
    else
      fprintf(errorFile, ": %s\n", s+13);
  } else
    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);
}

