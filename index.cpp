#include "index.hpp"

saltyfish::Index::Index(){};

saltyfish::location &saltyfish::Index::get_location()
{
    return saltyfish::Index::loc;
}

saltyfish::Parser::symbol_type saltyfish::Index::get_next_token()
{
    if (lexer == nullptr)
        lexer = new Lexer(&std::cin);
    saltyfish::Parser::symbol_type type = lexer->get_next_token(*this);
    return type;
}
