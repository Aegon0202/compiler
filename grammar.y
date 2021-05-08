

/* declare token */
%token T_COMMA "逗号"
%token T_SEMICOLON "分号"
%token T_LB "左大括号"
%token T_RB "右大括号"
%token T_LM "左中括号"
%token T_RM "右中括号"
%token T_LS "左小括号"
%token T_RS "右小括号"

%token T_EQUAL  "等号"

%token T_CONST "const关键字";
%token T_INT   "int关键字"
%token T_VOID "void关键字"

%token T_IF "if关键字"
%token T_ELSE "else关键字"
%token T_WHILE "while关键字"
%token T_BREAK "break关键字"
%token T_CONTINUE "continue关键字"
%token T_RETURN "return关键字"

%token T_ADD "加号"
%token T_SUB "减号"
%token T_MUL "乘号"
%token T_DIV "除号"
%token T_MOD "余号"
%token T_NOT "感叹号"
%token T_B "小于号"
%token T_A "大于号"
%token T_BE "小于等于号"
%token T_AE "大于等于号"
%token T_E "等于等于号"
%token T_NE "不等于号"
%token T_AND "与号"
%token T_OR "或号"

%token <const char *> T_IDENT "标识符"
%token <int> T_DECIMAL "十进制常数"
%token <int> T_OCTAL "八进制常数"
%token <int> T_HEXADECIMAL "十六进制常数"

%token END 0 "文件结尾"

%start CompUnit

%%
CompUnit
    :   Decl 
    |   FuncDef
    |   CompUnit FuncDef 
    |   CompUnit Decl 
    ;

Decl
    :   ConstDecl 
    |   VarDecl
    ;


ConstDecl
    :    T_CONST BType ConstDefs T_SEMICOLON 
    ;

ConstDefs
    :   ConstDef
    |   ConstDefs T_COMMA ConstDef
    ;

BType
    :   T_INT
    ;

ConstDef
    :   T_IDENT ArrayDef T_EQUAL ConstInitVal 
    ;

ArrayDef
    :   %empty 
    |   ArrayDef T_LM ConstExp T_RM
    ;

ConstInitVal
    :   number
    ;

VarDecl
    :   BType VarDefs T_SEMICOLON
    ;

VarDefs
    :   VarDef
    |   VarDefs T_COMMA VarDef
    ;

VarDef
    :   T_IDENT ArrayDef T_EQUAL InitVal 
    |   T_IDENT ArrayDef
    ;


InitVal
    :   Exp
    |   T_LB InitVals T_RB
    ;

InitVals
    :   %empty
    |   InitVal
    |   InitVals T_COMMA InitVal
    ;

FuncType
    :   T_INT
    |   T_VOID
    ;

FuncDef
    :   FuncType T_IDENT T_LS FuncfParams T_RS Block
    ;

FuncfParams
    :   %empty
    |   FuncfParam
    |   FuncfParams T_COMMA FuncfParam
    ;

FuncfParam
    :   BType T_IDENT
    |   BType T_IDENT T_LS T_RS ExpArrayDefs
    ;

ExpArrayDefs
    :   %empty
    |   ExpArrayDef
    |   ExpArrayDefs ExpArrayDef
    ;

ExpArrayDef
    :   T_LM Exp T_RM
    ;

Block
    :   T_LB BlockItems T_RB
    ;

BlockItems
    :   %empty
    |   BlockItem
    |   BlockItems BlockItem
    ;

BlockItem
    :   Decl
    |   Stmt
    ;

Stmt
    :   LVal T_EQUAL Exp T_SEMICOLON
    |   Exp T_SEMICOLON
    |   T_SEMICOLON
    |   Block
    |   T_IF T_LS Conditon T_RS Stmt
    |   T_IF T_LS Conditon T_RS Stmt T_ELSE Stmt
    |   T_WHILE T_LS Conditon T_RS Stmt
    |   T_BREAK T_SEMICOLON
    |   T_CONTINUE T_SEMICOLON
    |   T_RETURN T_SEMICOLON
    |   T_RETURN Exp T_SEMICOLON
    ;

Exp
    :   AddExp
    ;

Conditon
    : LOrExp
    ;

LVal
    :   T_IDENT ExpArrayDefs
    ;

PrimaryExp
    :   T_LS Exp T_RS
    |   LVal
    |   Number
    ;

Number 
    :   IntConst
    ;






