#include <cctype>
#include <fstream>
#include <cassert>

#include "driver.hpp"
#include "scanner.hpp"

saltyfish::Driver::Driver(){};

saltyfish::Parser::symbol_type saltyfish::Driver::get_next_token(){
   if(scanner==nullptr) scanner=new Scanner(&std::cin);
   saltyfish::Parser::symbol_type type=scanner->get_next_token(*this);
   return type;
}

int saltyfish::Driver::parse()
{
   return parseStream(std::cin);
}

int saltyfish::Driver::parse(const std::string &filename){
   if(filename.empty()){
      return -1;
   }
   loc.initialize((string*)&filename);
   std::ifstream stream(filename);
   if(!stream.good()){
      return -1;
   }
   return parseStream(stream);
}

int saltyfish::Driver::parse(std::istream &stream){
   if(!stream.good() || stream.eof()){
      return -1;
   }
   return parseStream(stream);
}

int saltyfish::Driver::parseStream(std::istream &stream){
   delete scanner;
   delete parser;
   scanner=new Scanner(&stream);
   parser=new Parser(*scanner,*this);
   return parser->parse();
}

saltyfish::location& saltyfish::Driver::getLoc(){
   return loc;
}