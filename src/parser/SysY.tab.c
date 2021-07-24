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
     INTCONST = 258,
     MULOP = 259,
     EQOP = 260,
     RELOP = 261,
     STRING = 262,
     K_CURLY_L = 263,
     K_CURLY_R = 264,
     K_SQUARE_L = 265,
     K_SQUARE_R = 266,
     K_PARENTHESES_L = 267,
     K_PARENTHESES_R = 268,
     K_COMMA = 269,
     K_SEMICOLON = 270,
     K_ASSIGNOP = 271,
     K_ADD = 272,
     K_SUB = 273,
     K_MUL = 274,
     K_DIV = 275,
     K_MOD = 276,
     K_GT = 277,
     K_GTE = 278,
     K_LT = 279,
     K_LTE = 280,
     K_EQ = 281,
     K_NEQ = 282,
     K_AND = 283,
     K_OR = 284,
     K_NOT = 285,
     KEYWORD = 286,
     K_CONST = 287,
     K_VOID = 288,
     K_INT = 289,
     K_IF = 290,
     K_ELSE = 291,
     K_WHILE = 292,
     K_BREAK = 293,
     K_CONTINUE = 294,
     K_RETURN = 295,
     IDENT = 296,
     COMMENT = 297,
     FILEEND = 298,
     K_LOW_THAN_ELSE = 299
   };
#endif
/* Tokens.  */
#define INTCONST 258
#define MULOP 259
#define EQOP 260
#define RELOP 261
#define STRING 262
#define K_CURLY_L 263
#define K_CURLY_R 264
#define K_SQUARE_L 265
#define K_SQUARE_R 266
#define K_PARENTHESES_L 267
#define K_PARENTHESES_R 268
#define K_COMMA 269
#define K_SEMICOLON 270
#define K_ASSIGNOP 271
#define K_ADD 272
#define K_SUB 273
#define K_MUL 274
#define K_DIV 275
#define K_MOD 276
#define K_GT 277
#define K_GTE 278
#define K_LT 279
#define K_LTE 280
#define K_EQ 281
#define K_NEQ 282
#define K_AND 283
#define K_OR 284
#define K_NOT 285
#define KEYWORD 286
#define K_CONST 287
#define K_VOID 288
#define K_INT 289
#define K_IF 290
#define K_ELSE 291
#define K_WHILE 292
#define K_BREAK 293
#define K_CONTINUE 294
#define K_RETURN 295
#define IDENT 296
#define COMMENT 297
#define FILEEND 298
#define K_LOW_THAN_ELSE 299




/* Copy the first part of user declarations.  */
#line 1 "src/parser/SysY.y"

    #include "./SysY.tab.h"
    #include "../SysY.type/SysY.type.def.h"
    #include "../SysY.type/SysY.type.new.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #define YYERROR_VERBOSE
    extern YYSTYPE result;
    int yylex();
    void yyerror(const char *s);


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
#line 14 "src/parser/SysY.y"
{
    struct Keyword *keyword;
    struct CompUnit *compunit;
    struct Decl *decl;
    struct ConstDecl *constdecl;
    struct BType *btype;
    struct ConstDefs *constdefs;
    struct ConstDef *constdef;
    struct ConstArrayDefs *constarraydefs;
    struct ConstArrayDef *constarraydef;
    struct ConstInitVal *constinitval;
    struct ConstInitVals *constinitvals;
    struct VarDecl *vardecl;
    struct VarDefs *vardefs;
    struct VarDef *vardef;
    struct InitVal *initval;
    struct InitVals *initvals;
    struct FuncDef *funcdef;
    struct FuncType *functype;
    struct FuncFParams *funcfparams;
    struct FuncFParam *funcfparam;
    struct ExpArrayDefs *exparraydefs;
    struct ExpArrayDef *exparraydef;
    struct Block *block;
    struct BlockItems *blockitems;
    struct BlockItem *blockitem;
    struct Stmt *stmt;
    struct Assign *assign;
    struct IfStmt *ifstmt;
    struct WhileStmt *whilestmt;
    struct ReturnStmt *returnstmt;
    struct Exp *exp;
    struct Cond *cond;
    struct LVal *lval;
    struct ArrayImpl *arrayimpl;
    struct PrimaryExp *primaryexp;
    struct Number *number;
    struct IntConst *intconst;
    struct UnaryExp *unaryexp;
    struct FuncImpl *funcimpl;
    struct UnaryExps *unaryexps;
    struct UnaryOp *unaryop;
    struct FuncRParams *funcrparams;
    struct FuncRParam *funcrparam;
    struct MulExp *mulexp;
    struct MulOp *mulop;
    struct AddExp *addexp;
    struct AddOp *addop;
    struct RelExp *relexp;
    struct RelOp *relop;
    struct EqExp *eqexp;
    struct EqOp *eqop;
    struct LAndExp *landexp;
    struct LAndOp *landop;
    struct LOrExp *lorexp;
    struct LOrOp *lorop;
    struct ConstExp *constexp;
    struct Ident *ident;
    struct RefList *reflist;
    struct String* string;
}
/* Line 193 of yacc.c.  */
#line 259 "src/parser/SysY.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 272 "src/parser/SysY.tab.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   270

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNRULES -- Number of states.  */
#define YYNSTATES  166

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    15,    17,    22,
      24,    28,    32,    37,    39,    42,    46,    48,    51,    55,
      57,    61,    65,    67,    71,    73,    76,    80,    85,    87,
      90,    94,    96,   100,   106,   112,   119,   126,   128,   132,
     135,   140,   146,   148,   151,   155,   158,   162,   164,   167,
     169,   171,   176,   178,   181,   183,   189,   197,   203,   206,
     209,   212,   216,   218,   220,   222,   225,   229,   231,   233,
     235,   237,   241,   246,   249,   251,   253,   255,   257,   261,
     263,   265,   267,   271,   273,   277,   281,   283,   287,   289,
     293,   295,   299,   301,   305
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      46,     0,    -1,    47,    -1,    60,    -1,    46,    47,    -1,
      46,    60,    -1,    48,    -1,    55,    -1,    32,    34,    49,
      15,    -1,    50,    -1,    49,    14,    50,    -1,    41,    16,
      53,    -1,    41,    51,    16,    53,    -1,    52,    -1,    51,
      52,    -1,    10,    84,    11,    -1,    84,    -1,     8,     9,
      -1,     8,    54,     9,    -1,    53,    -1,    54,    14,    53,
      -1,    34,    56,    15,    -1,    57,    -1,    56,    14,    57,
      -1,    41,    -1,    41,    51,    -1,    41,    16,    58,    -1,
      41,    51,    16,    58,    -1,    69,    -1,     8,     9,    -1,
       8,    59,     9,    -1,    58,    -1,    59,    14,    58,    -1,
      34,    41,    12,    13,    65,    -1,    33,    41,    12,    13,
      65,    -1,    34,    41,    12,    61,    13,    65,    -1,    33,
      41,    12,    61,    13,    65,    -1,    62,    -1,    61,    14,
      62,    -1,    34,    41,    -1,    34,    41,    10,    11,    -1,
      34,    41,    10,    11,    63,    -1,    64,    -1,    63,    64,
      -1,    10,    69,    11,    -1,     8,     9,    -1,     8,    66,
       9,    -1,    67,    -1,    66,    67,    -1,    47,    -1,    68,
      -1,    71,    16,    69,    15,    -1,    15,    -1,    69,    15,
      -1,    65,    -1,    35,    12,    70,    13,    68,    -1,    35,
      12,    70,    13,    68,    36,    68,    -1,    37,    12,    70,
      13,    68,    -1,    38,    15,    -1,    39,    15,    -1,    40,
      15,    -1,    40,    69,    15,    -1,    79,    -1,    83,    -1,
      41,    -1,    41,    63,    -1,    12,    69,    13,    -1,    71,
      -1,    73,    -1,     3,    -1,    72,    -1,    41,    12,    13,
      -1,    41,    12,    76,    13,    -1,    75,    74,    -1,    17,
      -1,    18,    -1,    30,    -1,    77,    -1,    76,    14,    77,
      -1,    69,    -1,     7,    -1,    74,    -1,    78,     4,    74,
      -1,    78,    -1,    79,    17,    78,    -1,    79,    18,    78,
      -1,    79,    -1,    80,     6,    79,    -1,    80,    -1,    81,
       5,    80,    -1,    81,    -1,    82,    28,    81,    -1,    82,
      -1,    83,    29,    82,    -1,    79,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   168,   168,   169,   170,   171,   175,   176,   180,   184,
     185,   189,   190,   194,   195,   199,   203,   204,   205,   209,
     210,   214,   218,   219,   223,   224,   225,   226,   230,   231,
     232,   236,   237,   241,   242,   243,   244,   248,   249,   253,
     254,   255,   259,   260,   264,   268,   269,   273,   274,   278,
     279,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   297,   301,   305,   306,   310,   311,   312,   316,
     320,   321,   322,   323,   327,   328,   329,   333,   334,   338,
     339,   342,   343,   347,   348,   349,   353,   354,   358,   359,
     363,   364,   368,   369,   373
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTCONST", "MULOP", "EQOP", "RELOP",
  "STRING", "K_CURLY_L", "K_CURLY_R", "K_SQUARE_L", "K_SQUARE_R",
  "K_PARENTHESES_L", "K_PARENTHESES_R", "K_COMMA", "K_SEMICOLON",
  "K_ASSIGNOP", "K_ADD", "K_SUB", "K_MUL", "K_DIV", "K_MOD", "K_GT",
  "K_GTE", "K_LT", "K_LTE", "K_EQ", "K_NEQ", "K_AND", "K_OR", "K_NOT",
  "KEYWORD", "K_CONST", "K_VOID", "K_INT", "K_IF", "K_ELSE", "K_WHILE",
  "K_BREAK", "K_CONTINUE", "K_RETURN", "IDENT", "COMMENT", "FILEEND",
  "K_LOW_THAN_ELSE", "$accept", "COMPUNIT", "DECL", "CONSTDECL",
  "CONSTDEFS", "CONSTDEF", "CONSTARRAYDEFS", "CONSTARRAYDEF",
  "CONSTINITVAL", "CONSTINITVALS", "VARDECL", "VARDEFS", "VARDEF",
  "INITVAL", "INITVALS", "FUNCDEF", "FUNCFPARAMS", "FUNCFPARAM",
  "EXPARRAYDEFS", "EXPARRAYDEF", "BLOCK", "BLOCKITEMS", "BLOCKITEM",
  "STMT", "EXP", "COND", "LVAL", "PRIMARYEXP", "NUMBER", "UNARYEXP",
  "UNARYOP", "FUNCRPARAMS", "FUNCRPARAM", "MULEXP", "ADDEXP", "RELEXP",
  "EQEXP", "LANDEXP", "LOREXP", "CONSTEXP", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    46,    46,    46,    47,    47,    48,    49,
      49,    50,    50,    51,    51,    52,    53,    53,    53,    54,
      54,    55,    56,    56,    57,    57,    57,    57,    58,    58,
      58,    59,    59,    60,    60,    60,    60,    61,    61,    62,
      62,    62,    63,    63,    64,    65,    65,    66,    66,    67,
      67,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    69,    70,    71,    71,    72,    72,    72,    73,
      74,    74,    74,    74,    75,    75,    75,    76,    76,    77,
      77,    78,    78,    79,    79,    79,    80,    80,    81,    81,
      82,    82,    83,    83,    84
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     4,     1,
       3,     3,     4,     1,     2,     3,     1,     2,     3,     1,
       3,     3,     1,     3,     1,     2,     3,     4,     1,     2,
       3,     1,     3,     5,     5,     6,     6,     1,     3,     2,
       4,     5,     1,     2,     3,     2,     3,     1,     2,     1,
       1,     4,     1,     2,     1,     5,     7,     5,     2,     2,
       2,     3,     1,     1,     1,     2,     3,     1,     1,     1,
       1,     3,     4,     2,     1,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     6,     7,     3,     0,
       0,    24,     0,    22,     1,     4,     5,     0,     0,     9,
       0,     0,     0,     0,    25,    13,     0,    21,     0,     0,
       0,     8,     0,     0,     0,    37,    69,     0,    74,    75,
      76,    64,    67,    70,    68,    81,     0,    83,    94,     0,
       0,     0,     0,    26,    28,    62,     0,    14,    24,    23,
       0,    11,    16,     0,    10,     0,    34,    39,     0,     0,
       0,     0,     0,    65,    42,    73,     0,     0,     0,    15,
      33,     0,    29,    31,     0,    27,    17,    19,     0,    12,
      45,    52,     0,     0,     0,     0,     0,     0,    49,    54,
       0,    47,    50,     0,    67,     0,    36,    38,    66,     0,
      80,    71,    79,     0,    77,    43,    82,    84,    85,    35,
      30,     0,    18,     0,     0,     0,    58,    59,    60,     0,
      46,    48,    53,     0,    40,    44,    72,     0,    32,    20,
       0,    86,    88,    90,    92,    63,     0,    61,     0,    41,
      78,     0,     0,     0,     0,     0,     0,    51,    55,    87,
      89,    91,    93,    57,     0,    56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,    98,     6,    18,    19,    24,    25,    61,    88,
       7,    12,    13,    53,    84,     8,    34,    35,    73,    74,
      99,   100,   101,   102,   103,   140,    42,    43,    44,    45,
      46,   113,   114,    47,    55,   142,   143,   144,   145,    62
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -140
static const yytype_int16 yypact[] =
{
      94,   -13,   -17,    -8,    26,  -140,  -140,  -140,  -140,    -5,
      33,    52,    41,  -140,  -140,  -140,  -140,     4,   134,  -140,
      -3,   229,    -2,    88,    19,  -140,     7,  -140,   185,    64,
      -5,  -140,    55,    29,   137,  -140,  -140,   229,  -140,  -140,
    -140,    97,  -140,  -140,  -140,  -140,   229,    74,   140,    78,
      55,   141,    10,  -140,  -140,   140,    88,  -140,   109,  -140,
      35,  -140,  -140,   185,  -140,   105,  -140,    84,    55,    90,
     117,   229,    54,   126,  -140,  -140,   229,   229,   229,  -140,
    -140,    55,  -140,  -140,    68,  -140,  -140,  -140,    72,  -140,
    -140,  -140,     7,   129,   148,   123,   153,   213,  -140,  -140,
     144,  -140,  -140,   154,   149,   159,  -140,  -140,  -140,   160,
    -140,  -140,  -140,   150,  -140,  -140,  -140,    74,    74,  -140,
    -140,    88,  -140,   185,   229,   229,  -140,  -140,  -140,   157,
    -140,  -140,  -140,   229,   126,  -140,  -140,    80,  -140,  -140,
     162,   140,   167,   172,   152,   158,   176,  -140,   175,   126,
    -140,   183,   229,   229,   229,   229,   183,  -140,   156,   140,
     167,   172,   152,  -140,   183,  -140
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -140,  -140,   112,  -140,  -140,   164,   179,    61,   -54,  -140,
    -140,  -140,   173,   -48,  -140,   200,   184,   136,    73,   -70,
     -27,  -140,   108,  -139,   -22,    85,   -63,  -140,  -140,   -30,
    -140,  -140,    75,    89,   -21,    56,    57,    59,  -140,   196
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      48,    54,   104,   115,    83,    66,    87,    48,    85,    89,
      32,    50,   158,    36,    21,    70,    75,   163,    52,    82,
      28,     9,    37,    80,    10,   165,    14,    38,    39,    21,
      54,    33,    33,    11,    54,    56,    17,   104,    36,    48,
      40,   106,    48,    60,    86,    20,   116,    37,    58,   109,
     112,    41,    38,    39,   119,    26,    27,    36,     1,     2,
       3,   110,    21,    65,    22,    40,    37,   111,    23,   139,
      67,    38,    39,   138,    21,   129,    41,   120,    76,   115,
      63,   122,   121,    36,    40,    57,   123,   110,   104,    79,
      57,    36,    37,   104,   105,    41,    52,    38,    39,    54,
      37,   104,    48,   141,   141,    38,    39,    71,    36,    72,
      40,   148,     5,    65,    90,   112,    15,    37,    40,    21,
      91,    41,    38,    39,    33,    23,     1,     2,     3,    41,
     108,   159,   141,   141,   141,    40,    71,     1,   126,    92,
      93,   124,    94,    95,    96,    97,    41,    36,    30,    31,
      68,    69,    65,   130,    81,    69,    37,    77,    78,    91,
     125,    38,    39,   136,   137,   133,   117,   118,   127,   132,
     134,   135,   147,   152,    40,   151,     1,   153,    92,    93,
     154,    94,    95,    96,    97,    41,    36,   155,    36,   156,
     157,    65,   164,    60,    64,    37,    29,    37,    91,    59,
      38,    39,    38,    39,    16,   107,    51,   149,   131,   160,
     146,   161,   150,    40,   162,    40,    36,    49,    93,     0,
      94,    95,    96,    97,    41,    37,    41,     0,   128,     0,
      38,    39,    36,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    40,     0,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      41
};

static const yytype_int16 yycheck[] =
{
      21,    23,    65,    73,    52,    32,    60,    28,    56,    63,
      13,    13,   151,     3,    10,    37,    46,   156,     8,     9,
      16,    34,    12,    50,    41,   164,     0,    17,    18,    10,
      52,    34,    34,    41,    56,    16,    41,   100,     3,    60,
      30,    68,    63,     8,     9,    12,    76,    12,    41,    71,
      72,    41,    17,    18,    81,    14,    15,     3,    32,    33,
      34,     7,    10,     8,    12,    30,    12,    13,    16,   123,
      41,    17,    18,   121,    10,    97,    41,     9,     4,   149,
      16,     9,    14,     3,    30,    24,    14,     7,   151,    11,
      29,     3,    12,   156,    10,    41,     8,    17,    18,   121,
      12,   164,   123,   124,   125,    17,    18,    10,     3,    12,
      30,   133,     0,     8,     9,   137,     4,    12,    30,    10,
      15,    41,    17,    18,    34,    16,    32,    33,    34,    41,
      13,   152,   153,   154,   155,    30,    10,    32,    15,    34,
      35,    12,    37,    38,    39,    40,    41,     3,    14,    15,
      13,    14,     8,     9,    13,    14,    12,    17,    18,    15,
      12,    17,    18,    13,    14,    16,    77,    78,    15,    15,
      11,    11,    15,     6,    30,    13,    32,     5,    34,    35,
      28,    37,    38,    39,    40,    41,     3,    29,     3,    13,
      15,     8,    36,     8,    30,    12,    17,    12,    15,    26,
      17,    18,    17,    18,     4,    69,    22,   134,   100,   153,
     125,   154,   137,    30,   155,    30,     3,    21,    35,    -1,
      37,    38,    39,    40,    41,    12,    41,    -1,    15,    -1,
      17,    18,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    30,    -1,    -1,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    32,    33,    34,    46,    47,    48,    55,    60,    34,
      41,    41,    56,    57,     0,    47,    60,    41,    49,    50,
      12,    10,    12,    16,    51,    52,    14,    15,    16,    51,
      14,    15,    13,    34,    61,    62,     3,    12,    17,    18,
      30,    41,    71,    72,    73,    74,    75,    78,    79,    84,
      13,    61,     8,    58,    69,    79,    16,    52,    41,    57,
       8,    53,    84,    16,    50,     8,    65,    41,    13,    14,
      69,    10,    12,    63,    64,    74,     4,    17,    18,    11,
      65,    13,     9,    58,    59,    58,     9,    53,    54,    53,
       9,    15,    34,    35,    37,    38,    39,    40,    47,    65,
      66,    67,    68,    69,    71,    10,    65,    62,    13,    69,
       7,    13,    69,    76,    77,    64,    74,    78,    78,    65,
       9,    14,     9,    14,    12,    12,    15,    15,    15,    69,
       9,    67,    15,    16,    11,    11,    13,    14,    58,    53,
      70,    79,    80,    81,    82,    83,    70,    15,    69,    63,
      77,    13,     6,     5,    28,    29,    13,    15,    68,    79,
      80,    81,    82,    68,    36,    68
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
        case 2:
#line 168 "src/parser/SysY.y"
    { (yyval.compunit) = newCompUnit(COMPUNIT,DECL,(yyvsp[(1) - (1)].decl),NULL); result.compunit = (yyval.compunit)->next; ;}
    break;

  case 3:
#line 169 "src/parser/SysY.y"
    { (yyval.compunit) = newCompUnit(COMPUNIT,FUNCDEF,(yyvsp[(1) - (1)].funcdef),NULL); result.compunit = (yyval.compunit)->next; ;}
    break;

  case 4:
#line 170 "src/parser/SysY.y"
    { (yyval.compunit) = newCompUnit(COMPUNIT,DECL,(yyvsp[(2) - (2)].decl),(yyvsp[(1) - (2)].compunit)); result.compunit = (yyval.compunit)->next; ;}
    break;

  case 5:
#line 171 "src/parser/SysY.y"
    { (yyval.compunit) = newCompUnit(COMPUNIT,FUNCDEF,(yyvsp[(2) - (2)].funcdef),(yyvsp[(1) - (2)].compunit)); result.compunit = (yyval.compunit)->next; ;}
    break;

  case 6:
#line 175 "src/parser/SysY.y"
    { (yyval.decl) = newDecl(DECL,CONSTDECL,(yyvsp[(1) - (1)].constdecl)); ;}
    break;

  case 7:
#line 176 "src/parser/SysY.y"
    { (yyval.decl) = newDecl(DECL,VARDECL,(yyvsp[(1) - (1)].vardecl)); ;}
    break;

  case 8:
#line 180 "src/parser/SysY.y"
    { (yyval.constdecl) = newConstDecl(CONSTDECL,newBType(BTYPE,K_INT),(yyvsp[(3) - (4)].constdefs)->next); ;}
    break;

  case 9:
#line 184 "src/parser/SysY.y"
    { (yyval.constdefs) = newConstDefs(CONSTDEFS,(yyvsp[(1) - (1)].constdef),NULL); ;}
    break;

  case 10:
#line 185 "src/parser/SysY.y"
    { (yyval.constdefs) = newConstDefs(CONSTDEFS,(yyvsp[(3) - (3)].constdef),(yyvsp[(1) - (3)].constdefs)); ;}
    break;

  case 11:
#line 189 "src/parser/SysY.y"
    { (yyval.constdef) = newConstDef(CONSTDEF,(yyvsp[(1) - (3)].ident),NULL,(yyvsp[(3) - (3)].constinitval)); ;}
    break;

  case 12:
#line 190 "src/parser/SysY.y"
    { (yyval.constdef) = newConstDef(CONSTDEF,(yyvsp[(1) - (4)].ident),(yyvsp[(2) - (4)].constarraydefs)->next,(yyvsp[(4) - (4)].constinitval)); ;}
    break;

  case 13:
#line 194 "src/parser/SysY.y"
    { (yyval.constarraydefs) = newConstArrayDefs(CONSTARRAYDEFS,(yyvsp[(1) - (1)].constarraydef),NULL); ;}
    break;

  case 14:
#line 195 "src/parser/SysY.y"
    { (yyval.constarraydefs) = newConstArrayDefs(CONSTARRAYDEFS,(yyvsp[(2) - (2)].constarraydef),(yyvsp[(1) - (2)].constarraydefs)); ;}
    break;

  case 15:
#line 199 "src/parser/SysY.y"
    { (yyval.constarraydef) = newConstArrayDef(CONSTARRAYDEF,(yyvsp[(2) - (3)].constexp)); ;}
    break;

  case 16:
#line 203 "src/parser/SysY.y"
    { (yyval.constinitval) = newConstInitVal(CONSTINITVAL,CONSTEXP,(yyvsp[(1) - (1)].constexp)); ;}
    break;

  case 17:
#line 204 "src/parser/SysY.y"
    { (yyval.constinitval) = newConstInitVal(CONSTINITVAL,CONSTINITVALS,newConstInitVals(CONSTINITVALS,NULL,NULL)); ;}
    break;

  case 18:
#line 205 "src/parser/SysY.y"
    { (yyval.constinitval) = newConstInitVal(CONSTINITVAL,CONSTINITVALS,(yyvsp[(2) - (3)].constinitvals)->next); ;}
    break;

  case 19:
#line 209 "src/parser/SysY.y"
    { (yyval.constinitvals) = newConstInitVals(CONSTINITVALS,(yyvsp[(1) - (1)].constinitval),NULL); ;}
    break;

  case 20:
#line 210 "src/parser/SysY.y"
    { (yyval.constinitvals) = newConstInitVals(CONSTINITVALS,(yyvsp[(3) - (3)].constinitval),(yyvsp[(1) - (3)].constinitvals)); ;}
    break;

  case 21:
#line 214 "src/parser/SysY.y"
    { (yyval.vardecl) = newVarDecl(VARDECL,newBType(BTYPE,K_INT),(yyvsp[(2) - (3)].vardefs)->next); ;}
    break;

  case 22:
#line 218 "src/parser/SysY.y"
    { (yyval.vardefs) = newVarDefs(VARDEFS,(yyvsp[(1) - (1)].vardef),NULL); ;}
    break;

  case 23:
#line 219 "src/parser/SysY.y"
    { (yyval.vardefs) = newVarDefs(VARDEFS,(yyvsp[(3) - (3)].vardef),(yyvsp[(1) - (3)].vardefs)); ;}
    break;

  case 24:
#line 223 "src/parser/SysY.y"
    { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[(1) - (1)].ident),NULL,NULL); ;}
    break;

  case 25:
#line 224 "src/parser/SysY.y"
    { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[(1) - (2)].ident),(yyvsp[(2) - (2)].constarraydefs)->next,NULL); ;}
    break;

  case 26:
#line 225 "src/parser/SysY.y"
    { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[(1) - (3)].ident),NULL,(yyvsp[(3) - (3)].initval)); ;}
    break;

  case 27:
#line 226 "src/parser/SysY.y"
    { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[(1) - (4)].ident),(yyvsp[(2) - (4)].constarraydefs)->next,(yyvsp[(4) - (4)].initval)); ;}
    break;

  case 28:
#line 230 "src/parser/SysY.y"
    { (yyval.initval) = newInitVal(INITVAL,EXP,(yyvsp[(1) - (1)].exp)); ;}
    break;

  case 29:
#line 231 "src/parser/SysY.y"
    { (yyval.initval) = newInitVal(INITVAL,INITVAL,newInitVals(INITVALS,NULL,NULL)); ;}
    break;

  case 30:
#line 232 "src/parser/SysY.y"
    { (yyval.initval) = newInitVal(INITVAL,INITVAL,(yyvsp[(2) - (3)].initvals)->next); ;}
    break;

  case 31:
#line 236 "src/parser/SysY.y"
    { (yyval.initvals) = newInitVals(INITVALS,(yyvsp[(1) - (1)].initval),NULL); ;}
    break;

  case 32:
#line 237 "src/parser/SysY.y"
    { (yyval.initvals) = newInitVals(INITVALS,(yyvsp[(3) - (3)].initval),(yyvsp[(1) - (3)].initvals)); ;}
    break;

  case 33:
#line 241 "src/parser/SysY.y"
    { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_INT),(yyvsp[(2) - (5)].ident),newFuncFParams(FUNCFPARAMS,NULL,NULL),(yyvsp[(5) - (5)].block)); ;}
    break;

  case 34:
#line 242 "src/parser/SysY.y"
    { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_VOID),(yyvsp[(2) - (5)].ident),newFuncFParams(FUNCFPARAMS,NULL,NULL),(yyvsp[(5) - (5)].block)); ;}
    break;

  case 35:
#line 243 "src/parser/SysY.y"
    { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_INT),(yyvsp[(2) - (6)].ident),(yyvsp[(4) - (6)].funcfparams)->next,(yyvsp[(6) - (6)].block)); ;}
    break;

  case 36:
#line 244 "src/parser/SysY.y"
    { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_VOID),(yyvsp[(2) - (6)].ident),(yyvsp[(4) - (6)].funcfparams)->next,(yyvsp[(6) - (6)].block)); ;}
    break;

  case 37:
#line 248 "src/parser/SysY.y"
    { (yyval.funcfparams) = newFuncFParams(FUNCFPARAMS,(yyvsp[(1) - (1)].funcfparam),NULL); ;}
    break;

  case 38:
#line 249 "src/parser/SysY.y"
    { (yyval.funcfparams) = newFuncFParams(FUNCFPARAMS,(yyvsp[(3) - (3)].funcfparam),(yyvsp[(1) - (3)].funcfparams)); ;}
    break;

  case 39:
#line 253 "src/parser/SysY.y"
    { (yyval.funcfparam) = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),(yyvsp[(2) - (2)].ident),NULL); ;}
    break;

  case 40:
#line 254 "src/parser/SysY.y"
    { (yyval.funcfparam) = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),(yyvsp[(2) - (4)].ident),newExpArrayDefs(EXPARRAYDEFS,newExpArrayDef(EXPARRAYDEF,NULL),NULL)); ;}
    break;

  case 41:
#line 255 "src/parser/SysY.y"
    { (yyval.funcfparam) = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),(yyvsp[(2) - (5)].ident),newExpArrayDefs(EXPARRAYDEFS,newExpArrayDef(EXPARRAYDEF,NULL),(yyvsp[(5) - (5)].exparraydefs)->next)); ;}
    break;

  case 42:
#line 259 "src/parser/SysY.y"
    { (yyval.exparraydefs) = newExpArrayDefs(EXPARRAYDEFS,(yyvsp[(1) - (1)].exparraydef),NULL); ;}
    break;

  case 43:
#line 260 "src/parser/SysY.y"
    { (yyval.exparraydefs) = newExpArrayDefs(EXPARRAYDEFS,(yyvsp[(2) - (2)].exparraydef),(yyvsp[(1) - (2)].exparraydefs)); ;}
    break;

  case 44:
#line 264 "src/parser/SysY.y"
    { (yyval.exparraydef) = newExpArrayDef(EXPARRAYDEF,(yyvsp[(2) - (3)].exp)); ;}
    break;

  case 45:
#line 268 "src/parser/SysY.y"
    { (yyval.block) = newBlock(BLOCK,NULL); ;}
    break;

  case 46:
#line 269 "src/parser/SysY.y"
    { (yyval.block) = newBlock(BLOCK,(yyvsp[(2) - (3)].blockitems)->next); ;}
    break;

  case 47:
#line 273 "src/parser/SysY.y"
    { (yyval.blockitems) = newBlockItems(BLOCKITEMS,(yyvsp[(1) - (1)].blockitem),NULL); ;}
    break;

  case 48:
#line 274 "src/parser/SysY.y"
    { (yyval.blockitems) = newBlockItems(BLOCKITEMS,(yyvsp[(2) - (2)].blockitem),(yyvsp[(1) - (2)].blockitems)); ;}
    break;

  case 49:
#line 278 "src/parser/SysY.y"
    { (yyval.blockitem) = newBlockItem(BLOCKITEM,DECL,(yyvsp[(1) - (1)].decl)); ;}
    break;

  case 50:
#line 279 "src/parser/SysY.y"
    { (yyval.blockitem) = newBlockItem(BLOCKITEM,STMT,(yyvsp[(1) - (1)].stmt)); ;}
    break;

  case 51:
#line 283 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,ASSIGN,newAssign(ASSIGN,(yyvsp[(1) - (4)].lval),(yyvsp[(3) - (4)].exp))); ;}
    break;

  case 52:
#line 284 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,EXP,newExp(EXP,NULL)); ;}
    break;

  case 53:
#line 285 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,EXP,(yyvsp[(1) - (2)].exp)); ;}
    break;

  case 54:
#line 286 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,BLOCK,(yyvsp[(1) - (1)].block)); ;}
    break;

  case 55:
#line 287 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,IFSTMT,newIfStmt(IFSTMT,(yyvsp[(3) - (5)].cond),(yyvsp[(5) - (5)].stmt),NULL)); ;}
    break;

  case 56:
#line 288 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,IFSTMT,newIfStmt(IFSTMT,(yyvsp[(3) - (7)].cond),(yyvsp[(5) - (7)].stmt),(yyvsp[(7) - (7)].stmt))); ;}
    break;

  case 57:
#line 289 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,WHILESTMT,newWhileStmt(WHILESTMT,(yyvsp[(3) - (5)].cond),(yyvsp[(5) - (5)].stmt))); ;}
    break;

  case 58:
#line 290 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,BREAKSTMT,NULL); ;}
    break;

  case 59:
#line 291 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,CONTINUESTMT,NULL); ;}
    break;

  case 60:
#line 292 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,RETURNSTMT,newReturnStmt(RETURNSTMT,NULL)); ;}
    break;

  case 61:
#line 293 "src/parser/SysY.y"
    { (yyval.stmt) = newStmt(STMT,RETURNSTMT,newReturnStmt(RETURNSTMT,(yyvsp[(2) - (3)].exp))); ;}
    break;

  case 62:
#line 297 "src/parser/SysY.y"
    { (yyval.exp) = newExp(EXP,(yyvsp[(1) - (1)].addexp)->next); ;}
    break;

  case 63:
#line 301 "src/parser/SysY.y"
    { (yyval.cond) = newCond(COND,(yyvsp[(1) - (1)].lorexp)->next); ;}
    break;

  case 64:
#line 305 "src/parser/SysY.y"
    { (yyval.lval) = newLVal(LVAL,IDENT,(yyvsp[(1) - (1)].ident)); ;}
    break;

  case 65:
#line 306 "src/parser/SysY.y"
    { (yyval.lval) = newLVal(LVAL,ARRAYIMPL,newArrayImpl(ARRAYIMPL,(yyvsp[(1) - (2)].ident),(yyvsp[(2) - (2)].exparraydefs)->next)); ;}
    break;

  case 66:
#line 310 "src/parser/SysY.y"
    { (yyval.primaryexp) = newPrimaryExp(PRIMARYEXP,EXP,(yyvsp[(2) - (3)].exp)); ;}
    break;

  case 67:
#line 311 "src/parser/SysY.y"
    { (yyval.primaryexp) = newPrimaryExp(PRIMARYEXP,LVAL,(yyvsp[(1) - (1)].lval)); ;}
    break;

  case 68:
#line 312 "src/parser/SysY.y"
    { (yyval.primaryexp) = newPrimaryExp(PRIMARYEXP,NUMBER,(yyvsp[(1) - (1)].number)); ;}
    break;

  case 69:
#line 316 "src/parser/SysY.y"
    { (yyval.number) = newNumber(NUMBER,(yyvsp[(1) - (1)].intconst)); ;}
    break;

  case 70:
#line 320 "src/parser/SysY.y"
    { (yyval.unaryexp) = newUnaryExp(UNARYEXP,PRIMARYEXP,(yyvsp[(1) - (1)].primaryexp)); ;}
    break;

  case 71:
#line 321 "src/parser/SysY.y"
    { (yyval.unaryexp) = newUnaryExp(UNARYEXP,FUNCIMPL,newFuncImpl(FUNCIMPL,(yyvsp[(1) - (3)].ident),newFuncRParams(FUNCRPARAMS,NULL,NULL))); ;}
    break;

  case 72:
#line 322 "src/parser/SysY.y"
    { (yyval.unaryexp) = newUnaryExp(UNARYEXP,FUNCIMPL,newFuncImpl(FUNCIMPL,(yyvsp[(1) - (4)].ident),(yyvsp[(3) - (4)].funcrparams)->next)); ;}
    break;

  case 73:
#line 323 "src/parser/SysY.y"
    { (yyval.unaryexp) = newUnaryExp(UNARYEXP,UNARYEXPS,newUnaryExps(UNARYEXPS,(yyvsp[(1) - (2)].unaryop),(yyvsp[(2) - (2)].unaryexp))); ;}
    break;

  case 74:
#line 327 "src/parser/SysY.y"
    { (yyval.unaryop) = newUnaryOp(UNARYOP,K_ADD); ;}
    break;

  case 75:
#line 328 "src/parser/SysY.y"
    { (yyval.unaryop) = newUnaryOp(UNARYOP,K_SUB); ;}
    break;

  case 76:
#line 329 "src/parser/SysY.y"
    { (yyval.unaryop) = newUnaryOp(UNARYOP,K_NOT); ;}
    break;

  case 77:
#line 333 "src/parser/SysY.y"
    { (yyval.funcrparams) = newFuncRParams(FUNCRPARAMS,(yyvsp[(1) - (1)].funcrparam),NULL); ;}
    break;

  case 78:
#line 334 "src/parser/SysY.y"
    { (yyval.funcrparams) = newFuncRParams(FUNCRPARAMS,(yyvsp[(3) - (3)].funcrparam),(yyvsp[(1) - (3)].funcrparams)); ;}
    break;

  case 79:
#line 338 "src/parser/SysY.y"
    { (yyval.funcrparam) = newFuncRParam(FUNCRPARAM,EXP,(yyvsp[(1) - (1)].exp)); ;}
    break;

  case 80:
#line 339 "src/parser/SysY.y"
    { (yyval.funcrparam) = newFuncRParam(FUNCRPARAM,STRING,(yyvsp[(1) - (1)].string)); ;}
    break;

  case 81:
#line 342 "src/parser/SysY.y"
    { (yyval.mulexp) = newMulExp(MULEXP,NULL,(yyvsp[(1) - (1)].unaryexp),NULL); ;}
    break;

  case 82:
#line 343 "src/parser/SysY.y"
    { (yyval.mulexp) = newMulExp(MULEXP,(yyvsp[(2) - (3)].mulop),(yyvsp[(3) - (3)].unaryexp),(yyvsp[(1) - (3)].mulexp)); ;}
    break;

  case 83:
#line 347 "src/parser/SysY.y"
    { (yyval.addexp) = newAddExp(ADDEXP,NULL,(yyvsp[(1) - (1)].mulexp)->next,NULL); ;}
    break;

  case 84:
#line 348 "src/parser/SysY.y"
    { (yyval.addexp) = newAddExp(ADDEXP,newAddOp(ADDOP,K_ADD),(yyvsp[(3) - (3)].mulexp)->next,(yyvsp[(1) - (3)].addexp)); ;}
    break;

  case 85:
#line 349 "src/parser/SysY.y"
    { (yyval.addexp) = newAddExp(ADDEXP,newAddOp(ADDOP,K_SUB),(yyvsp[(3) - (3)].mulexp)->next,(yyvsp[(1) - (3)].addexp)); ;}
    break;

  case 86:
#line 353 "src/parser/SysY.y"
    { (yyval.relexp) = newRelExp(RELEXP,NULL,(yyvsp[(1) - (1)].addexp)->next,NULL); ;}
    break;

  case 87:
#line 354 "src/parser/SysY.y"
    { (yyval.relexp) = newRelExp(RELEXP,(yyvsp[(2) - (3)].relop),(yyvsp[(3) - (3)].addexp)->next,(yyvsp[(1) - (3)].relexp)); ;}
    break;

  case 88:
#line 358 "src/parser/SysY.y"
    { (yyval.eqexp) = newEqExp(EQEXP,NULL,(yyvsp[(1) - (1)].relexp)->next,NULL); ;}
    break;

  case 89:
#line 359 "src/parser/SysY.y"
    { (yyval.eqexp) = newEqExp(EQEXP,(yyvsp[(2) - (3)].eqop),(yyvsp[(3) - (3)].relexp)->next,(yyvsp[(1) - (3)].eqexp)); ;}
    break;

  case 90:
#line 363 "src/parser/SysY.y"
    { (yyval.landexp) = newLAndExp(LANDEXP,(yyvsp[(1) - (1)].eqexp)->next,NULL); ;}
    break;

  case 91:
#line 364 "src/parser/SysY.y"
    { (yyval.landexp) = newLAndExp(LANDEXP,(yyvsp[(3) - (3)].eqexp)->next,(yyvsp[(1) - (3)].landexp)); ;}
    break;

  case 92:
#line 368 "src/parser/SysY.y"
    { (yyval.lorexp) = newLOrExp(LOREXP,(yyvsp[(1) - (1)].landexp)->next,NULL); ;}
    break;

  case 93:
#line 369 "src/parser/SysY.y"
    { (yyval.lorexp) = newLOrExp(LOREXP,(yyvsp[(3) - (3)].landexp)->next,(yyvsp[(1) - (3)].lorexp)); ;}
    break;

  case 94:
#line 373 "src/parser/SysY.y"
    { (yyval.constexp) = newConstExp(CONSTEXP,(yyvsp[(1) - (1)].addexp)->next); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2111 "src/parser/SysY.tab.c"
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


#line 375 "src/parser/SysY.y"

