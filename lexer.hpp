#ifndef LEXER_HPP
#define LEXER_HPP

// If you want to create multiple (different) lexer classes,
// you use the `-P' flag (or the `prefix=' option) to rename each yyFlexLexer to some other xxFlexLexer.
// You then can include `<FlexLexer.h>' in your other sources once per lexer class, first renaming yyFlexLexer as follows:
#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer sfFlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL saltyfish::Parser::symbol_type saltyfish::Lexer::get_next_token()

#include "parser.hpp"
namespace saltyfish
{
    class Lexer : public yyFlexLexer
    {
    public:
        Lexer() {}
        virtual ~Lexer() {}
        virtual saltyfish::Parser::symbol_type get_next_token();
    };

}
#endif