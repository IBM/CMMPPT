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
     RELEASE = 258,
     ADD = 259,
     SET = 260,
     READ = 261,
     PROBLEM = 262,
     PART = 263,
     OPERATION = 264,
     BOM_ENTRY = 265,
     SUB_ENTRY = 266,
     BOP_ENTRY = 267,
     DEMAND = 268,
     PART_WITH_OPERATION = 269,
     SINGLE = 270,
     SPARSE = 271,
     DENSE = 272,
     END_BOUNDS = 273,
     HARDLB = 274,
     SOFTLB = 275,
     HARDUB = 276,
     CAPACITY = 277,
     MATERIAL = 278,
     IDENTIFIER = 279,
     STRING = 280,
     DOUBLE = 281,
     INTEGER = 282,
     BOOLEAN = 283,
     CATEGORY = 284,
     EXEC_BOUNDS = 285,
     STOCK_BOUNDS = 286,
     CUM_SHIP_BOUNDS = 287
   };
#endif
/* Tokens.  */
#define RELEASE 258
#define ADD 259
#define SET 260
#define READ 261
#define PROBLEM 262
#define PART 263
#define OPERATION 264
#define BOM_ENTRY 265
#define SUB_ENTRY 266
#define BOP_ENTRY 267
#define DEMAND 268
#define PART_WITH_OPERATION 269
#define SINGLE 270
#define SPARSE 271
#define DENSE 272
#define END_BOUNDS 273
#define HARDLB 274
#define SOFTLB 275
#define HARDUB 276
#define CAPACITY 277
#define MATERIAL 278
#define IDENTIFIER 279
#define STRING 280
#define DOUBLE 281
#define INTEGER 282
#define BOOLEAN 283
#define CATEGORY 284
#define EXEC_BOUNDS 285
#define STOCK_BOUNDS 286
#define CUM_SHIP_BOUNDS 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 85 "../src/witParse.y"
{
    int    intval;
    double doubleval;
    char * strval;
    bool   boolval;
    int    cval;
    int    pval;
    int    btype;
}
/* Line 1489 of yacc.c.  */
#line 123 "witParse.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

