#pragma once
#include "ASTUnit.h"

namespace saltyfish {
    class ASTVisitor;

    class Stmt :
        virtual public ASTUnit
    {
    public:
        virtual void accept(ASTVisitor& visitor) {

        }
    };
}

