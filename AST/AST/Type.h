#pragma once
#include <map>
#include <string>

#include "ASTUnit.h"

namespace saltyfish{
	class ASTVisitor;

	class Type 
		:virtual public ASTUnit
	{
	public:
		enum IdentType {
			Int, String, Void
		} type;
		std::string typeStr;
		std::map<std::string, IdentType> strTypeMap = {
			{"int",Int},
			{"void",Void},
			{"string",String}
		};
	public:
		Type();
		Type(std::string &typeStr);
		Type(std::string& typeStr,location loc);
		virtual void accept(ASTVisitor& visitor);
		friend std::ostream& operator<<(std::ostream& o, const Type& type);
	};

}