#pragma once
#include <memory>
#include <vector>

#include "Stmt.h"
#include "Exp.h"

namespace saltyfish {
    class ASTVisitor;

    class IfStmt :
        public Stmt
    {
    public:
        class IfStmtBitFields {
        public:
            unsigned hasElse : 1;
        } bitFields;
        std::unique_ptr<Exp> cond;
        std::unique_ptr<Stmt> ifBody, elseBody;

    public:
        IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody);
        IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody,std::unique_ptr<Stmt> elseBody);
        IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody,location loc);
        IfStmt(std::unique_ptr<Exp> cond, std::unique_ptr<Stmt> ifBody, std::unique_ptr<Stmt> elseBody,location loc);
        bool hasElse();
        void setHasElse(bool b);
        virtual void accept(ASTVisitor& visitor);
    };
}

