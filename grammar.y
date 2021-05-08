

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
CompUnit:
%empty | CompUnit Decl | CompUnit FuncDef

Decl:
ConstDecl | VarDecl


ConstDecl:
T_CONST BType constdef_comma_list T_SEMICOLON {cout<<"常量声明"<<endl;}

constdef_comma_list:
ConstDef
| constdef_comma_list T_COMMA ConstDef

BType:
T_INT

ConstDef:
T_IDENT constdef_dimension_list T_EQUAL ConstInitVal {cout<<"常数定义"<<endl;}

constdef_dimension_list:
%empty | constdef_dimension_list T_LM ConstExp T_RM

ConstInitVal:
Number  {cout<<"数字"<<endl;}

VarDecl:
BType VarDef vardef_comma_list T_SEMICOLON {cout<<"变量声明"<<endl;}

vardef_comma_list:
VarDef| vardef_comma_list T_COMMA VarDef

VarDef:
T_IDENT vardef_dimension_list T_EQUAL InitVal {cout<<"变量定义"<<endl;}
| T_IDENT vardef_dimension_list {cout<<"变量定义"<<endl;}

InitVal:
Number

