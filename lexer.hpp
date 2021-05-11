#ifndef LEXER_HPP
#define LEXER_HPP

/**
 * Generated Flex class name is yyFlexLexer by default. If we want to use more flex-generated
 * classes we should name them differently. See scanner.l prefix option.
 * 
 * Unfortunately the implementation relies on this trick with redefining class name
 * with a preprocessor macro. See GNU Flex manual, "Generating C++ Scanners" section
 */
#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer sfFlexLexer
#include <FlexLexer.h>
#endif

#include "parser.hpp"
#include "location.hh"

#undef YY_DECL
#define YY_DECL saltyfish::Parser::symbol_type saltyfish::Lexer::get_next_token(saltyfish::Index &index)
// Scanner method signature is defined by this macro. Original yylex() returns int.
// Sinice Bison 3 uses symbol_type, we must change returned type. We also rename it
// to something sane, since you cannot overload return type.

namespace saltyfish
{

    class Lexer : public yyFlexLexer
    {
    public:
        Lexer() {}
        virtual ~Lexer() {}
        virtual saltyfish::Parser::symbol_type get_next_token(saltyfish::Index &index);
        Lexer(std::istream *in) : yyFlexLexer(in) {}

    private:
    };

}

#endif