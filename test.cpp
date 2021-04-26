  #include "parser.hpp"
  
  namespace saltyfish
  {
    // Return the next token.
    auto yylex () -> Parser::symbol_type
    {
      static int count = 0;
      switch (int stage = count++)
        {
        case 0:
          return Parser::make_TEXT ("I have three numbers for you.");
        case 1: case 2: case 3:
          return Parser::make_NUMBER (stage-1);
        case 4:
          return Parser::make_TEXT ("And that's all!");
        default:
          return NULL;
        }
    }
  }