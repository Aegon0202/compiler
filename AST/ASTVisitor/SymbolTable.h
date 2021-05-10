#pragma once
#include <map>
#include <string>

#include "Type.h"

namespace saltyfish {
	class SymbolAttr
	{
	public:
		typedef unsigned level_t;
		typedef unsigned times_t;
	public:
		enum SymbolDeclarer{
			Func,Var,ArrVar,ConstVar,ConstArrVar
		} declarer;
		Type::IdentType specifier;
		times_t referedTimes=0;
		level_t level;
		saltyfish::SymbolAttr* next;
	public:
		SymbolAttr(SymbolAttr::SymbolDeclarer declarer, Type::IdentType specifier, unsigned level);
	};

	class SymbolTable
	{
	public:
		typedef SymbolAttr::level_t level_t;
		typedef SymbolAttr::times_t times_t;
	public:
		std::map<std::string, SymbolAttr> table;
		int insert(SymbolAttr& symbolAttr);
		void clearTemp(level_t level);
		bool isDeclared(std::string symbolName);

	};
}