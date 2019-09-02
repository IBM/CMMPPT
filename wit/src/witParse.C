/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "../src/witParse.y"

//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

/*------------------------------------------------------------------------------
 *
 *     Source file:    witParse.y
 *
 *     Parser for WIT 4.0 Input Data File Format
 *
 *     To create an executable parser file:
 *
 *           yacc -d  witParse.y             (creates y.tab.c and y.tab.h)
 *           mv -f y.tab.c witParse.C
 *           mv -f y.tab.h ytab.h
 *           lex  witLexer.l                 (creates lex.yy.c)    
 *           mv -f lex.yy.c witLexer.C
 *           xlC witLexer.C witParse.C -o witParse -ly -ll
 *    
 *           Notes:
 *           
 *              -d   causes y.tab.h to be created.  This is the token
 *                   name definition file.
 *
 *              -ly  causes the yacc library to be included. This 
 *                   library is in /usr/lib/liby.a
 *
 *              -ll  causes the lex library to be included. This
 *                   library is in /usr/lib/libl.a      
 *
 *------------------------------------------------------------------------------
 *
 */

#include <Parser.h>
#include <Problem.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <VecBS.h>
#include <Entity.h>
#include <MsgFac.h>

#include <errno.h>

extern  FILE *yyin;

//#if defined(__GNUC__)
extern int yylex (void);
extern void yyrestart (FILE *);
//#endif

extern "C" void yyerror(const char *)
{
}

extern char yytext[];

void prterror();                     /* print line number and filename */ 

    /* variables used by the parser */
//RWCollectableString   *rulePtr;
WitSymTabEntry        *entryPtr;     /* ptr to current symbol table entry */
WitEntity::argVecDef  *argPtr;
WitVectorSpec         *vecPtr;       /* pointer to current vector format */
WitBoundSetSpec       *bndPtr;
WitParseRC            retcode;
WitEntity::eType      entityType;    /* type of the current entity */
WitAttribute::aType   attrType;      /* type of the current attribute */  
WitParser::PS         commandState;  /* current command state */
const char            *s1, *s2, *s3; /* temporary strings     */



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 187 of yacc.c.  */
#line 254 "witParse.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 267 "witParse.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   78

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNRULES -- Number of states.  */
#define YYNSTATES  111

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    36,    33,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    12,    14,    16,    18,
      19,    20,    26,    28,    29,    30,    37,    39,    41,    42,
      48,    49,    55,    57,    59,    61,    63,    65,    67,    69,
      71,    72,    76,    78,    79,    82,    84,    87,    89,    91,
      93,    95,    98,   100,   102,   104,   106,   108,   110,   112,
     113,   114,   115,   123,   124,   125,   132,   133,   134,   141,
     142,   145,   146,   151,   153,   156,   158,   160,   161,   166,
     168,   170,   172,   173,   176,   177,   181,   183,   185,   187,
     188,   189,   190,   198
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      38,     0,    -1,    39,    -1,    41,    39,    -1,    -1,    39,
      40,    -1,    45,    -1,    49,    -1,    87,    -1,    -1,    -1,
       3,    42,    44,    43,    33,    -1,    25,    -1,    -1,    -1,
       4,    46,    48,    47,    55,    33,    -1,    53,    -1,    14,
      -1,    -1,     5,     7,    50,    54,    33,    -1,    -1,     5,
      51,    52,    55,    33,    -1,    53,    -1,     8,    -1,     9,
      -1,    10,    -1,    11,    -1,    12,    -1,    13,    -1,    58,
      -1,    -1,    59,    56,    58,    -1,    29,    -1,    -1,    58,
      61,    -1,    60,    -1,    59,    60,    -1,    65,    -1,    57,
      -1,    62,    -1,    80,    -1,    63,    64,    -1,    24,    -1,
      65,    -1,    66,    -1,    25,    -1,    27,    -1,    26,    -1,
      28,    -1,    -1,    -1,    -1,    15,    67,    34,    68,    78,
      69,    35,    -1,    -1,    -1,    16,    70,    34,    71,    74,
      35,    -1,    -1,    -1,    17,    72,    34,    73,    77,    35,
      -1,    -1,    74,    75,    -1,    -1,    79,    36,    76,    78,
      -1,    78,    -1,    77,    78,    -1,    65,    -1,    27,    -1,
      -1,    82,    81,    83,    18,    -1,    30,    -1,    31,    -1,
      32,    -1,    -1,    83,    84,    -1,    -1,    86,    85,    66,
      -1,    19,    -1,    20,    -1,    21,    -1,    -1,    -1,    -1,
       6,    88,    91,    89,    33,    90,    41,    -1,    25,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   132,   132,   133,   136,   137,   140,   149,   158,   170,
     173,   169,   184,   188,   195,   187,   226,   238,   257,   256,
     279,   278,   291,   327,   334,   341,   348,   355,   362,   370,
     374,   373,   436,   443,   444,   492,   493,   496,   553,   585,
     587,   590,   593,   631,   683,   687,   690,   693,   696,   701,
     726,   731,   700,   741,   765,   740,   776,   799,   775,   810,
     811,   819,   817,   830,   831,   834,   935,   952,   951,   976,
    1003,  1030,  1057,  1058,  1067,  1066,  1089,  1100,  1111,  1124,
    1132,  1137,  1123,  1174
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "RELEASE", "ADD", "SET", "READ",
  "PROBLEM", "PART", "OPERATION", "BOM_ENTRY", "SUB_ENTRY", "BOP_ENTRY",
  "DEMAND", "PART_WITH_OPERATION", "SINGLE", "SPARSE", "DENSE",
  "END_BOUNDS", "HARDLB", "SOFTLB", "HARDUB", "CAPACITY", "MATERIAL",
  "IDENTIFIER", "STRING", "DOUBLE", "INTEGER", "BOOLEAN", "CATEGORY",
  "EXEC_BOUNDS", "STOCK_BOUNDS", "CUM_SHIP_BOUNDS", "';'", "'('", "')'",
  "':'", "$accept", "data_specification", "command_list", "command",
  "release_specification", "@1", "@2", "release_num", "add_command", "@3",
  "@4", "add_clause", "set_command", "@5", "@6", "set_clause",
  "entity_name", "global_specification", "entity_specification", "@7",
  "category", "attribute_list", "argument_list", "argument", "attribute",
  "simple_attribute", "attribute_name", "attribute_value", "simple_value",
  "vector_format", "@8", "@9", "@10", "@11", "@12", "@13", "@14",
  "sparse_list", "period_value", "@15", "value_list", "vector_value",
  "period", "bound_set_attribute", "@16", "bound_set_type",
  "bounds_specification", "bound_item", "@17", "bound_type",
  "read_command", "@18", "@19", "@20", "file_name", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    59,    40,    41,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    38,    38,    39,    39,    40,    40,    40,    42,
      43,    41,    44,    46,    47,    45,    48,    48,    50,    49,
      51,    49,    52,    53,    53,    53,    53,    53,    53,    54,
      56,    55,    57,    58,    58,    59,    59,    60,    60,    61,
      61,    62,    63,    64,    64,    65,    65,    65,    65,    67,
      68,    69,    66,    70,    71,    66,    72,    73,    66,    74,
      74,    76,    75,    77,    77,    78,    79,    81,    80,    82,
      82,    82,    83,    83,    85,    84,    86,    86,    86,    88,
      89,    90,    87,    91
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     1,     1,     0,
       0,     5,     1,     0,     0,     6,     1,     1,     0,     5,
       0,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     1,     0,     2,     1,     2,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     0,
       2,     0,     4,     1,     2,     1,     1,     0,     4,     1,
       1,     1,     0,     2,     0,     3,     1,     1,     1,     0,
       0,     0,     7,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     9,     0,     2,     4,     0,     1,    13,    20,    79,
       5,     6,     7,     8,     3,    12,    10,     0,    18,     0,
       0,     0,    23,    24,    25,    26,    27,    28,    17,    14,
      16,    33,     0,    22,    83,    80,    11,     0,     0,    29,
      45,    47,    46,    48,    32,     0,    38,    30,    35,    37,
       0,     0,    19,    42,    69,    70,    71,    34,    39,     0,
      40,    67,    21,    33,    36,    81,    15,    49,    53,    56,
      41,    43,    44,    72,    31,     0,     0,     0,     0,     0,
      82,    50,    54,    57,    68,    76,    77,    78,    73,    74,
       0,    59,     0,     0,    65,    51,     0,     0,    63,    75,
       0,    66,    55,    60,     0,    58,    64,    52,    61,     0,
      62
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,    10,     4,     5,    21,    16,    11,    17,
      37,    29,    12,    31,    19,    32,    30,    38,    45,    63,
      46,    39,    47,    48,    57,    58,    59,    70,    94,    72,
      76,    90,   100,    77,    91,    78,    92,    96,   103,   109,
      97,    95,   104,    60,    73,    61,    79,    88,    93,    89,
      13,    20,    50,    75,    35
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -92
static const yytype_int8 yypact[] =
{
      -1,   -92,     3,    55,   -92,   -18,   -92,   -92,     1,   -92,
     -92,   -92,   -92,   -92,    55,   -92,   -92,    29,   -92,    36,
     -12,   -19,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,
     -92,   -92,    25,   -92,   -92,   -92,   -92,    25,   -17,     4,
     -92,   -92,   -92,   -92,   -92,   -16,   -92,    25,   -92,   -92,
      32,    33,   -92,   -92,   -92,   -92,   -92,   -92,   -92,    -6,
     -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,
     -92,   -92,   -92,   -92,     4,    -1,    34,    35,    37,    11,
     -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,
      30,   -92,    30,    47,   -92,   -92,   -23,    -2,   -92,   -92,
      38,   -92,   -92,   -92,    31,   -92,   -92,   -92,   -92,    30,
     -92
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -92,   -92,    66,   -92,    -3,   -92,   -92,   -92,   -92,   -92,
     -92,   -92,   -92,   -92,   -92,   -92,    56,   -92,    39,   -92,
     -92,    14,   -92,    27,   -92,   -92,   -92,   -92,   -32,   -15,
     -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,
     -92,   -91,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,
     -92,   -92,   -92,   -92,   -92
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      49,    98,     1,     6,   101,    49,   106,    15,    18,    67,
      68,    69,   102,    34,    36,    49,    52,    62,   110,    40,
      41,    42,    43,    40,    41,    42,    43,    71,    53,    84,
      85,    86,    87,   105,    54,    55,    56,    22,    23,    24,
      25,    26,    27,    28,    22,    23,    24,    25,    26,    27,
      40,    41,    42,    43,    44,    40,    41,    42,    43,     7,
       8,     9,    67,    68,    69,    65,    66,   108,    81,    82,
      14,    83,    80,   107,    64,    33,    51,    74,    99
};

static const yytype_uint8 yycheck[] =
{
      32,    92,     3,     0,    27,    37,    97,    25,     7,    15,
      16,    17,    35,    25,    33,    47,    33,    33,   109,    25,
      26,    27,    28,    25,    26,    27,    28,    59,    24,    18,
      19,    20,    21,    35,    30,    31,    32,     8,     9,    10,
      11,    12,    13,    14,     8,     9,    10,    11,    12,    13,
      25,    26,    27,    28,    29,    25,    26,    27,    28,     4,
       5,     6,    15,    16,    17,    33,    33,    36,    34,    34,
       4,    34,    75,    35,    47,    19,    37,    63,    93
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    38,    39,    41,    42,     0,     4,     5,     6,
      40,    45,    49,    87,    39,    25,    44,    46,     7,    51,
      88,    43,     8,     9,    10,    11,    12,    13,    14,    48,
      53,    50,    52,    53,    25,    91,    33,    47,    54,    58,
      25,    26,    27,    28,    29,    55,    57,    59,    60,    65,
      89,    55,    33,    24,    30,    31,    32,    61,    62,    63,
      80,    82,    33,    56,    60,    33,    33,    15,    16,    17,
      64,    65,    66,    81,    58,    90,    67,    70,    72,    83,
      41,    34,    34,    34,    18,    19,    20,    21,    84,    86,
      68,    71,    73,    85,    65,    78,    74,    77,    78,    66,
      69,    27,    35,    75,    79,    35,    78,    35,    36,    76,
      78
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:
#line 141 "../src/witParse.y"
    {
      delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    ;}
    break;

  case 7:
#line 150 "../src/witParse.y"
    { 
      delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    ;}
    break;

  case 8:
#line 159 "../src/witParse.y"
    {
      if (WitParser::instance ()->currentEntity_ != NULL)
        delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    ;}
    break;

  case 9:
#line 170 "../src/witParse.y"
    { WitParser::instance ()->setParseState(WitParser::PS_release); ;}
    break;

  case 10:
#line 173 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    ;}
    break;

  case 11:
#line 178 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
      free (yylval.strval);     
    ;}
    break;

  case 13:
#line 188 "../src/witParse.y"
    { 
      WitParser::instance ()->setParseState(WitParser::PS_add);
      commandState = WitParser::PS_add;     /* save command state */
      WitParser::instance ()->currentRule_ = "add";
    ;}
    break;

  case 14:
#line 195 "../src/witParse.y"
    {
      /* lookup rule for this entity and save symbol table pointer */
      entryPtr = WitParser::instance ()->currentEntity_->
        lookupRule(
           WitParser::instance ()->theSymTable_,
           WitParser::instance ()->currentRule_);

      /* Issue error message if no entry found in symbol table */
      if (entryPtr == NULL) {
        switch (commandState) {
          case WitParser::PS_add:
            WitParser::instance ()->myMsgFac () ("noParamAddMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
          case WitParser::PS_set:
            WitParser::instance ()->myMsgFac () ("noParamSetMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
        };
        WitParser::instance ()->currentEntity_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
    ;}
    break;

  case 15:
#line 221 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    ;}
    break;

  case 16:
#line 227 "../src/witParse.y"
    {
      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            entityType);
      free (yylval.strval); 
      WitParser::instance ()->currentAttr_ = NULL;
    ;}
    break;

  case 17:
#line 239 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_partOperation);
      WitParser::instance ()->currentRule_ = "add_partWithOperation";

      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitEntity::partWithOperation);

      free (yylval.strval);
      WitParser::instance ()->currentAttr_ = NULL;
      entityType = WitEntity::partWithOperation;
    ;}
    break;

  case 18:
#line 257 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_setProblem);
      commandState = WitParser::PS_set;     /* save command state */

      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitEntity::problem);
      free (yylval.strval);
      WitParser::instance ()->currentAttr_ = NULL;

      WitParser::instance ()->currentRule_ = "set_problem";
    ;}
    break;

  case 19:
#line 274 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    ;}
    break;

  case 20:
#line 279 "../src/witParse.y"
    { 
      WitParser::instance ()->setParseState(WitParser::PS_set);
      commandState = WitParser::PS_set;     /* save command state */
      WitParser::instance ()->currentRule_ = "set";
    ;}
    break;

  case 21:
#line 286 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    ;}
    break;

  case 22:
#line 292 "../src/witParse.y"
    {
      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            entityType);
      free (yylval.strval);
      WitParser::instance ()->currentAttr_ = NULL;

      /* lookup rule for this entity and save symbol table pointer */
      entryPtr = WitParser::instance ()->currentEntity_->
        lookupRule (
           WitParser::instance ()->theSymTable_,
           WitParser::instance ()->currentRule_);

      /* Issue error message if no entry found in symbol table */
      if (entryPtr == NULL) {
        switch (commandState) {
          case WitParser::PS_add:
            WitParser::instance ()->myMsgFac () ("noParamAddMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
          case WitParser::PS_set:
            WitParser::instance ()->myMsgFac () ("noParamSetMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
        };
        WitParser::instance ()->currentEntity_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
    ;}
    break;

  case 23:
#line 328 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_part);
      WitParser::instance ()->currentRule_ += "_part";
      entityType = WitEntity::part;
    ;}
    break;

  case 24:
#line 335 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_operation);
      WitParser::instance ()->currentRule_ += "_operation";
      entityType = WitEntity::operation;
    ;}
    break;

  case 25:
#line 342 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_bomEntry);
      WitParser::instance ()->currentRule_ += "_bomEntry";
      entityType = WitEntity::bomEntry;
    ;}
    break;

  case 26:
#line 349 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_subEntry);
      WitParser::instance ()->currentRule_ += "_subEntry";
      entityType = WitEntity::subEntry;
    ;}
    break;

  case 27:
#line 356 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_bopEntry);
      WitParser::instance ()->currentRule_ += "_bopEntry";
      entityType = WitEntity::bopEntry;
    ;}
    break;

  case 28:
#line 363 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_demand);
      WitParser::instance ()->currentRule_ += "_demand";
      entityType = WitEntity::demand;
    ;}
    break;

  case 30:
#line 374 "../src/witParse.y"
    {
      /*------------------------------------------------------*
       * we have created the entity object and the argument   *
       * vector; we also know the current rule; if there is   *
       * an entry for the rule, then invoke the function.     *
       *------------------------------------------------------*/

      if (entryPtr != NULL) {
        /* before invoking the function, check the validity of the arguments */
        /* by comparing the arg. types against the types in the symbol table */

        retcode = WitParser::instance ()->currentEntity_->checkArgs(entryPtr);
        if (retcode == BAD_NUM_ARGS) {
          if (entryPtr->nparms() == 1)
            s2 = "argument";
          else s2 = "arguments";
          if (WitParser::instance ()->currentEntity()->argc() == 1)
            s3 = "was found";
          else s3 = "were found";

          s1 = "";
          if (commandState == WitParser::PS_add)
            s1 = "add "; 
          if (commandState == WitParser::PS_set)
            s1 = "set ";

          WitParser::instance ()->myMsgFac () ("incorrectNumberArgMsg",
             s1,
             WitParser::instance ()->currentEntity_->entityID(),
             entryPtr->nparms(),
             s2,
             WitParser::instance ()->currentEntity()->argc(),
             s3);

          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
        else
        if (retcode) {
          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }

        else                                    /* if arguments O.K.  */
        if (entryPtr->func() != NULL) {         /* if function exists */

          /* if entity has been invalidated because of an error,      */
          /* then, skip invocations of API functions for the entity   */
          if (!WitParser::instance ()->currentEntity_->invalid())
            (entryPtr->func())();               /* invoke function */
        }
      };  

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
    ;}
    break;

  case 32:
#line 437 "../src/witParse.y"
    { 
      if (WitParser::instance ()->parseState() == WitParser::PS_part)
      (yyval.cval) = (yyvsp[(1) - (1)].cval);
    ;}
    break;

  case 34:
#line 445 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextAttr);

      /* If an entry exists for the current rule, then invoke   */
      /* the function.                                          */

      if (entryPtr == NULL) {
        WitParser::instance ()->myMsgFac () ("noAttributeDefMsg",
          WitParser::instance ()->currentAttr_->attrID(),
          WitParser::instance ()->currentEntity_->entityID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
      else
      {
        retcode = WitParser::instance ()->currentAttr_->checkArgs(entryPtr);
        if (retcode == PERIOD_ERROR) {
           WitParser::instance ()->currentAttr_->setInvalid(true);
           prterror();
        }            
        else
        if (retcode) {
          WitParser::instance ()->currentAttr_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
        else 
        if (entryPtr->func() != NULL) {        /* if function exists  */

          /* if entity or attr has been invalidated because of error, */
          /* then, skip invocations of API function for attributes    */
          if (!WitParser::instance ()->currentEntity_->invalid() and
            !WitParser::instance ()->currentAttr_->invalid())
            retcode = (entryPtr->func())();    /* invoke wrapper function */
            if (retcode) {
              WitParser::instance ()->currentAttr_->setInvalid(true);
              prterror();
            }; 
        } 
      };                   

      WitParser::instance ()->currentRule_ =
         WitParser::instance ()->previousRule_;
    ;}
    break;

  case 37:
#line 497 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextArg);
      switch((yyvsp[(1) - (1)].pval)) {
        case WitDRParmType::intScalar:
          argPtr =
             WitParser::instance ()->currentEntity_->setIntArg(yylval.intval);
          break;
        case WitDRParmType::doubleScalar:
          argPtr =
             WitParser::instance ()->currentEntity_->
                setDoubleArg(yylval.doubleval);
          break;
        case WitDRParmType::boolScalar:
          argPtr = WitParser::instance ()->currentEntity_->
            setBoolArg(yylval.boolval);
          break;
        case WitDRParmType::string:
          argPtr =
             WitParser::instance ()->currentEntity_->
                setStringArg(yylval.strval);

          /* string was created by the lexer and it's no longer needed */
          free (yylval.strval);
          break;
        default:
          return 0;
      };
      if (argPtr == NULL) {
        s1 = "";
        if (commandState == WitParser::PS_add)
          s1 = "add ";
        if (commandState == WitParser::PS_set)
          s1 = "set ";

        /* if rule is not defined in symbol table, skip message now. */
        /* However, error message will be issued after all arguments */
        /* are processed.  We don't know the correct number of args. */

        if (entryPtr != NULL) {
          if (entryPtr->nparms() == 1)
            s2 = "argument is";
          else s2 = "arguments are";

          WitParser::instance ()->myMsgFac () ("tooManyArgsMsg",
             s1,
             WitParser::instance ()->currentEntity_->entityID(),
             entryPtr->nparms(),
             s2);

          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
      }
    ;}
    break;

  case 38:
#line 554 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextArg);
      argPtr = WitParser::instance ()->currentEntity_->
        setCatArg (static_cast <WitDRPartCat::Tag> (yylval.cval));
      if (argPtr == NULL) {
        s1 = "";
        if (commandState == WitParser::PS_add)
          s1 = "add ";
        if (commandState == WitParser::PS_set)
          s1 = "set ";

        /* If rule is not defined, skip message. See comment above */
        if (entryPtr != NULL) {
          if (entryPtr->nparms() == 1)
            s2 = "argument is";
          else s2 = "arguments are";

          WitParser::instance ()->myMsgFac () ("tooManyArgsMsg",
             s1,
             WitParser::instance ()->currentEntity_->entityID(),
             entryPtr->nparms(),
             s2);

          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
     }
    ;}
    break;

  case 42:
#line 594 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_attr);
      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");

      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (WitParser::instance ()->myProblem (), yylval.strval);

      free (yylval.strval);

      /* lookup rule for this attribute and save symbol table pointer */
      /* this will be helpful to check the datatype of a vector       */
      entryPtr = WitParser::instance ()->currentAttr_->
        lookupRule(WitParser::instance ()->theSymTable_,
           WitParser::instance ()->currentRule_);

      /* if attribute name is not in the symbol table, we can't       */
      /* continue because we don't know the expected argument types   */
      if (entryPtr == NULL) {
        WitParser::instance ()->myMsgFac () ("noAttributeDefMsg",
          WitParser::instance ()->currentAttr_->attrID(),
          WitParser::instance ()->currentEntity_->entityID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }

      (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 43:
#line 632 "../src/witParse.y"
    {
      switch((yyvsp[(1) - (1)].pval)) {
        case WitDRParmType::intScalar:
          argPtr =
             WitParser::instance ()->currentAttr_->setIntArg(yylval.intval);
          break;
        case WitDRParmType::doubleScalar:
          argPtr =
             WitParser::instance ()->currentAttr_->setDoubleArg(
                yylval.doubleval);
          break;
        case WitDRParmType::boolScalar:
          argPtr = WitParser::instance ()->currentAttr_->
            setBoolArg(yylval.boolval);
          break;
        case WitDRParmType::string:
          argPtr =
             WitParser::instance ()->currentAttr_->setStringArg(yylval.strval);

          /* string was created by the lexer and it's no longer needed */
          free (yylval.strval);
          break;
        default:
          return 0;
      };

      if (argPtr == NULL) {
        s1 = "";
        if (commandState == WitParser::PS_add)
          s1 = "add ";
        if (commandState == WitParser::PS_set)
          s1 = "set ";

        if (entryPtr != NULL) {
          if (entryPtr->nparms() == 1)
            s2 = "argument is";
          else s2 = "arguments are";

          WitParser::instance ()->myMsgFac () ("tooManyArgsMsg",
             s1,
             WitParser::instance ()->currentAttr_->attrID(),
             entryPtr->nparms(),
             s2);

          WitParser::instance ()->currentAttr_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
      }
    ;}
    break;

  case 44:
#line 684 "../src/witParse.y"
    { (yyval.pval) = WitDRParmType::undefined; ;}
    break;

  case 45:
#line 688 "../src/witParse.y"
    { (yyval.pval) = WitDRParmType::string; ;}
    break;

  case 46:
#line 691 "../src/witParse.y"
    { (yyval.pval) = WitDRParmType::intScalar; ;}
    break;

  case 47:
#line 694 "../src/witParse.y"
    { (yyval.pval) = WitDRParmType::doubleScalar; ;}
    break;

  case 48:
#line 697 "../src/witParse.y"
    { (yyval.pval) = WitDRParmType::boolScalar; ;}
    break;

  case 49:
#line 701 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_single);

      // Construct a VectorSpec object
      if (WitParser::instance ()->currentAttr_->attrType() ==
          WitAttribute::simple) {
        if (entryPtr != NULL) {
          vecPtr =
             new WitVectorSpec (
                WitParser::instance ()->myProblem (),
                WitVectorSpec::single_,
                entryPtr->parmTypes()[0]);

          argPtr = WitParser::instance ()->currentAttr_->setVectorArg(vecPtr);
        }
      }
      else 
        vecPtr =
           new WitVectorSpec (
              WitParser::instance ()->myProblem (),
              WitVectorSpec::single_,
              WitDRParmType::doubleVector);
    ;}
    break;

  case 50:
#line 726 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_singleParen);
    ;}
    break;

  case 51:
#line 731 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_singleValue);
    ;}
    break;

  case 52:
#line 736 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    ;}
    break;

  case 53:
#line 741 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparse);

      /* Construct a VectorSpec object */
      if (WitParser::instance ()->currentAttr_->attrType() ==
          WitAttribute::simple) {
        if (entryPtr != NULL) {
          vecPtr =
             new WitVectorSpec (
                WitParser::instance ()->myProblem (),
                WitVectorSpec::sparse_,
                entryPtr->parmTypes()[0]);
          argPtr = WitParser::instance ()->currentAttr_->setVectorArg(vecPtr);
        }
      }
      else
         vecPtr =
            new WitVectorSpec (
               WitParser::instance ()->myProblem (),
               WitVectorSpec::sparse_,
               WitDRParmType::doubleVector);
    ;}
    break;

  case 54:
#line 765 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparseParen);
    ;}
    break;

  case 55:
#line 771 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    ;}
    break;

  case 56:
#line 776 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_dense);

      // Construct a VectorSpec object
      if (WitParser::instance ()->currentAttr_->attrType() ==
          WitAttribute::simple) {
        if (entryPtr != NULL) {
          vecPtr =
             new WitVectorSpec (WitParser::instance ()->myProblem (),
                WitVectorSpec::dense_,
                entryPtr->parmTypes()[0]);
          argPtr = WitParser::instance ()->currentAttr_->setVectorArg(vecPtr);
        }
      }
      else
         vecPtr =
            new WitVectorSpec (
               WitParser::instance ()->myProblem (),
               WitVectorSpec::dense_,
               WitDRParmType::doubleVector);
    ;}
    break;

  case 57:
#line 799 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_denseParen);
    ;}
    break;

  case 58:
#line 805 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    ;}
    break;

  case 60:
#line 812 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparseValue);
    ;}
    break;

  case 61:
#line 819 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_periodColon);
    ;}
    break;

  case 62:
#line 824 "../src/witParse.y"
    { 
      WitParser::instance ()->setParseState(WitParser::PS_sparseParen);
      (yyval.pval) = (yyvsp[(4) - (4)].pval); 
    ;}
    break;

  case 65:
#line 835 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_denseValue);

      if (entryPtr != NULL) {
      switch((yyvsp[(1) - (1)].pval)) {

        case WitDRParmType::intScalar:
          if (entryPtr->parmTypes()[0] == WitDRParmType::doubleVector) {
            retcode =
               vecPtr->addDoubleVal(static_cast <double> (yylval.intval));

            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::intVector) {
            retcode = vecPtr->addIntVal(yylval.intval);
            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::boundSet) {
            retcode =
               vecPtr->addDoubleVal(static_cast <double> (yylval.intval));
            break;
          }
          else {
            retcode = WRONG_TYPE;
            break;
          };
          
        case WitDRParmType::doubleScalar:
         if (entryPtr->parmTypes()[0] == WitDRParmType::intVector) {
            vecPtr->addIntVal(static_cast <int> (yylval.doubleval));
            retcode = SHOULD_BE_INT;
            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::doubleVector) {
            retcode = vecPtr->addDoubleVal(yylval.doubleval);
            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::boundSet) {
            retcode = vecPtr->addDoubleVal(yylval.doubleval);
            break;
          }
          else {
            retcode = WRONG_TYPE;
            break;
          };

        case WitDRParmType::boolScalar:
           {
           if (entryPtr->parmTypes()[0] == WitDRParmType::boolVector)
              {
              retcode = vecPtr->addBoolVal (yylval.boolval);

              break;
              };

           WitParser::instance ()->myMsgFac () ("booleanVectorMsg",
              WitParser::instance ()->currentAttr_->attrID ());

           WitParser::instance ()->currentAttr_->setInvalid (true);

           prterror ();

           retcode = 0;

           WitParser::instance ()->myMsgFac () ("parsingStopsMsg");

           break;
           }

        case WitDRParmType::string:
          WitParser::instance ()->myMsgFac () ("stringVectorMsg",
             WitParser::instance ()->currentAttr_->attrID());
          WitParser::instance ()->currentAttr_->setInvalid(true);
          prterror();
          free (yylval.strval);
          retcode = 0;
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
          break;
      }

      if (retcode == WRONG_TYPE) {
        WitParser::instance ()->myMsgFac () ("vecTypeNotValidMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        retcode = 0;
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
      if (retcode == SHOULD_BE_INT) {
        WitParser::instance ()->myMsgFac () ("vecValueNotIntMsg",
           WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        retcode = 0;
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
      }
    ;}
    break;

  case 66:
#line 936 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_period);

      retcode = vecPtr->addPeriodVal(yylval.intval);
      if (retcode == PERIOD_ERROR) {
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      }
      if (retcode == PERIOD_DUP) {
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      }
    ;}
    break;

  case 67:
#line 952 "../src/witParse.y"
    {  
      /* lookup rule for this attribute and save symbol table pointer */
      entryPtr = WitParser::instance ()->currentAttr_->
         lookupRule (
            WitParser::instance ()->theSymTable_,
            WitParser::instance ()->currentRule_);

      if (entryPtr == NULL) {
        WitParser::instance ()->myMsgFac () ("noAttributeDefMsg",
          WitParser::instance ()->currentAttr_->attrID(),
          WitParser::instance ()->currentEntity_->entityID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
    ;}
    break;

  case 68:
#line 970 "../src/witParse.y"
    {
      bndPtr = NULL;
      WitParser::instance ()->setParseState(WitParser::PS_endBounds);
    ;}
    break;

  case 69:
#line 977 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);

      // Construct a BoundSetSpec object
      bndPtr = new WitBoundSetSpec(WitBoundSetSpec::exec);

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitAttribute::boundSet);

      argPtr = WitParser::instance ()->currentAttr_->setBoundSetArg(bndPtr);

      free (yylval.strval);
      (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 70:
#line 1004 "../src/witParse.y"
    { 
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);

      // Construct a BoundSetSpec object
      bndPtr = new WitBoundSetSpec(WitBoundSetSpec::stock);

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitAttribute::boundSet);

      argPtr = WitParser::instance ()->currentAttr_->setBoundSetArg(bndPtr);

      free (yylval.strval);
      (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 71:
#line 1031 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);

      // Construct a BoundSetSpec object
      bndPtr = new WitBoundSetSpec(WitBoundSetSpec::cumShip);

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitAttribute::boundSet);
      argPtr = WitParser::instance ()->currentAttr_->setBoundSetArg(bndPtr);

      free (yylval.strval);
      (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 73:
#line 1061 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);
    ;}
    break;

  case 74:
#line 1067 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_boundType);
    ;}
    break;

  case 75:
#line 1072 "../src/witParse.y"
    {
       switch ((yyvsp[(1) - (3)].btype)) {
         case WitBoundSetSpec::hardlb:
           bndPtr->setHardLBVec(vecPtr);
           break;

         case WitBoundSetSpec::softlb:
           bndPtr->setSoftLBVec(vecPtr);
           break;

         case WitBoundSetSpec::hardub:
           bndPtr->setHardUBVec(vecPtr);
           break;
       }
    ;}
    break;

  case 76:
#line 1090 "../src/witParse.y"
    {
      if (bndPtr->hardLBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateHardLBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      (yyval.btype) = WitBoundSetSpec::hardlb;
    ;}
    break;

  case 77:
#line 1101 "../src/witParse.y"
    {
      if (bndPtr->softLBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateSoftLBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      (yyval.btype) = WitBoundSetSpec::softlb;
    ;}
    break;

  case 78:
#line 1112 "../src/witParse.y"
    {
      if (bndPtr->hardUBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateHardUBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      (yyval.btype) = WitBoundSetSpec::hardub;
    ;}
    break;

  case 79:
#line 1124 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_read);
      commandState = WitParser::PS_read;
      WitParser::instance ()->currentRule_ = "read";
      // printf("Current rule is: %s\n", WitParser::instance ()->currentRule_);
    ;}
    break;

  case 80:
#line 1132 "../src/witParse.y"
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    ;}
    break;

  case 81:
#line 1137 "../src/witParse.y"
    {
      /* save lexer state and shift input stream to new file */
      errno   = 0;
      retcode = WitParlex::saveFile(yylval.strval);
      if (retcode == FOPEN_ERROR) {
        yylval.strval[strlen(yylval.strval)-1] = '\0';
        WitParser::instance ()->myMsgFac () ("fopenSmsg",
           yylval.strval+1,
          "r",
          errno,
          strerror(errno));
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }

      if (retcode == STACK_OVERFLOW) {
        WitParser::instance ()->myMsgFac () ("stackOverflowMsg", STACKSIZE);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }

//    printf("Lexer is now reading from file %s\n", WitParlex::fileName());

      WitParser::instance ()->setDataFileName   ();
      WitParser::instance ()->setDataFileLineNo ();

      //#if defined (__GNUC__)
      yyrestart(yyin);
      //#endif

      WitParser::instance ()->setParseState(WitParser::PS_initial);
      free (yylval.strval);
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2676 "witParse.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1177 "../src/witParse.y"


//------------------------------------------------------------------------------

WitParser * WitParser::instance_ = NULL;

//------------------------------------------------------------------------------

void WitParser::parse (WitProblem * theProblem, const char * fileName, FILE * f)
   {
   witAssert (instance_ == NULL);

   instance_ = new WitParser;

   instance ()->parseIt (theProblem, fileName, f);

   delete instance ();

   instance_ = NULL;
   }

//------------------------------------------------------------------------------

// accessors

WitParser::PS    WitParser::parseState() const
{ return parseState_; }

WitEntity     *WitParser::currentEntity() const
{ return currentEntity_; }

WitAttribute  *WitParser::currentAttr() const
{ return currentAttr_; }

WitMsgFacility & WitParser::myMsgFac () const
   {
   return myProblem ()->myMsgFac ();
   }

void WitParser::setParseState(PS st)
{ parseState_ = st; }


void prterror()
{
  WitParser::instance ()->myMsgFac () ("lineNumberMsg",
    reinterpret_cast <const char *> (yytext),
    WitParlex::lineno(), WitParlex::fileName());
}

//-----------------------------------------------------------------------------
// WitParser::prtSyntaxError
//
//     Implementation of syntax checker
//
//     Contains function to check the state of the parser and generate
//     a meaninful error message when a syntax error occurs.
//
// The parse states are defined in Parser.h:
//
//  PS_initial               // initial state, looking for "release"
//  PS_add                   // found "add", looking for <entity_name>
//  PS_set                   // found "set", looking for <entity_name>
//  PS_read                  // found "read", looking for <data_file>
//  PS_release               // found "release", looking for release_spec
//  PS_part                  // found "part", looking for <category>
//  PS_operation             // found "operation", looking for <arg>
//  PS_bomEntry              // found "bomEntry", looking for <arg>
//  PS_subEntry              // found "subEntry", looking for <arg>
//  PS_bopEntry              // found "bopEntry", looking for <arg>
//  PS_demand                // found "demand", looking for <arg>
//  PS_partOperation         // found "partWithOperation, looking for <arg>
//  PS_nextArg               // looking for another <arg> or a ";"
//  PS_attr                  // found <attr_name>, looking for <attr_value>
//  PS_single                // found "single", looking for "("
//  PS_dense                 // found "dense", looking for "("
//  PS_sparse                // found "sparse", looking for "("
//  PS_singleParen           // found "single (", looking for <vector_value>
//  PS_denseParen            // found "dense (", looking for <vector_value>
//  PS_sparseParen           // found "sparse (", looking for <period>
//  PS_singleValue           // found single value, looking for ")"
//  PS_denseValue            // found <value>, looking for <value> or ")"
//  PS_sparseValue           // found <period_val>, looking for <period> or ")"
//  PS_nextAttr              // looking for <attr_name> or ";"
//  PS_nextCommand           // found ";" and looking for <command>
//  PS_commandEnd            // looking for ";"
//  PS_period                // found <period>, looking for ":"
//  PS_periodColon	     // found ":", looking for <vector_value>
//  PS_nextBound             // found <bound_set_type>, looking for <bounds>
//  PS_boundType             // found <bound_type>, looking for <vector>
//  PS_endBounds             // found "endBounds", looking for ";" or next attr
//  PS_setProblem            // found "set problem", looking for
//                           //        <attribute name>
//-----------------------------------------------------------------------------

void WitParser::prtSyntaxError(WitSymTabEntry *pEntry,
                WitParser::PS parseState, const char * yytext) const
{
  const char * s1;

  // Print syntax error message, depending on current parser state
  switch (parseState) {
    case WitParser::PS_initial:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
         "\"release\"", yytext);
      break;
    case WitParser::PS_commandEnd:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg", "a \";\"", yytext);
      break;
    case WitParser::PS_nextCommand:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
      "a command name (e.g., set, add, read)", yytext);
      break;
    case WitParser::PS_add:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of an addable object type after \"add\"", yytext);
      break;
    case WitParser::PS_set:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of a settable object type after \"set\"", yytext);
      break;
    case WitParser::PS_setProblem:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of an attribute after \"set problem\"", yytext);
      break;
    case WitParser::PS_read:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a string (the name of a file) after \"read\"", yytext);
      break;
    case WitParser::PS_release:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a quoted string (the release number) after \"release\"", yytext);
      break;
    case WitParser::PS_part:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "part", yytext);
      break;
    case WitParser::PS_operation:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "operation", yytext);
      break;
    case WitParser::PS_bomEntry:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "bomEntry", yytext);
      break;
    case WitParser::PS_subEntry:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "subEntry", yytext);
      break;
    case WitParser::PS_bopEntry:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "bopEntry", yytext);
      break;
    case WitParser::PS_demand:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "demand", yytext);
      break;
    case WitParser::PS_partOperation:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "partWithOperation", yytext);
      break;
    case WitParser::PS_nextArg:

      // Check to see if we have the correct number of argumenta.
      // If so, we are looking for an attribute or a semicolon
      if (WitParser::instance ()->currentEntity()->argc() >= pEntry->nparms()) {
        WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
          "the name of an attribute or a \";\"", yytext);
        break;
      };

      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
         "another argument",
         yytext);
      break;
    case WitParser::PS_attr:
      switch (pEntry->parmTypes()[0]) {   // what the value should be
        case WitDRParmType::intScalar:
          s1 = "integer";	
          break;
        case WitDRParmType::doubleScalar:
          s1 = "double";
          break;
        case WitDRParmType::string:
          s1 = "string";
          break;
        case WitDRParmType::boolScalar:
          s1 = "boolean";
          break;
        case WitDRParmType::partCat:
          s1 = "\"category\"";
          break;
        case WitDRParmType::boolVector:
          s1 = "\"boolean vector\"";
          break;
        case WitDRParmType::intVector:
          s1 = "\"integer vector\"";
          break;
        case WitDRParmType::doubleVector:
          s1 = "\"double vector\"";
          break;
        case WitDRParmType::boundSet:
          s1 = "\"bound set\"";
          break;
        default:
          s1 = "unknown";
          break;
      };
      WitParser::instance ()->myMsgFac () ("attrValueNotFound",
        s1, WitParser::instance ()->currentAttr()->attrID(), yytext);
      break;
    case WitParser::PS_nextAttr:
    case WitParser::PS_endBounds:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of an attribute or a \";\"", yytext);
      break;
    case WitParser::PS_nextBound:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a bound set attribute (e.g., softLB, hardLB, hardUB)"
        " or \"endBounds\"", yytext);
      break;
    case WitParser::PS_boundType:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector format (e.g, single, dense, sparse)", yytext);
      break;
    case WitParser::PS_single:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \"(\" after \"single\"", yytext);
      break;
    case WitParser::PS_dense:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \"(\" after \"dense\"", yytext);
      break;
    case WitParser::PS_sparse:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \"(\" after \"sparse\"", yytext);
      break;
    case WitParser::PS_singleParen:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector element for a \"single\" vector specification",
        yytext);
      break;
    case WitParser::PS_denseParen:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector element for a \"dense\" vector specification",
        yytext);
      break;
    case WitParser::PS_sparseParen:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a period (an integer) in \"sparse\" vector specification", yytext);
      break;
    case WitParser::PS_singleValue:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \")\" after the one vector value in a \"single\" vector "
        "specification", yytext);
      break;
    case WitParser::PS_denseValue:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector value or a \")\"", yytext);
      break;
    case WitParser::PS_sparseValue:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a period (an integer) or a \")\"", yytext);
      break;
    case WitParser::PS_period:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \":\" in \"sparse\" vector specification", yytext);
      break;
    case WitParser::PS_periodColon:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector value (a number) after the \":\"", yytext);
      break;
  }
}

//------------------------------------------------------------------------------

void WitParser::setDataFileName () const
   {
   myProblem ()->myApiMgr ()->myInputID ()->setDataFileName (fileName ());
   }

//------------------------------------------------------------------------------

void WitParser::setDataFileLineNo () const
   {
   myProblem ()->myApiMgr ()->myInputID ()->setDataFileLineNo (lineno ());
   }

//-----------------------------------------------------------------------------

// Constructor and destructor

WitParser::WitParser ():
      theSymTable_   (NULL),
      currentRule_   (),
      previousRule_  (),
      currentEntity_ (NULL),
      currentAttr_   (NULL),
      parseState_    (PS_initial),
      myProblem_     (NULL)
   {
   commandState = PS_initial;
   }

WitParser::~WitParser ()
   {
   }

//------------------------------------------------------------------------------

// Parse the input stream
int WitParser::parseIt(WitProblem * theProblem, const char* fileName, FILE* f)
{
  int   rc;

  WitParlex::setFileName (fileName);   // save current file name
  yyin = f;                            // save pointer to FILE

  myProblem_   = theProblem;
  theSymTable_ = new WitSymTable (theProblem->myWitRun ());
  rc           = theSymTable_->registerRules ();

  if (rc)
     return rc;

  rc = yyparse();

  if (rc)
  {
    WitParser::instance ()->prtSyntaxError(entryPtr,
       WitParser::instance ()->parseState(),
      reinterpret_cast <const char *> (yytext));
    WitParser::instance ()->myMsgFac () ("lineNumberMsg",
      reinterpret_cast <const char *> (yytext),
      WitParlex::lineno(), WitParlex::fileName());
    WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
  }

  delete theSymTable_;
  return rc;
}

