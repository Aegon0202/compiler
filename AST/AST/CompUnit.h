#pragma once
#include <vector>
#include <memory>

#include "ASTUnit.h"

namespace saltyfish {
    class CompUnit :
        virtual public ASTUnit
    {
    public:
        std::vector<std::unique_ptr<ASTUnit>> compUnitList;

    public:
        CompUnit();
        CompUnit(std::vector<std::unique_ptr<ASTUnit>> compUnitList);
        CompUnit(std::vector<std::unique_ptr<ASTUnit>> compUnitList,location loc);
        void accept(ASTVisitor& visitor);
    };
}

