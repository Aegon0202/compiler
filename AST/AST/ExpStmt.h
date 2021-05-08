#pragma once
#include <vector>
#include <memory>

#include "Exp.h"
#include "Stmt.h"

namespace saltyfish {
    class ASTVisitor;

    class ExpStmt :
        public Stmt
    {
    public:
        std::unique_ptr<Exp> exp;

    public:
        ExpStmt(std::unique_ptr<Exp> exp);
        ExpStmt(std::unique_ptr<Exp> exp,location loc);
        virtual void accept(ASTVisitor& visitor);
    };
}

