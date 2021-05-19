/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "flex.bison/SysY.y"

    #include "./SysY.tab.h"
    #include "../SysY.type/SysY.type.def.h"
    #include "../SysY.type/SysY.type.new.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    extern YYSTYPE result;
    int yylex();
    void yyerror(char*);

#line 82 "flex.bison/SysY.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_FLEX_BISON_SYSY_TAB_H_INCLUDED
# define YY_YY_FLEX_BISON_SYSY_TAB_H_INCLUDED
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "flex.bison/SysY.y"

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

#line 241 "flex.bison/SysY.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_FLEX_BISON_SYSY_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

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
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

#define YYUNDEFTOK  2
#define YYMAXUTOK   299


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   167,   167,   168,   169,   170,   174,   175,   179,   183,
     184,   188,   189,   193,   194,   198,   202,   203,   204,   208,
     209,   213,   217,   218,   222,   223,   224,   225,   229,   230,
     231,   235,   236,   240,   241,   242,   243,   247,   248,   252,
     253,   254,   258,   259,   263,   267,   268,   272,   273,   277,
     278,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   296,   300,   304,   305,   309,   310,   311,   315,
     319,   320,   321,   322,   326,   327,   328,   332,   333,   337,
     338,   341,   342,   346,   347,   348,   352,   353,   357,   358,
     362,   363,   367,   368,   372
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
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
  "EQEXP", "LANDEXP", "LOREXP", "CONSTEXP", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299
};
# endif

#define YYPACT_NINF (-140)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
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

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
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

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -140,  -140,   112,  -140,  -140,   164,   179,    61,   -54,  -140,
    -140,  -140,   173,   -48,  -140,   200,   184,   136,    73,   -70,
     -27,  -140,   108,  -139,   -22,    85,   -63,  -140,  -140,   -30,
    -140,  -140,    75,    89,   -21,    56,    57,    59,  -140,   196
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,    98,     6,    18,    19,    24,    25,    61,    88,
       7,    12,    13,    53,    84,     8,    34,    35,    73,    74,
      99,   100,   101,   102,   103,   140,    42,    43,    44,    45,
      46,   113,   114,    47,    55,   142,   143,   144,   145,    62
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
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
static const yytype_int8 yystos[] =
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

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
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

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
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


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 2:
#line 167 "flex.bison/SysY.y"
                { (yyval.compunit) = newCompUnit(COMPUNIT,DECL,(yyvsp[0].decl),NULL); }
#line 1559 "flex.bison/SysY.tab.c"
    break;

  case 3:
#line 168 "flex.bison/SysY.y"
                { (yyval.compunit) = newCompUnit(COMPUNIT,FUNCDEF,(yyvsp[0].funcdef),NULL); }
#line 1565 "flex.bison/SysY.tab.c"
    break;

  case 4:
#line 169 "flex.bison/SysY.y"
                        { (yyval.compunit) = newCompUnit(COMPUNIT,DECL,(yyvsp[0].decl),(yyvsp[-1].compunit)); result.compunit = (yyval.compunit)->next; }
#line 1571 "flex.bison/SysY.tab.c"
    break;

  case 5:
#line 170 "flex.bison/SysY.y"
                            { (yyval.compunit) = newCompUnit(COMPUNIT,FUNCDEF,(yyvsp[0].funcdef),(yyvsp[-1].compunit)); result.compunit = (yyval.compunit)->next; }
#line 1577 "flex.bison/SysY.tab.c"
    break;

  case 6:
#line 174 "flex.bison/SysY.y"
                    { (yyval.decl) = newDecl(DECL,CONSTDECL,(yyvsp[0].constdecl)); }
#line 1583 "flex.bison/SysY.tab.c"
    break;

  case 7:
#line 175 "flex.bison/SysY.y"
                { (yyval.decl) = newDecl(DECL,VARDECL,(yyvsp[0].vardecl)); }
#line 1589 "flex.bison/SysY.tab.c"
    break;

  case 8:
#line 179 "flex.bison/SysY.y"
                                            { (yyval.constdecl) = newConstDecl(CONSTDECL,newBType(BTYPE,K_INT),(yyvsp[-1].constdefs)->next); }
#line 1595 "flex.bison/SysY.tab.c"
    break;

  case 9:
#line 183 "flex.bison/SysY.y"
                    { (yyval.constdefs) = newConstDefs(CONSTDEFS,(yyvsp[0].constdef),NULL); }
#line 1601 "flex.bison/SysY.tab.c"
    break;

  case 10:
#line 184 "flex.bison/SysY.y"
                                    { (yyval.constdefs) = newConstDefs(CONSTDEFS,(yyvsp[0].constdef),(yyvsp[-2].constdefs)); }
#line 1607 "flex.bison/SysY.tab.c"
    break;

  case 11:
#line 188 "flex.bison/SysY.y"
                                        { (yyval.constdef) = newConstDef(CONSTDEF,(yyvsp[-2].ident),NULL,(yyvsp[0].constinitval)); }
#line 1613 "flex.bison/SysY.tab.c"
    break;

  case 12:
#line 189 "flex.bison/SysY.y"
                                                        { (yyval.constdef) = newConstDef(CONSTDEF,(yyvsp[-3].ident),(yyvsp[-2].constarraydefs)->next,(yyvsp[0].constinitval)); }
#line 1619 "flex.bison/SysY.tab.c"
    break;

  case 13:
#line 193 "flex.bison/SysY.y"
                        { (yyval.constarraydefs) = newConstArrayDefs(CONSTARRAYDEFS,(yyvsp[0].constarraydef),NULL); }
#line 1625 "flex.bison/SysY.tab.c"
    break;

  case 14:
#line 194 "flex.bison/SysY.y"
                                        { (yyval.constarraydefs) = newConstArrayDefs(CONSTARRAYDEFS,(yyvsp[0].constarraydef),(yyvsp[-1].constarraydefs)); }
#line 1631 "flex.bison/SysY.tab.c"
    break;

  case 15:
#line 198 "flex.bison/SysY.y"
                                        { (yyval.constarraydef) = newConstArrayDef(CONSTARRAYDEF,(yyvsp[-1].constexp)); }
#line 1637 "flex.bison/SysY.tab.c"
    break;

  case 16:
#line 202 "flex.bison/SysY.y"
                    { (yyval.constinitval) = newConstInitVal(CONSTINITVAL,CONSTEXP,(yyvsp[0].constexp)); }
#line 1643 "flex.bison/SysY.tab.c"
    break;

  case 17:
#line 203 "flex.bison/SysY.y"
                            { (yyval.constinitval) = newConstInitVal(CONSTINITVAL,CONSTINITVALS,newConstInitVals(CONSTINITVALS,NULL,NULL)); }
#line 1649 "flex.bison/SysY.tab.c"
    break;

  case 18:
#line 204 "flex.bison/SysY.y"
                                            { (yyval.constinitval) = newConstInitVal(CONSTINITVAL,CONSTINITVALS,(yyvsp[-1].constinitvals)->next); }
#line 1655 "flex.bison/SysY.tab.c"
    break;

  case 19:
#line 208 "flex.bison/SysY.y"
                        { (yyval.constinitvals) = newConstInitVals(CONSTINITVALS,(yyvsp[0].constinitval),NULL); }
#line 1661 "flex.bison/SysY.tab.c"
    break;

  case 20:
#line 209 "flex.bison/SysY.y"
                                            { (yyval.constinitvals) = newConstInitVals(CONSTINITVALS,(yyvsp[0].constinitval),(yyvsp[-2].constinitvals)); }
#line 1667 "flex.bison/SysY.tab.c"
    break;

  case 21:
#line 213 "flex.bison/SysY.y"
                                    { (yyval.vardecl) = newVarDecl(VARDECL,newBType(BTYPE,K_INT),(yyvsp[-1].vardefs)->next); }
#line 1673 "flex.bison/SysY.tab.c"
    break;

  case 22:
#line 217 "flex.bison/SysY.y"
                { (yyval.vardefs) = newVarDefs(VARDEFS,(yyvsp[0].vardef),NULL); }
#line 1679 "flex.bison/SysY.tab.c"
    break;

  case 23:
#line 218 "flex.bison/SysY.y"
                                { (yyval.vardefs) = newVarDefs(VARDEFS,(yyvsp[0].vardef),(yyvsp[-2].vardefs)); }
#line 1685 "flex.bison/SysY.tab.c"
    break;

  case 24:
#line 222 "flex.bison/SysY.y"
                { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[0].ident),NULL,NULL); }
#line 1691 "flex.bison/SysY.tab.c"
    break;

  case 25:
#line 223 "flex.bison/SysY.y"
                                { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[-1].ident),(yyvsp[0].constarraydefs)->next,NULL); }
#line 1697 "flex.bison/SysY.tab.c"
    break;

  case 26:
#line 224 "flex.bison/SysY.y"
                                    { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[-2].ident),NULL,(yyvsp[0].initval)); }
#line 1703 "flex.bison/SysY.tab.c"
    break;

  case 27:
#line 225 "flex.bison/SysY.y"
                                                { (yyval.vardef) = newVarDef(VARDEF,(yyvsp[-3].ident),(yyvsp[-2].constarraydefs)->next,(yyvsp[0].initval)); }
#line 1709 "flex.bison/SysY.tab.c"
    break;

  case 28:
#line 229 "flex.bison/SysY.y"
            { (yyval.initval) = newInitVal(INITVAL,EXP,(yyvsp[0].exp)); }
#line 1715 "flex.bison/SysY.tab.c"
    break;

  case 29:
#line 230 "flex.bison/SysY.y"
                            { (yyval.initval) = newInitVal(INITVAL,INITVAL,newInitVals(INITVALS,NULL,NULL)); }
#line 1721 "flex.bison/SysY.tab.c"
    break;

  case 30:
#line 231 "flex.bison/SysY.y"
                                        { (yyval.initval) = newInitVal(INITVAL,INITVAL,(yyvsp[-1].initvals)->next); }
#line 1727 "flex.bison/SysY.tab.c"
    break;

  case 31:
#line 235 "flex.bison/SysY.y"
                { (yyval.initvals) = newInitVals(INITVALS,(yyvsp[0].initval),NULL); }
#line 1733 "flex.bison/SysY.tab.c"
    break;

  case 32:
#line 236 "flex.bison/SysY.y"
                                    { (yyval.initvals) = newInitVals(INITVALS,(yyvsp[0].initval),(yyvsp[-2].initvals)); }
#line 1739 "flex.bison/SysY.tab.c"
    break;

  case 33:
#line 240 "flex.bison/SysY.y"
                                                            { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_INT),(yyvsp[-3].ident),newFuncFParams(FUNCFPARAMS,NULL,NULL),(yyvsp[0].block)); }
#line 1745 "flex.bison/SysY.tab.c"
    break;

  case 34:
#line 241 "flex.bison/SysY.y"
                                                            { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_VOID),(yyvsp[-3].ident),newFuncFParams(FUNCFPARAMS,NULL,NULL),(yyvsp[0].block)); }
#line 1751 "flex.bison/SysY.tab.c"
    break;

  case 35:
#line 242 "flex.bison/SysY.y"
                                                                        { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_INT),(yyvsp[-4].ident),(yyvsp[-2].funcfparams)->next,(yyvsp[0].block)); }
#line 1757 "flex.bison/SysY.tab.c"
    break;

  case 36:
#line 243 "flex.bison/SysY.y"
                                                                        { (yyval.funcdef) = newFuncDef(FUNCDEF,newFuncType(FUNCTYPE,K_VOID),(yyvsp[-4].ident),(yyvsp[-2].funcfparams)->next,(yyvsp[0].block)); }
#line 1763 "flex.bison/SysY.tab.c"
    break;

  case 37:
#line 247 "flex.bison/SysY.y"
                    { (yyval.funcfparams) = newFuncFParams(FUNCFPARAMS,(yyvsp[0].funcfparam),NULL); }
#line 1769 "flex.bison/SysY.tab.c"
    break;

  case 38:
#line 248 "flex.bison/SysY.y"
                                        { (yyval.funcfparams) = newFuncFParams(FUNCFPARAMS,(yyvsp[0].funcfparam),(yyvsp[-2].funcfparams)); }
#line 1775 "flex.bison/SysY.tab.c"
    break;

  case 39:
#line 252 "flex.bison/SysY.y"
                    { (yyval.funcfparam) = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),(yyvsp[0].ident),NULL); }
#line 1781 "flex.bison/SysY.tab.c"
    break;

  case 40:
#line 253 "flex.bison/SysY.y"
                                            { (yyval.funcfparam) = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),(yyvsp[-2].ident),newExpArrayDefs(EXPARRAYDEFS,newExpArrayDef(EXPARRAYDEF,NULL),NULL)); }
#line 1787 "flex.bison/SysY.tab.c"
    break;

  case 41:
#line 254 "flex.bison/SysY.y"
                                                        { (yyval.funcfparam) = newFuncFParam(FUNCFPARAM,newBType(BTYPE,K_INT),(yyvsp[-3].ident),newExpArrayDefs(EXPARRAYDEFS,newExpArrayDef(EXPARRAYDEF,NULL),(yyvsp[0].exparraydefs)->next)); }
#line 1793 "flex.bison/SysY.tab.c"
    break;

  case 42:
#line 258 "flex.bison/SysY.y"
                    { (yyval.exparraydefs) = newExpArrayDefs(EXPARRAYDEFS,(yyvsp[0].exparraydef),NULL); }
#line 1799 "flex.bison/SysY.tab.c"
    break;

  case 43:
#line 259 "flex.bison/SysY.y"
                                    { (yyval.exparraydefs) = newExpArrayDefs(EXPARRAYDEFS,(yyvsp[0].exparraydef),(yyvsp[-1].exparraydefs)); }
#line 1805 "flex.bison/SysY.tab.c"
    break;

  case 44:
#line 263 "flex.bison/SysY.y"
                                    { (yyval.exparraydef) = newExpArrayDef(EXPARRAYDEF,(yyvsp[-1].exp)); }
#line 1811 "flex.bison/SysY.tab.c"
    break;

  case 45:
#line 267 "flex.bison/SysY.y"
                            { (yyval.block) = newBlock(BLOCK,NULL); }
#line 1817 "flex.bison/SysY.tab.c"
    break;

  case 46:
#line 268 "flex.bison/SysY.y"
                                        { (yyval.block) = newBlock(BLOCK,(yyvsp[-1].blockitems)->next); }
#line 1823 "flex.bison/SysY.tab.c"
    break;

  case 47:
#line 272 "flex.bison/SysY.y"
                    { (yyval.blockitems) = newBlockItems(BLOCKITEMS,(yyvsp[0].blockitem),NULL); }
#line 1829 "flex.bison/SysY.tab.c"
    break;

  case 48:
#line 273 "flex.bison/SysY.y"
                                { (yyval.blockitems) = newBlockItems(BLOCKITEMS,(yyvsp[0].blockitem),(yyvsp[-1].blockitems)); }
#line 1835 "flex.bison/SysY.tab.c"
    break;

  case 49:
#line 277 "flex.bison/SysY.y"
                { (yyval.blockitem) = newBlockItem(BLOCKITEM,DECL,(yyvsp[0].decl)); }
#line 1841 "flex.bison/SysY.tab.c"
    break;

  case 50:
#line 278 "flex.bison/SysY.y"
                { (yyval.blockitem) = newBlockItem(BLOCKITEM,STMT,(yyvsp[0].stmt)); }
#line 1847 "flex.bison/SysY.tab.c"
    break;

  case 51:
#line 282 "flex.bison/SysY.y"
                                        { (yyval.stmt) = newStmt(STMT,ASSIGN,newAssign(ASSIGN,(yyvsp[-3].lval),(yyvsp[-1].exp))); }
#line 1853 "flex.bison/SysY.tab.c"
    break;

  case 52:
#line 283 "flex.bison/SysY.y"
                    { (yyval.stmt) = newStmt(STMT,EXP,newExp(EXP,NULL)); }
#line 1859 "flex.bison/SysY.tab.c"
    break;

  case 53:
#line 284 "flex.bison/SysY.y"
                        { (yyval.stmt) = newStmt(STMT,EXP,(yyvsp[-1].exp)); }
#line 1865 "flex.bison/SysY.tab.c"
    break;

  case 54:
#line 285 "flex.bison/SysY.y"
                { (yyval.stmt) = newStmt(STMT,BLOCK,(yyvsp[0].block)); }
#line 1871 "flex.bison/SysY.tab.c"
    break;

  case 55:
#line 286 "flex.bison/SysY.y"
                                                                             { (yyval.stmt) = newStmt(STMT,IFSTMT,newIfStmt(IFSTMT,(yyvsp[-2].cond),(yyvsp[0].stmt),NULL)); }
#line 1877 "flex.bison/SysY.tab.c"
    break;

  case 56:
#line 287 "flex.bison/SysY.y"
                                                                    { (yyval.stmt) = newStmt(STMT,IFSTMT,newIfStmt(IFSTMT,(yyvsp[-4].cond),(yyvsp[-2].stmt),(yyvsp[0].stmt))); }
#line 1883 "flex.bison/SysY.tab.c"
    break;

  case 57:
#line 288 "flex.bison/SysY.y"
                                                            { (yyval.stmt) = newStmt(STMT,WHILESTMT,newWhileStmt(WHILESTMT,(yyvsp[-2].cond),(yyvsp[0].stmt))); }
#line 1889 "flex.bison/SysY.tab.c"
    break;

  case 58:
#line 289 "flex.bison/SysY.y"
                            { (yyval.stmt) = newStmt(STMT,BREAKSTMT,NULL); }
#line 1895 "flex.bison/SysY.tab.c"
    break;

  case 59:
#line 290 "flex.bison/SysY.y"
                                { (yyval.stmt) = newStmt(STMT,CONTINUESTMT,NULL); }
#line 1901 "flex.bison/SysY.tab.c"
    break;

  case 60:
#line 291 "flex.bison/SysY.y"
                                { (yyval.stmt) = newStmt(STMT,RETURNSTMT,newReturnStmt(RETURNSTMT,NULL)); }
#line 1907 "flex.bison/SysY.tab.c"
    break;

  case 61:
#line 292 "flex.bison/SysY.y"
                                    { (yyval.stmt) = newStmt(STMT,RETURNSTMT,newReturnStmt(RETURNSTMT,(yyvsp[-1].exp))); }
#line 1913 "flex.bison/SysY.tab.c"
    break;

  case 62:
#line 296 "flex.bison/SysY.y"
                { (yyval.exp) = newExp(EXP,(yyvsp[0].addexp)->next); }
#line 1919 "flex.bison/SysY.tab.c"
    break;

  case 63:
#line 300 "flex.bison/SysY.y"
                { (yyval.cond) = newCond(COND,(yyvsp[0].lorexp)->next); }
#line 1925 "flex.bison/SysY.tab.c"
    break;

  case 64:
#line 304 "flex.bison/SysY.y"
                { (yyval.lval) = newLVal(LVAL,IDENT,(yyvsp[0].ident)); }
#line 1931 "flex.bison/SysY.tab.c"
    break;

  case 65:
#line 305 "flex.bison/SysY.y"
                            { (yyval.lval) = newLVal(LVAL,ARRAYIMPL,newArrayImpl(ARRAYIMPL,(yyvsp[-1].ident),(yyvsp[0].exparraydefs)->next)); }
#line 1937 "flex.bison/SysY.tab.c"
    break;

  case 66:
#line 309 "flex.bison/SysY.y"
                                            { (yyval.primaryexp) = newPrimaryExp(PRIMARYEXP,EXP,(yyvsp[-1].exp)); }
#line 1943 "flex.bison/SysY.tab.c"
    break;

  case 67:
#line 310 "flex.bison/SysY.y"
                { (yyval.primaryexp) = newPrimaryExp(PRIMARYEXP,LVAL,(yyvsp[0].lval)); }
#line 1949 "flex.bison/SysY.tab.c"
    break;

  case 68:
#line 311 "flex.bison/SysY.y"
                { (yyval.primaryexp) = newPrimaryExp(PRIMARYEXP,NUMBER,(yyvsp[0].number)); }
#line 1955 "flex.bison/SysY.tab.c"
    break;

  case 69:
#line 315 "flex.bison/SysY.y"
                    { (yyval.number) = newNumber(NUMBER,(yyvsp[0].intconst)); }
#line 1961 "flex.bison/SysY.tab.c"
    break;

  case 70:
#line 319 "flex.bison/SysY.y"
                    { (yyval.unaryexp) = newUnaryExp(UNARYEXP,PRIMARYEXP,(yyvsp[0].primaryexp)); }
#line 1967 "flex.bison/SysY.tab.c"
    break;

  case 71:
#line 320 "flex.bison/SysY.y"
                                                { (yyval.unaryexp) = newUnaryExp(UNARYEXP,FUNCIMPL,newFuncImpl(FUNCIMPL,(yyvsp[-2].ident),newFuncRParams(FUNCRPARAMS,NULL,NULL))); }
#line 1973 "flex.bison/SysY.tab.c"
    break;

  case 72:
#line 321 "flex.bison/SysY.y"
                                                            { (yyval.unaryexp) = newUnaryExp(UNARYEXP,FUNCIMPL,newFuncImpl(FUNCIMPL,(yyvsp[-3].ident),(yyvsp[-1].funcrparams)->next)); }
#line 1979 "flex.bison/SysY.tab.c"
    break;

  case 73:
#line 322 "flex.bison/SysY.y"
                            { (yyval.unaryexp) = newUnaryExp(UNARYEXP,UNARYEXPS,newUnaryExps(UNARYEXPS,(yyvsp[-1].unaryop),(yyvsp[0].unaryexp))); }
#line 1985 "flex.bison/SysY.tab.c"
    break;

  case 74:
#line 326 "flex.bison/SysY.y"
                { (yyval.unaryop) = newUnaryOp(UNARYOP,K_ADD); }
#line 1991 "flex.bison/SysY.tab.c"
    break;

  case 75:
#line 327 "flex.bison/SysY.y"
                { (yyval.unaryop) = newUnaryOp(UNARYOP,K_SUB); }
#line 1997 "flex.bison/SysY.tab.c"
    break;

  case 76:
#line 328 "flex.bison/SysY.y"
                { (yyval.unaryop) = newUnaryOp(UNARYOP,K_NOT); }
#line 2003 "flex.bison/SysY.tab.c"
    break;

  case 77:
#line 332 "flex.bison/SysY.y"
                    { (yyval.funcrparams) = newFuncRParams(FUNCRPARAMS,(yyvsp[0].funcrparam),NULL); }
#line 2009 "flex.bison/SysY.tab.c"
    break;

  case 78:
#line 333 "flex.bison/SysY.y"
                                        { (yyval.funcrparams) = newFuncRParams(FUNCRPARAMS,(yyvsp[0].funcrparam),(yyvsp[-2].funcrparams)); }
#line 2015 "flex.bison/SysY.tab.c"
    break;

  case 79:
#line 337 "flex.bison/SysY.y"
            { (yyval.funcrparam) = newFuncRParam(FUNCRPARAM,EXP,(yyvsp[0].exp)); }
#line 2021 "flex.bison/SysY.tab.c"
    break;

  case 80:
#line 338 "flex.bison/SysY.y"
                { (yyval.funcrparam) = newFuncRParam(FUNCRPARAM,STRING,(yyvsp[0].string)); }
#line 2027 "flex.bison/SysY.tab.c"
    break;

  case 81:
#line 341 "flex.bison/SysY.y"
                    { (yyval.mulexp) = newMulExp(MULEXP,NULL,(yyvsp[0].unaryexp),NULL); }
#line 2033 "flex.bison/SysY.tab.c"
    break;

  case 82:
#line 342 "flex.bison/SysY.y"
                                { (yyval.mulexp) = newMulExp(MULEXP,(yyvsp[-1].mulop),(yyvsp[0].unaryexp),(yyvsp[-2].mulexp)); }
#line 2039 "flex.bison/SysY.tab.c"
    break;

  case 83:
#line 346 "flex.bison/SysY.y"
                { (yyval.addexp) = newAddExp(ADDEXP,NULL,(yyvsp[0].mulexp)->next,NULL); }
#line 2045 "flex.bison/SysY.tab.c"
    break;

  case 84:
#line 347 "flex.bison/SysY.y"
                            { (yyval.addexp) = newAddExp(ADDEXP,newAddOp(ADDOP,K_ADD),(yyvsp[0].mulexp)->next,(yyvsp[-2].addexp)); }
#line 2051 "flex.bison/SysY.tab.c"
    break;

  case 85:
#line 348 "flex.bison/SysY.y"
                            { (yyval.addexp) = newAddExp(ADDEXP,newAddOp(ADDOP,K_SUB),(yyvsp[0].mulexp)->next,(yyvsp[-2].addexp)); }
#line 2057 "flex.bison/SysY.tab.c"
    break;

  case 86:
#line 352 "flex.bison/SysY.y"
                { (yyval.relexp) = newRelExp(RELEXP,NULL,(yyvsp[0].addexp)->next,NULL); }
#line 2063 "flex.bison/SysY.tab.c"
    break;

  case 87:
#line 353 "flex.bison/SysY.y"
                            { (yyval.relexp) = newRelExp(RELEXP,(yyvsp[-1].relop),(yyvsp[0].addexp)->next,(yyvsp[-2].relexp)); }
#line 2069 "flex.bison/SysY.tab.c"
    break;

  case 88:
#line 357 "flex.bison/SysY.y"
                { (yyval.eqexp) = newEqExp(EQEXP,NULL,(yyvsp[0].relexp)->next,NULL); }
#line 2075 "flex.bison/SysY.tab.c"
    break;

  case 89:
#line 358 "flex.bison/SysY.y"
                            { (yyval.eqexp) = newEqExp(EQEXP,(yyvsp[-1].eqop),(yyvsp[0].relexp)->next,(yyvsp[-2].eqexp)); }
#line 2081 "flex.bison/SysY.tab.c"
    break;

  case 90:
#line 362 "flex.bison/SysY.y"
                { (yyval.landexp) = newLAndExp(LANDEXP,(yyvsp[0].eqexp)->next,NULL); }
#line 2087 "flex.bison/SysY.tab.c"
    break;

  case 91:
#line 363 "flex.bison/SysY.y"
                               { (yyval.landexp) = newLAndExp(LANDEXP,(yyvsp[0].eqexp)->next,(yyvsp[-2].landexp)); }
#line 2093 "flex.bison/SysY.tab.c"
    break;

  case 92:
#line 367 "flex.bison/SysY.y"
                { (yyval.lorexp) = newLOrExp(LOREXP,(yyvsp[0].landexp)->next,NULL); }
#line 2099 "flex.bison/SysY.tab.c"
    break;

  case 93:
#line 368 "flex.bison/SysY.y"
                               { (yyval.lorexp) = newLOrExp(LOREXP,(yyvsp[0].landexp)->next,(yyvsp[-2].lorexp)); }
#line 2105 "flex.bison/SysY.tab.c"
    break;

  case 94:
#line 372 "flex.bison/SysY.y"
                { (yyval.constexp) = newConstExp(CONSTEXP,(yyvsp[0].addexp)->next); }
#line 2111 "flex.bison/SysY.tab.c"
    break;


#line 2115 "flex.bison/SysY.tab.c"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp);
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
#line 374 "flex.bison/SysY.y"
