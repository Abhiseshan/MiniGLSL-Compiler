%{
/**********************************************************************
 *
 * **YOUR GROUP INFO SHOULD GO HERE**
 *
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "parser.tab.h"
#include "string.h"


#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int CheckInt(void);
int yyline = 1;

%}
%option noyywrap

WS          [ \t]
DIGIT 		[0-9]
APLHA		[a-zA-Z]
SIGN 		[+-]
INTEGER 	SIGN?(0|([1-9]DIGIT*)) 

%%
"/*"        {
              int c1      = 0;
              int c2      = yyinput();
              int curline = yyline;
              for(;;) {
                if (c2 == EOF) {
                  fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: Unmatched /*\n", curline);
                  errorOccurred = TRUE;
                  yyterminate();
                }
                if (c1 == '*' && c2 == '/')
                  break;
                c1 = c2;
                c2 = yyinput();
                if (c1 == '\n' && c2 != EOF)
                  yyline++;
              }
            }

"float"		{
				yTRACE((TOKEN_FLOAT));
				return TOKEN_FLOAT;
			}

"vec2"		{
				yTRACE((TOKEN_VEC2));
				return TOKEN_VEC2;
			}

"vec3"		{
				yTRACE((TOKEN_VEC3));
				return TOKEN_VEC3;
			}

"vec4"		{
				yTRACE((TOKEN_VEC4));
				return TOKEN_VEC4;
			}

"bool"		{
				yTRACE((TOKEN_BOOL));
				return TOKEN_BOOL;
			}

"bvec2"		{
				yTRACE((TOKEN_BVEC2));
				return TOKEN_BVEC2;
			}

"bvec3"		{
				yTRACE((TOKEN_BVEC3));
				return TOKEN_BVEC3;
			}

"bvec4"		{
				yTRACE((TOKEN_BVEC4));
				return TOKEN_BVEC4;
			}

"int"		{
				yTRACE((TOKEN_INT));
				return TOKEN_INT;
			}

"ivec2"		{
				yTRACE((TOKEN_IVEC2));
				return TOKEN_IVEC2;
			}

"ivec3"		{
				yTRACE((TOKEN_IVEC3));
				return TOKEN_IVEC3;
			}

"ivec4"		{
				yTRACE((TOKEN_IVEC4));
				return TOKEN_IVEC4;
			}

"lit"		{
				yTRACE((TOKEN_LIT));
				return TOKEN_LIT;
			}

"dp3"		{
				yTRACE((TOKEN_DP3));
				return TOKEN_DP3;
			}

"rsq"		{
				yTRACE((TOKEN_RSQ));
				return TOKEN_RSQ;
			}

"if"		{
				yTRACE((TOKEN_IF));
				return TOKEN_IF;
			}

"else"		{
				yTRACE((TOKEN_ELSE));
				return TOKEN_ELSE;
			}

"while"		{
				yTRACE((TOKEN_WHILE));
				return TOKEN_WHILE;
			}

"true"		{
				yTRACE((TOKEN_TRUE));
				return TOKEN_TRUE;
			}

"false"		{
				yTRACE((TOKEN_FALSE));
				return TOKEN_FALSE;
			}

"+"			{
				yTRACE((TOKEN_PLUS));
				return TOKEN_PLUS;
			}

"-"			{
				yTRACE((TOKEN_MINUS));
				return TOKEN_MINUS;
			}

"*"			{
				yTRACE((TOKEN_MUL));
				return TOKEN_MUL;
			}

"/"			{
				yTRACE((TOKEN_DIV));
				return TOKEN_DIV;
			}

"^"			{
				yTRACE((TOKEN_EXP));
				return TOKEN_EXP;
			}

"!"			{
				yTRACE((TOKEN_NOT));
				return TOKEN_NOT;
			}

"&&"		{
				yTRACE((TOKEN_AND));
				return TOKEN_AND;
			}

"||"		{
				yTRACE((TOKEN_OR));
				return TOKEN_OR;
			}

"="			{
				yTRACE((TOKEN_ASSIGN));
				return TOKEN_ASSIGN;
			}

"=="		{
				yTRACE((TOKEN_EQUAL));
				return TOKEN_EQUAL;
			}

"!="		{
				yTRACE((TOKEN_NOT_EQUAL));
				return TOKEN_NOT_EQUAL;
			}

"<"			{
				yTRACE((TOKEN_LTHAN));
				return TOKEN_LTHAN;
			}

"<="		{
				yTRACE((TOKEN_LTHAN_EQUAL));
				return TOKEN_LTHAN_EQUAL;
			}

">"			{
				yTRACE((TOKEN_GTHAN));
				return TOKEN_GTHAN;
			}

">="		{
				yTRACE((TOKEN_GTHAN_EQUAL));
				return TOKEN_GTHAN_EQUAL;
			}

"const"		{
				yTRACE((TOKEN_CONST));
				return TOKEN_CONST;
			}

"void"		{
				yTRACE((TOKEN_VOID));
				return TOKEN_VOID;
			}

";"			{
				yTRACE((TOKEN_EOL));
				return TOKEN_EOL;
			}

","			{
				yTRACE((TOKEN_COMMA));
				return TOKEN_COMMA;
			}

"{"			{
				yTRACE((TOKEN_OPEN_SB));
				return TOKEN_OPEN_SB;
			}

"}"			{
				yTRACE((TOKEN_CLOSE_SB));
				return TOKEN_CLOSE_SB;
			}

"("			{
				yTRACE((TOKEN_OPEN_CB));
				return TOKEN_OPEN_CB;
			}

")"			{
				yTRACE((TOKEN_CLOSE_CB));
				return TOKEN_CLOSE_CB;
			}

"["			{
				yTRACE((TOKEN_OPEN_SQB));
				return TOKEN_OPEN_SQB;
			}

"]"			{
				yTRACE((TOKEN_CLOSE_SQB));
				return TOKEN_CLOSE_SQB;
			}			

"\n"		{
				yyline++;
			}

{WS}+ 		{
			}

[a-zA-Z_][a-zA-Z_DIGIT]* {
				if (strlen(yytext) > MAX_IDENTIFIER) {
					yERROR("Exceeded MAX IDENTIFIER size");
				}
				else {
					yTRACE((TOKEN_IDENTIFIER));
					return TOKEN_IDENTIFIER;
				}
			}

[+-]?(0|([1-9][0-9]*)) 	{
				char* pEnd;
				long yyval = strtol(yytext, &pEnd, 10);
				if (yyval > MAX_INTEGER || strlen(yytext) > 10) {
					yERROR("Exceeded MAX INTEGER size");
				}
				else {
					yTRACE((TOKEN_INTEGER_VAL));
					return TOKEN_INTEGER_VAL;
				}
			}

[+-]?(0|([1-9][0-9]*)).[0-9]+ {
					yTRACE((TOKEN_FLOAT_VAL));
					return TOKEN_FLOAT_VAL; 
			}

.           { yERROR("Unknown token"); }

%%
