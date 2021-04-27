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
#line 29 "parser.y"

    #include <iostream>
    #include "parser.hpp"
    #include "lexer.hpp"

    using namespace saltyfish;

#line 46 "parser.cpp"




#include "parser.hpp"


// Unqualified %code blocks.
#line 82 "parser.y"

  namespace saltyfish
  {
    saltyfish::Lexer lexer;
    // Return the next token.
    auto yylex () -> Parser::symbol_type
    {      
      return lexer.get_next_token();
    }
  }

#line 67 "parser.cpp"


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
#line 140 "parser.cpp"


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
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 35: // "标识符"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
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
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.move< int > (YY_MOVE (that.value));
        break;

      case 35: // "标识符"
        value.move< std::string > (YY_MOVE (that.value));
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
      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.copy< int > (that.value);
        break;

      case 35: // "标识符"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        value.move< int > (that.value);
        break;

      case 35: // "标识符"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

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
        << ' ' << yytname_[yytype] << " (";
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
      case 36: // "十进制常数"
      case 37: // "八进制常数"
      case 38: // "十六进制常数"
        yylhs.value.emplace< int > ();
        break;

      case 35: // "标识符"
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 7:
#line 106 "parser.y"
                                                {cout<<"常量声明"<<endl;}
#line 604 "parser.cpp"
    break;

  case 12:
#line 119 "parser.y"
                {cout<<"数字"<<endl;}
#line 610 "parser.cpp"
    break;

  case 13:
#line 122 "parser.y"
                                       {cout<<"变量声明"<<endl;}
#line 616 "parser.cpp"
    break;

  case 22:
#line 141 "parser.y"
                                 {cout<<"函数无参声明"<<endl;}
#line 622 "parser.cpp"
    break;

  case 23:
#line 142 "parser.y"
                                             {cout<<"函数有参声明"<<endl;}
#line 628 "parser.cpp"
    break;

  case 29:
#line 155 "parser.y"
            {cout<<"空块"<<endl;}
#line 634 "parser.cpp"
    break;

  case 35:
#line 164 "parser.y"
                                        {cout<<"if else语句"<<endl;}
#line 640 "parser.cpp"
    break;

  case 36:
#line 165 "parser.y"
                                        {cout<<"if语句"<<endl;}
#line 646 "parser.cpp"
    break;

  case 37:
#line 166 "parser.y"
                                        {cout<<"while语句"<<endl;}
#line 652 "parser.cpp"
    break;

  case 38:
#line 167 "parser.y"
                                        {cout<<"break语句"<<endl;}
#line 658 "parser.cpp"
    break;

  case 39:
#line 168 "parser.y"
                                        {cout<<"continue语句"<<endl;}
#line 664 "parser.cpp"
    break;

  case 40:
#line 169 "parser.y"
                                        {cout<<"return sth.语句"<<endl;}
#line 670 "parser.cpp"
    break;

  case 41:
#line 170 "parser.y"
                                        {cout<<"return语句"<<endl;}
#line 676 "parser.cpp"
    break;

  case 60:
#line 201 "parser.y"
                        {cout<<"乘法"<<endl;}
#line 682 "parser.cpp"
    break;

  case 64:
#line 207 "parser.y"
                      {cout<<"加法"<<endl;}
#line 688 "parser.cpp"
    break;

  case 72:
#line 219 "parser.y"
                    {cout<<"逻辑等于表达式"<<endl;}
#line 694 "parser.cpp"
    break;

  case 73:
#line 220 "parser.y"
                    {cout<<"逻辑不等于表达与"<<endl;}
#line 700 "parser.cpp"
    break;

  case 75:
#line 224 "parser.y"
                      {cout<<"逻辑与表达式"<<endl;}
#line 706 "parser.cpp"
    break;

  case 77:
#line 228 "parser.y"
                      {cout<<"逻辑或表达式"<<endl;}
#line 712 "parser.cpp"
    break;


#line 716 "parser.cpp"

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
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


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

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


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
    error (yyexc.what ());
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


  const signed char Parser::yypact_ninf_ = -82;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -82,    59,   -82,    -5,   -82,   -82,   -82,   -82,   -25,   -82,
     -82,   -23,   -82,   -21,    18,    54,   -82,    25,    27,    61,
     -82,    10,   -25,   -82,     3,    60,   -21,   -82,    48,   -82,
     -82,   -82,    31,   -82,   -82,   -82,   -82,   -82,   -82,   -82,
     -82,   -82,    48,    37,    57,   -82,    47,    20,    -1,   -82,
     -82,   -82,   -82,    48,    24,    36,    48,   -82,    48,    48,
      48,    48,    48,    75,   -82,   -82,    -5,    47,    70,    48,
     -82,    48,   -82,   -82,     8,   -82,   -82,   -82,    37,    37,
     -82,    58,    73,    87,    96,     2,   -82,   -82,   -82,   103,
     -82,   100,   -82,   -82,   -82,   -82,    48,   -82,    48,    48,
     -82,   -82,   -82,   114,   -82,    48,   -82,   110,    57,    85,
      76,    88,    98,   123,   -82,   130,    84,    48,    48,    48,
      48,    48,    48,    48,    48,    84,   -82,   119,    57,    57,
      57,    57,    85,    85,    76,    88,   -82,    84,   -82
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     1,     0,    10,    24,     3,     5,     0,     6,
       4,     0,    10,     0,    16,     0,    14,     0,     0,     0,
       8,     0,     0,    13,     0,     0,     0,     7,     0,    54,
      55,    56,    44,    48,    49,    50,    17,    18,    46,    51,
      47,    59,     0,    63,    42,    15,     0,     0,     0,    26,
      11,    12,     9,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,    22,    28,     0,     0,     0,     0,
      19,     0,    45,    57,     0,    60,    61,    62,    64,    65,
      29,     0,     0,     0,     0,     0,    44,    31,    34,     0,
      32,     0,    27,    23,    21,    20,     0,    52,     0,     0,
      38,    39,    41,     0,    30,     0,    58,     0,    66,    71,
      74,    76,    43,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    33,    36,    67,    68,
      69,    70,    72,    73,    75,    77,    37,     0,    35
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -82,   -82,    74,   -82,   -82,     0,   112,   -82,   -82,   -82,
     117,   -82,   -82,   -82,   -82,   -82,    77,   -41,   -82,   -81,
     -28,    41,   -62,   -82,   111,    -9,   -82,   -82,    55,     7,
       1,    19,    17,   -82
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     1,     6,     7,    19,     8,    20,    50,     9,    15,
      16,    36,    54,    10,    11,    48,    49,    88,    89,    90,
      37,   107,    38,    39,    40,    41,    42,    74,    43,    44,
     109,   110,   111,   112
  };

  const unsigned char
  Parser::yytable_[] =
  {
      55,    91,    66,    13,   103,    64,   102,    63,    12,    67,
      14,    96,    17,    46,    18,    28,    12,    81,    97,    82,
      83,    84,    85,    91,    47,    68,    93,    69,    73,    21,
      70,    29,    30,    57,    24,   127,    31,    86,    25,    71,
      56,    94,    72,    95,   136,    32,    33,    34,    35,    75,
      76,    77,    63,    53,    91,    65,   138,    22,    23,     2,
      58,    59,    60,    91,    26,    27,    47,    98,   106,    29,
      30,     3,     4,     5,    31,    91,    72,   115,    61,    62,
      63,    80,    99,    32,    33,    34,    35,     3,    12,    63,
      81,   100,    82,    83,    84,    85,    33,    34,    35,    81,
     101,    82,    83,    84,    85,   108,   108,   121,   122,   104,
      86,   105,   117,   118,   119,   120,    78,    79,   114,    86,
     116,   123,   132,   133,   128,   129,   130,   131,   108,   108,
     108,   108,   124,   125,   126,   137,    51,    87,    52,    45,
     113,   135,   134,    92
  };

  const unsigned char
  Parser::yycheck_[] =
  {
      28,    63,     3,     3,    85,    46,     4,     5,    13,    10,
      35,     3,    35,    10,    35,     5,    13,    15,    10,    17,
      18,    19,    20,    85,    24,    53,    67,     3,    56,    11,
       6,    21,    22,    42,     9,   116,    26,    35,    11,     3,
       9,    69,     6,    71,   125,    35,    36,    37,    38,    58,
      59,    60,     5,     5,   116,    35,   137,     3,     4,     0,
      23,    24,    25,   125,     3,     4,    66,     9,    96,    21,
      22,    12,    13,    14,    26,   137,     6,   105,    21,    22,
       5,     6,     9,    35,    36,    37,    38,    12,    13,     5,
      15,     4,    17,    18,    19,    20,    36,    37,    38,    15,
       4,    17,    18,    19,    20,    98,    99,    31,    32,     6,
      35,    11,    27,    28,    29,    30,    61,    62,     4,    35,
      10,    33,   121,   122,   117,   118,   119,   120,   121,   122,
     123,   124,    34,    10,     4,    16,    25,    63,    26,    22,
      99,   124,   123,    66
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    40,     0,    12,    13,    14,    41,    42,    44,    47,
      52,    53,    13,    44,    35,    48,    49,    35,    35,    43,
      45,    11,     3,     4,     9,    11,     3,     4,     5,    21,
      22,    26,    35,    36,    37,    38,    50,    59,    61,    62,
      63,    64,    65,    67,    68,    49,    10,    44,    54,    55,
      46,    63,    45,     5,    51,    59,     9,    64,    23,    24,
      25,    21,    22,     5,    56,    35,     3,    10,    59,     3,
       6,     3,     6,    59,    66,    64,    64,    64,    67,    67,
       6,    15,    17,    18,    19,    20,    35,    41,    56,    57,
      58,    61,    55,    56,    59,    59,     3,    10,     9,     9,
       4,     4,     4,    58,     6,    11,    59,    60,    68,    69,
      70,    71,    72,    60,     4,    59,    10,    27,    28,    29,
      30,    31,    32,    33,    34,    10,     4,    58,    68,    68,
      68,    68,    69,    69,    70,    71,    58,    16,    58
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    39,    40,    40,    40,    41,    41,    42,    43,    43,
      44,    45,    46,    47,    48,    48,    49,    49,    50,    50,
      51,    51,    52,    52,    53,    53,    54,    54,    55,    56,
      56,    57,    57,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    59,    60,    61,    62,    62,    62,    63,    63,
      63,    64,    64,    64,    65,    65,    65,    66,    66,    67,
      67,    67,    67,    68,    68,    68,    69,    69,    69,    69,
      69,    70,    70,    70,    71,    71,    72,    72
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     1,     1,     4,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       3,     3,     5,     6,     1,     1,     1,     3,     2,     2,
       3,     1,     1,     4,     1,     7,     5,     5,     2,     2,
       3,     2,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     4,     2,     1,     1,     1,     1,     3,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     1,     3
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
  "\"八进制常数\"", "\"十六进制常数\"", "$accept", "CompUnit",
  "Decl", "ConstDecl", "constdef_comma_list", "BType", "ConstDef",
  "ConstInitVal", "VarDecl", "vardecl_comma_list", "VarDef", "InitVal",
  "exp_comma_list", "FuncDef", "FuncType", "FuncFParams", "FuncFParam",
  "Block", "BlockItem", "Stmt", "Exp", "Cond", "LVal", "PrimaryExp",
  "Number", "UnaryExp", "UnaryOp", "FuncRParams", "MulExp", "AddExp",
  "RelExp", "EqExp", "LAndExp", "LOrExp", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,    98,    98,    99,   100,   103,   103,   106,   109,   110,
     113,   116,   119,   122,   125,   126,   129,   130,   133,   134,
     137,   138,   141,   142,   145,   145,   148,   149,   152,   155,
     156,   159,   159,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   173,   176,   179,   182,   182,   182,   185,   185,
     185,   188,   189,   190,   193,   193,   193,   196,   197,   200,
     201,   202,   203,   206,   207,   208,   211,   212,   213,   214,
     215,   218,   219,   220,   223,   224,   227,   228
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
#line 1200 "parser.cpp"

#line 233 "parser.y"

namespace saltyfish
{
  // Report an error to the user.
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
