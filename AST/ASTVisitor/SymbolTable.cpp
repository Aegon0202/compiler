#include "SymbolTable.h"

using namespace std;
using namespace saltyfish;

SymbolAttr::SymbolAttr(SymbolAttr::SymbolDeclarer declarer, Type::IdentType specifier, unsigned level)
	:declarer(declarer),specifier(specifier),level(level)
{
	
}