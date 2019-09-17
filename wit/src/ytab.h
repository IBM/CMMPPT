/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_WITPARSE_TAB_H_INCLUDED
# define YY_YY_WITPARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 77 "../src/witParse.y" /* yacc.c:1909  */

    int    intval;
    double doubleval;
    char * strval;
    bool   boolval;
    int    cval;
    int    pval;
    int    btype;

#line 97 "witParse.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_WITPARSE_TAB_H_INCLUDED  */
