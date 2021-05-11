// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

// "%code top" blocks.
#line 33 "parser.y"

    #include <iostream>
    #include "parser.hpp"
    #include "lexer.hpp"
	#include "location.hh"
    using namespace std;
    using namespace saltyfish;

#line 47 "parser.cpp"




#include "parser.hpp"


// Unqualified %code blocks.
#line 42 "parser.y"

#define yylex lexer.get_next_token

#line 60 "parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 12 "parser.y"
namespace saltyfish {
#line 152 "parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
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
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_number_type
  Parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 35: // "标识符"
        value.YY_MOVE_OR_COPY< const char * > (YY_MOVE (that.value));
        break;

      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 35: // "标识符"
        value.move< const char * > (YY_MOVE (that.value));
        break;

      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.move< int > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 35: // "标识符"
        value.copy< const char * > (that.value);
        break;

      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.copy< int > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 35: // "标识符"
        value.move< const char * > (that.value);
        break;

      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.move< int > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 35: // "标识符"
        yylhs.value.emplace< const char * > ();
        break;

      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        yylhs.value.emplace< int > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 51:
#line 212 "parser.y"
                                                    {cout<< "if-else语句"<<endl;}
#line 628 "parser.cpp"
    break;


#line 632 "parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
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


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short Parser::yypact_ninf_ = -143;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     117,    46,    -3,    39,     9,  -143,  -143,  -143,  -143,    43,
      19,    11,  -143,    71,  -143,  -143,  -143,    18,    41,  -143,
     190,    47,   184,    24,    57,  -143,    69,   111,    30,    43,
    -143,   190,  -143,  -143,  -143,    56,  -143,  -143,  -143,  -143,
    -143,  -143,  -143,   190,   130,    29,    86,  -143,    94,    65,
       8,  -143,   166,  -143,  -143,    29,   190,   184,    35,  -143,
      94,    10,  -143,  -143,   111,  -143,    99,   190,    67,   107,
    -143,  -143,  -143,  -143,  -143,   190,  -143,  -143,   190,  -143,
      49,  -143,   113,   109,    94,  -143,  -143,   110,   128,  -143,
    -143,    94,  -143,  -143,   129,  -143,  -143,    14,  -143,  -143,
    -143,   130,  -143,  -143,    57,   142,   155,   161,   162,   148,
    -143,  -143,   106,  -143,  -143,   164,   169,   165,  -143,  -143,
     184,  -143,  -143,  -143,  -143,   190,  -143,   190,   190,  -143,
    -143,  -143,   177,  -143,  -143,  -143,   190,   107,  -143,  -143,
     172,    29,    68,   102,   157,   160,   181,  -143,   191,   107,
     141,  -143,  -143,  -143,  -143,   190,  -143,  -143,   190,   190,
     190,   141,  -143,   180,    29,    68,   102,   157,  -143,   141,
    -143
  };

  const signed char
  Parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     2,     6,     7,     3,     0,
      21,     0,    17,     0,     1,     5,     4,     0,     0,     9,
       0,     0,     0,    20,     0,    16,     0,     0,     0,     0,
       8,     0,    69,    70,    71,    60,    99,   100,   101,    62,
      65,    63,    75,     0,    80,    98,     0,    64,     0,     0,
       0,    32,     0,    22,    23,    57,     0,     0,    21,    18,
       0,     0,    12,    15,     0,    10,     0,     0,     0,    59,
      37,    68,    77,    78,    79,     0,    82,    83,     0,    14,
       0,    30,    34,     0,     0,    25,    26,     0,     0,    19,
      31,     0,    11,    61,     0,    67,    74,     0,    72,    38,
      76,    81,    48,    41,     0,     0,     0,     0,     0,     0,
      44,    49,     0,    42,    45,     0,    62,     0,    33,    28,
       0,    24,    13,    29,    39,     0,    66,     0,     0,    53,
      54,    55,     0,    40,    43,    47,     0,    36,    27,    73,
       0,    84,    90,    94,    96,    58,     0,    56,     0,    35,
       0,    86,    88,    87,    89,     0,    92,    93,     0,     0,
       0,     0,    46,    50,    85,    91,    95,    97,    52,     0,
      51
  };

  const short
  Parser::yypgoto_[] =
  {
    -143,  -143,    52,  -143,  -143,   168,   183,   134,  -143,  -143,
     185,   -47,  -143,   203,   182,   131,    76,   -68,   -44,  -143,
     103,  -142,   -19,    89,   -78,  -143,   -21,   -36,  -143,  -143,
      93,   145,  -143,   -20,  -143,    66,  -143,    70,  -143,    72,
    -143,   174,  -143
  };

  const short
  Parser::yydefgoto_[] =
  {
      -1,     4,   110,     6,    18,    19,    23,    62,     7,    11,
      12,    53,    87,     8,    50,    51,    69,    70,   111,   112,
     113,   114,   115,   140,    39,    40,    41,    42,    43,    97,
      98,    44,    75,    55,    78,   142,   155,   143,   158,   144,
     145,    46,    47
  };

  const unsigned char
  Parser::yytable_[] =
  {
      45,    99,   116,    54,    81,    86,    63,    71,   163,    14,
      89,    83,    66,    83,    24,    25,    90,   125,    84,   168,
      91,     1,     2,     3,   126,    20,    20,   170,    21,    27,
      22,    56,    10,    54,   116,    57,    45,    56,    54,   100,
     119,    64,    20,    63,    29,    30,    22,   123,    94,    96,
      76,    77,     5,   102,    80,   103,    15,    48,    31,     9,
      49,     1,   104,    67,   105,    68,   106,   107,   108,   109,
      32,    33,   116,   138,    13,    34,    31,    95,    17,    60,
      26,    99,    49,   116,    35,    36,    37,    38,    32,    33,
     132,   116,    58,    34,    79,   151,   152,   153,   154,    80,
      82,    54,    35,    36,    37,    38,    96,   141,   141,    93,
     102,    80,   133,   120,    67,    31,   121,   148,     1,   104,
     117,   105,    49,   106,   107,   108,   109,    32,    33,     1,
       2,     3,    34,   156,   157,   164,   122,   124,   141,   141,
     141,    35,    36,    37,    38,   102,    80,    36,    37,    38,
      31,   127,   131,    72,    73,    74,   105,    31,   106,   107,
     108,   109,    32,    33,   128,   129,   130,    34,   135,    32,
      33,    52,    85,   137,    34,    31,    35,    36,    37,    38,
     136,   147,   150,    35,    36,    37,    38,    32,    33,    52,
     159,   161,    34,    31,   160,   162,   169,    65,    92,    31,
      28,    35,    36,    37,    38,    32,    33,    16,    61,    59,
      34,    32,    33,   149,   118,   134,    34,   146,   139,    35,
      36,    37,    38,   101,   165,    35,    36,    37,    38,   166,
      88,     0,   167
  };

  const short
  Parser::yycheck_[] =
  {
      20,    69,    80,    22,    48,    52,    27,    43,   150,     0,
      57,     3,    31,     3,     3,     4,    60,     3,    10,   161,
      10,    12,    13,    14,    10,     7,     7,   169,     9,    11,
      11,     7,    35,    52,   112,    11,    56,     7,    57,    75,
      84,    11,     7,    64,     3,     4,    11,    91,    67,    68,
      21,    22,     0,     4,     5,     6,     4,    10,     9,    13,
      13,    12,    13,     7,    15,     9,    17,    18,    19,    20,
      21,    22,   150,   120,    35,    26,     9,    10,    35,    10,
       9,   149,    13,   161,    35,    36,    37,    38,    21,    22,
     109,   169,    35,    26,     8,    27,    28,    29,    30,     5,
      35,   120,    35,    36,    37,    38,   125,   127,   128,    10,
       4,     5,     6,     3,     7,     9,     6,   136,    12,    13,
       7,    15,    13,    17,    18,    19,    20,    21,    22,    12,
      13,    14,    26,    31,    32,   155,     8,     8,   158,   159,
     160,    35,    36,    37,    38,     4,     5,    36,    37,    38,
       9,     9,     4,    23,    24,    25,    15,     9,    17,    18,
      19,    20,    21,    22,     9,     4,     4,    26,     4,    21,
      22,     5,     6,     8,    26,     9,    35,    36,    37,    38,
      11,     4,    10,    35,    36,    37,    38,    21,    22,     5,
      33,    10,    26,     9,    34,     4,    16,    29,    64,     9,
      17,    35,    36,    37,    38,    21,    22,     4,    26,    24,
      26,    21,    22,   137,    83,   112,    26,   128,   125,    35,
      36,    37,    38,    78,   158,    35,    36,    37,    38,   159,
      56,    -1,   160
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    12,    13,    14,    41,    42,    43,    48,    53,    13,
      35,    49,    50,    35,     0,    42,    53,    35,    44,    45,
       7,     9,    11,    46,     3,     4,     9,    11,    46,     3,
       4,     9,    21,    22,    26,    35,    36,    37,    38,    64,
      65,    66,    67,    68,    71,    73,    81,    82,    10,    13,
      54,    55,     5,    51,    62,    73,     7,    11,    35,    50,
      10,    54,    47,    66,    11,    45,    62,     7,     9,    56,
      57,    67,    23,    24,    25,    72,    21,    22,    74,     8,
       5,    58,    35,     3,    10,     6,    51,    52,    81,    51,
      58,    10,    47,    10,    62,    10,    62,    69,    70,    57,
      67,    71,     4,     6,    13,    15,    17,    18,    19,    20,
      42,    58,    59,    60,    61,    62,    64,     7,    55,    58,
       3,     6,     8,    58,     8,     3,    10,     9,     9,     4,
       4,     4,    62,     6,    60,     4,    11,     8,    51,    70,
      63,    73,    75,    77,    79,    80,    63,     4,    62,    56,
      10,    27,    28,    29,    30,    76,    31,    32,    78,    33,
      34,    10,     4,    61,    73,    75,    77,    79,    61,    16,
      61
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    40,    41,    41,    41,    41,    42,    42,    43,    44,
      44,    45,    45,    46,    46,    47,    48,    49,    49,    50,
      50,    50,    50,    51,    51,    51,    52,    52,    53,    53,
      53,    53,    54,    54,    55,    55,    55,    56,    56,    57,
      58,    58,    59,    59,    60,    60,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    62,    63,    64,
      64,    65,    65,    65,    66,    67,    67,    67,    67,    68,
      68,    68,    69,    69,    70,    71,    71,    72,    72,    72,
      73,    73,    74,    74,    75,    75,    76,    76,    76,    76,
      77,    77,    78,    78,    79,    79,    80,    80,    81,    82,
      82,    82
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     1,     2,     2,     1,     1,     4,     1,
       3,     4,     3,     4,     3,     1,     3,     1,     3,     4,
       2,     1,     3,     1,     3,     2,     1,     3,     6,     6,
       5,     5,     1,     3,     2,     5,     4,     1,     2,     3,
       3,     2,     1,     2,     1,     1,     4,     2,     1,     1,
       5,     7,     5,     2,     2,     2,     3,     1,     1,     2,
       1,     3,     1,     1,     1,     1,     4,     3,     2,     1,
       1,     1,     1,     3,     1,     1,     3,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     3,     1,     1,
       1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"文件结尾\"", "error", "$undefined", "\"逗号\"", "\"分号\"",
  "\"左大括号\"", "\"右大括号\"", "\"左中括号\"",
  "\"右中括号\"", "\"左小括号\"", "\"右小括号\"", "\"等号\"",
  "\"const关键字\"", "\"int关键字\"", "\"void关键字\"",
  "\"if关键字\"", "\"else关键字\"", "\"while关键字\"",
  "\"break关键字\"", "\"continue关键字\"", "\"return关键字\"",
  "\"加号\"", "\"减号\"", "\"乘号\"", "\"除号\"", "\"余号\"",
  "\"感叹号\"", "\"小于号\"", "\"大于号\"", "\"小于等于号\"",
  "\"大于等于号\"", "\"等于等于号\"", "\"不等于号\"",
  "\"与号\"", "\"或号\"", "\"标识符\"", "\"十进制常数\"",
  "\"八进制常数\"", "\"十六进制常数\"", "K_LOW_THAN_ELSE",
  "$accept", "CompUnit", "Decl", "ConstDecl", "ConstDefs", "ConstDef",
  "ArrayDef", "ConstInitVal", "VarDecl", "VarDefs", "VarDef", "InitVal",
  "InitVals", "FuncDef", "FuncFParams", "FuncFParam", "ExpArrayDefs",
  "ExpArrayDef", "Block", "BlockItems", "BlockItem", "Stmt", "Exp",
  "Condition", "LVal", "PrimaryExp", "Number", "UnaryExp", "UnaryOp",
  "FuncRParams", "FuncRParam", "MulExp", "MulOp", "AddExp", "AddOp",
  "RelExp", "RelOp", "EqExp", "EqOp", "LAndExp", "LOrExp", "ConstExp",
  "IntConst", YY_NULLPTR
  };

#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   100,   100,   101,   102,   103,   107,   108,   113,   117,
     118,   122,   123,   127,   128,   132,   136,   140,   141,   145,
     146,   147,   148,   153,   154,   155,   159,   160,   165,   166,
     167,   168,   172,   173,   177,   178,   179,   183,   184,   188,
     192,   193,   197,   198,   202,   203,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   221,   225,   229,
     230,   234,   235,   236,   240,   244,   245,   246,   247,   251,
     252,   253,   257,   258,   262,   266,   267,   271,   272,   273,
     277,   278,   282,   283,   287,   288,   292,   293,   294,   295,
     299,   300,   304,   305,   309,   310,   314,   315,   319,   323,
     324,   325
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 12 "parser.y"
} // saltyfish
#line 1163 "parser.cpp"

#line 328 "parser.y"


namespace saltyfish
{
  auto Parser::error (const std::string& msg) -> void
  {
    std::cerr << msg << '\n';
  }
}

int main ()
{
  saltyfish::Parser parser;
  return parser.parse();
}


