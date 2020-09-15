/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "../src/witParse.y" /* yacc.c:339  */

//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
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


#line 142 "witParse.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "witParse.tab.h".  */
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
#line 77 "../src/witParse.y" /* yacc.c:355  */

    int    intval;
    double doubleval;
    char * strval;
    bool   boolval;
    int    cval;
    int    pval;
    int    btype;

#line 225 "witParse.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_WITPARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 242 "witParse.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

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
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  111

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   125,   128,   129,   132,   141,   150,   162,
     165,   161,   176,   180,   187,   179,   218,   230,   249,   248,
     271,   270,   283,   319,   326,   333,   340,   347,   354,   362,
     366,   365,   428,   435,   436,   484,   485,   488,   545,   577,
     579,   582,   585,   623,   675,   679,   682,   685,   688,   693,
     718,   723,   692,   733,   757,   732,   768,   791,   767,   802,
     803,   811,   809,   822,   823,   826,   927,   944,   943,   968,
     995,  1022,  1049,  1050,  1059,  1058,  1081,  1092,  1103,  1116,
    1124,  1129,  1115,  1166
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
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
  "release_specification", "$@1", "$@2", "release_num", "add_command",
  "$@3", "$@4", "add_clause", "set_command", "$@5", "$@6", "set_clause",
  "entity_name", "global_specification", "entity_specification", "$@7",
  "category", "attribute_list", "argument_list", "argument", "attribute",
  "simple_attribute", "attribute_name", "attribute_value", "simple_value",
  "vector_format", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14",
  "sparse_list", "period_value", "$@15", "value_list", "vector_value",
  "period", "bound_set_attribute", "$@16", "bound_set_type",
  "bounds_specification", "bound_item", "$@17", "bound_type",
  "read_command", "$@18", "$@19", "$@20", "file_name", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    59,    40,    41,    58
};
# endif

#define YYPACT_NINF -92

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-92)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
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

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
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

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
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

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
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


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 133 "../src/witParse.y" /* yacc.c:1646  */
    {
      delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    }
#line 1415 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 142 "../src/witParse.y" /* yacc.c:1646  */
    { 
      delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    }
#line 1427 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 151 "../src/witParse.y" /* yacc.c:1646  */
    {
      if (WitParser::instance ()->currentEntity_ != NULL)
        delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    }
#line 1440 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 162 "../src/witParse.y" /* yacc.c:1646  */
    { WitParser::instance ()->setParseState(WitParser::PS_release); }
#line 1446 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 165 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }
#line 1454 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 170 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
      free (yylval.strval);     
    }
#line 1463 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 180 "../src/witParse.y" /* yacc.c:1646  */
    { 
      WitParser::instance ()->setParseState(WitParser::PS_add);
      commandState = WitParser::PS_add;     /* save command state */
      WitParser::instance ()->currentRule_ = "add";
    }
#line 1473 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 187 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 1502 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 213 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    }
#line 1510 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 219 "../src/witParse.y" /* yacc.c:1646  */
    {
      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            entityType);
      free (yylval.strval); 
      WitParser::instance ()->currentAttr_ = NULL;
    }
#line 1525 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 231 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 1545 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 249 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 1565 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 266 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    }
#line 1573 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 271 "../src/witParse.y" /* yacc.c:1646  */
    { 
      WitParser::instance ()->setParseState(WitParser::PS_set);
      commandState = WitParser::PS_set;     /* save command state */
      WitParser::instance ()->currentRule_ = "set";
    }
#line 1583 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 278 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    }
#line 1591 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 284 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 1629 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 320 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_part);
      WitParser::instance ()->currentRule_ += "_part";
      entityType = WitEntity::part;
    }
#line 1639 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 327 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_operation);
      WitParser::instance ()->currentRule_ += "_operation";
      entityType = WitEntity::operation;
    }
#line 1649 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 334 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_bomEntry);
      WitParser::instance ()->currentRule_ += "_bomEntry";
      entityType = WitEntity::bomEntry;
    }
#line 1659 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 341 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_subEntry);
      WitParser::instance ()->currentRule_ += "_subEntry";
      entityType = WitEntity::subEntry;
    }
#line 1669 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 348 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_bopEntry);
      WitParser::instance ()->currentRule_ += "_bopEntry";
      entityType = WitEntity::bopEntry;
    }
#line 1679 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 355 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_demand);
      WitParser::instance ()->currentRule_ += "_demand";
      entityType = WitEntity::demand;
    }
#line 1689 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 366 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 1752 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 429 "../src/witParse.y" /* yacc.c:1646  */
    { 
      if (WitParser::instance ()->parseState() == WitParser::PS_part)
      (yyval.cval) = (yyvsp[0].cval);
    }
#line 1761 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 437 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 1811 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 489 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextArg);
      switch((yyvsp[0].pval)) {
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
    }
#line 1871 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 546 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 1905 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 586 "../src/witParse.y" /* yacc.c:1646  */
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

      (yyval.strval) = (yyvsp[0].strval);
    }
#line 1945 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 624 "../src/witParse.y" /* yacc.c:1646  */
    {
      switch((yyvsp[0].pval)) {
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
    }
#line 2000 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 676 "../src/witParse.y" /* yacc.c:1646  */
    { (yyval.pval) = WitDRParmType::undefined; }
#line 2006 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 680 "../src/witParse.y" /* yacc.c:1646  */
    { (yyval.pval) = WitDRParmType::string; }
#line 2012 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 683 "../src/witParse.y" /* yacc.c:1646  */
    { (yyval.pval) = WitDRParmType::intScalar; }
#line 2018 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 686 "../src/witParse.y" /* yacc.c:1646  */
    { (yyval.pval) = WitDRParmType::doubleScalar; }
#line 2024 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 689 "../src/witParse.y" /* yacc.c:1646  */
    { (yyval.pval) = WitDRParmType::boolScalar; }
#line 2030 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 693 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 2058 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 718 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_singleParen);
    }
#line 2066 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 723 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_singleValue);
    }
#line 2074 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 728 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }
#line 2082 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 733 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 2109 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 757 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparseParen);
    }
#line 2117 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 763 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }
#line 2125 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 768 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 2151 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 791 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_denseParen);
    }
#line 2159 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 797 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }
#line 2167 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 804 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparseValue);
    }
#line 2175 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 811 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_periodColon);
    }
#line 2183 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 816 "../src/witParse.y" /* yacc.c:1646  */
    { 
      WitParser::instance ()->setParseState(WitParser::PS_sparseParen);
      (yyval.pval) = (yyvsp[0].pval); 
    }
#line 2192 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 827 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_denseValue);

      if (entryPtr != NULL) {
      switch((yyvsp[0].pval)) {

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
    }
#line 2295 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 928 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 2313 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 944 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 2334 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 962 "../src/witParse.y" /* yacc.c:1646  */
    {
      bndPtr = NULL;
      WitParser::instance ()->setParseState(WitParser::PS_endBounds);
    }
#line 2343 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 969 "../src/witParse.y" /* yacc.c:1646  */
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
      (yyval.strval) = (yyvsp[0].strval);
    }
#line 2373 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 996 "../src/witParse.y" /* yacc.c:1646  */
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
      (yyval.strval) = (yyvsp[0].strval);
    }
#line 2403 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1023 "../src/witParse.y" /* yacc.c:1646  */
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
      (yyval.strval) = (yyvsp[0].strval);
    }
#line 2432 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1053 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);
    }
#line 2440 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1059 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_boundType);
    }
#line 2448 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1064 "../src/witParse.y" /* yacc.c:1646  */
    {
       switch ((yyvsp[-2].btype)) {
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
    }
#line 2468 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1082 "../src/witParse.y" /* yacc.c:1646  */
    {
      if (bndPtr->hardLBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateHardLBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      (yyval.btype) = WitBoundSetSpec::hardlb;
    }
#line 2482 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1093 "../src/witParse.y" /* yacc.c:1646  */
    {
      if (bndPtr->softLBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateSoftLBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      (yyval.btype) = WitBoundSetSpec::softlb;
    }
#line 2496 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1104 "../src/witParse.y" /* yacc.c:1646  */
    {
      if (bndPtr->hardUBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateHardUBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      (yyval.btype) = WitBoundSetSpec::hardub;
    }
#line 2510 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1116 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_read);
      commandState = WitParser::PS_read;
      WitParser::instance ()->currentRule_ = "read";
      // printf("Current rule is: %s\n", WitParser::instance ()->currentRule_);
    }
#line 2521 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1124 "../src/witParse.y" /* yacc.c:1646  */
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }
#line 2529 "witParse.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1129 "../src/witParse.y" /* yacc.c:1646  */
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
    }
#line 2567 "witParse.tab.c" /* yacc.c:1646  */
    break;


#line 2571 "witParse.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 1169 "../src/witParse.y" /* yacc.c:1906  */


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
