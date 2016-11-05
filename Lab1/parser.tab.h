/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_BOOL = 258,
    TOKEN_BVEC2 = 259,
    TOKEN_BVEC3 = 260,
    TOKEN_BVEC4 = 261,
    TOKEN_INT = 262,
    TOKEN_IVEC2 = 263,
    TOKEN_IVEC3 = 264,
    TOKEN_IVEC4 = 265,
    TOKEN_FLOAT = 266,
    TOKEN_VEC2 = 267,
    TOKEN_VEC3 = 268,
    TOKEN_VEC4 = 269,
    TOKEN_VARIABLE = 270,
    TOKEN_INTEGER_VAL = 271,
    TOKEN_IDENTIFIER = 272,
    TOKEN_LIT = 273,
    TOKEN_RSQ = 274,
    TOKEN_DP3 = 275,
    TOKEN_IF = 276,
    TOKEN_ELSE = 277,
    TOKEN_WHILE = 278,
    TOKEN_TRUE = 279,
    TOKEN_FALSE = 280,
    TOKEN_PLUS = 281,
    TOKEN_MINUS = 282,
    TOKEN_MUL = 283,
    TOKEN_DIV = 284,
    TOKEN_EXP = 285,
    TOKEN_NOT = 286,
    TOKEN_AND = 287,
    TOKEN_OR = 288,
    TOKEN_EQUAL = 289,
    TOKEN_NOT_EQUAL = 290,
    TOKEN_LTHAN = 291,
    TOKEN_LTHAN_EQUAL = 292,
    TOKEN_GTHAN = 293,
    TOKEN_GTHAN_EQUAL = 294,
    TOKEN_CONST = 295,
    TOKEN_FLOAT_VAL = 296,
    TOKEN_ASSIGN = 297,
    TOKEN_VOID = 298,
    TOKEN_EOL = 299,
    TOKEN_OPEN_SB = 300,
    TOKEN_CLOSE_SB = 301,
    TOKEN_OPEN_CB = 302,
    TOKEN_CLOSE_CB = 303,
    TOKEN_OPEN_SQB = 304,
    TOKEN_CLOSE_SQB = 305,
    TOKEN_COMMA = 306
  };
#endif
/* Tokens.  */
#define TOKEN_BOOL 258
#define TOKEN_BVEC2 259
#define TOKEN_BVEC3 260
#define TOKEN_BVEC4 261
#define TOKEN_INT 262
#define TOKEN_IVEC2 263
#define TOKEN_IVEC3 264
#define TOKEN_IVEC4 265
#define TOKEN_FLOAT 266
#define TOKEN_VEC2 267
#define TOKEN_VEC3 268
#define TOKEN_VEC4 269
#define TOKEN_VARIABLE 270
#define TOKEN_INTEGER_VAL 271
#define TOKEN_IDENTIFIER 272
#define TOKEN_LIT 273
#define TOKEN_RSQ 274
#define TOKEN_DP3 275
#define TOKEN_IF 276
#define TOKEN_ELSE 277
#define TOKEN_WHILE 278
#define TOKEN_TRUE 279
#define TOKEN_FALSE 280
#define TOKEN_PLUS 281
#define TOKEN_MINUS 282
#define TOKEN_MUL 283
#define TOKEN_DIV 284
#define TOKEN_EXP 285
#define TOKEN_NOT 286
#define TOKEN_AND 287
#define TOKEN_OR 288
#define TOKEN_EQUAL 289
#define TOKEN_NOT_EQUAL 290
#define TOKEN_LTHAN 291
#define TOKEN_LTHAN_EQUAL 292
#define TOKEN_GTHAN 293
#define TOKEN_GTHAN_EQUAL 294
#define TOKEN_CONST 295
#define TOKEN_FLOAT_VAL 296
#define TOKEN_ASSIGN 297
#define TOKEN_VOID 298
#define TOKEN_EOL 299
#define TOKEN_OPEN_SB 300
#define TOKEN_CLOSE_SB 301
#define TOKEN_OPEN_CB 302
#define TOKEN_CLOSE_CB 303
#define TOKEN_OPEN_SQB 304
#define TOKEN_CLOSE_SQB 305
#define TOKEN_COMMA 306

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 57 "parser.y" /* yacc.c:1909  */

  int num;

#line 160 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
