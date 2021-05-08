#pragma once
#include <memory>
#include <vector>

#include "Type.h"
#include "Ident.h"
#include "FuncParamDecl.h"
#include "BlockStmt.h"
#include "ASTUnit.h"

namespace saltyfish {
    class ASTVisitor;

    class FuncDef :
        public ASTUnit
    {
    public:
        std::unique_ptr<Type> type;
        std::unique_ptr<Ident> ident;
        std::vector<std::unique_ptr<FuncParamDecl>> funcParamDeclList;
        std::unique_ptr<BlockStmt> block;

    public:
        FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::unique_ptr<BlockStmt> block);
        FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::vector<std::unique_ptr<FuncParamDecl>> funcParamDeclList, std::unique_ptr<BlockStmt> block);
        FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::unique_ptr<BlockStmt> block,location loc);
        FuncDef(std::unique_ptr<Type> type, std::unique_ptr<Ident> ident, std::vector<std::unique_ptr<FuncParamDecl>> funcParamDeclList, std::unique_ptr<BlockStmt> block,location loc);
        void accept(ASTVisitor& visitor);
    };
}

