#ifndef INDEX_HPP
#define INDEX_HPP

#include <iostream>
#include <string>

#include "lexer.hpp"
#include "parser.hpp"
using namespace std;

namespace saltyfish
{
    class Index
    {
    private:
        saltyfish::location loc;

    private:
        Parser *parser = nullptr;
        Lexer *lexer = nullptr;

    public:
        Index();
        saltyfish::Parser::symbol_type get_next_token();
        // int parse();
        // int parse(const string &filename);
        saltyfish::location &get_location();
    };
}

#endif