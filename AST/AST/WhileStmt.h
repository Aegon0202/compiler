#pragma once
#include <memory>
#include <vector>

#include "Stmt.h"
#include "Exp.h"

namespace saltyfish {
    class ASTVisitor;

    class WhileStmt :
        public Stmt
    {
    public:
        std::unique_ptr<Exp> cond;
        std::unique_ptr<Stmt> body;

    public:
        WhileStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> body);
        WhileStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> body,location loc);
        virtual void accept(ASTVisitor& visitor);
    };
}

