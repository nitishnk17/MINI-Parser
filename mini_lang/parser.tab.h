/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VAR = 258,
     IF = 259,
     ELSE = 260,
     WHILE = 261,
     PLUS = 262,
     MINUS = 263,
     MUL = 264,
     DIV = 265,
     ASSIGN = 266,
     SEMICOLON = 267,
     LPAREN = 268,
     RPAREN = 269,
     LBRACE = 270,
     RBRACE = 271,
     EQ = 272,
     NEQ = 273,
     LT = 274,
     GT = 275,
     LE = 276,
     GE = 277,
     INTEGER = 278,
     IDENTIFIER = 279
   };
#endif
/* Tokens.  */
#define VAR 258
#define IF 259
#define ELSE 260
#define WHILE 261
#define PLUS 262
#define MINUS 263
#define MUL 264
#define DIV 265
#define ASSIGN 266
#define SEMICOLON 267
#define LPAREN 268
#define RPAREN 269
#define LBRACE 270
#define RBRACE 271
#define EQ 272
#define NEQ 273
#define LT 274
#define GT 275
#define LE 276
#define GE 277
#define INTEGER 278
#define IDENTIFIER 279




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 24 "src/parser.y"
{
    int ival;
    char* sval;
    struct Expr* expr;
    struct Stmt* stmt;
}
/* Line 1529 of yacc.c.  */
#line 104 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

