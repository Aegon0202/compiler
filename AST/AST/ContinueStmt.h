#pragma once
#include "Stmt.h"

namespace saltyfish {
    class ASTVisitor;

    class ContinueStmt :
        public Stmt
    {
    public:
        ContinueStmt();
        ContinueStmt(location loc);
        virtual void accept(ASTVisitor& visitor);
    };
}

