/**********************************
*        ____  _  _  ____ 
*       (    \/ )( \(  _ \
*        ) D () __ ( ) __/
*       (____/\_)(_/(__)  
*
**********************************/

#include <iostream>
#include <cctype>
#include <cstdio>
#include <memory>
#include <vector>
#include <ctype.h>
#include <map>
#include <cmath>

#include "token.h"

using namespace std;


//===----------------------------------------------------------------------===//
// 词法分析器
//===----------------------------------------------------------------------===//

static int gettok(){
    
    static int LastChar = ' ';

    // #1. 跳过空格
    while(isspace(LastChar))
        LastChar = getchar();

    // #2. 处理 identifier
    if(isalpha(LastChar)){
        Identifier = static_cast<char>(LastChar);
        while(isalnum(LastChar = getchar())){
            Identifier.push_back(static_cast<char>(LastChar));
        }

        // TODO: 如果仅前几位是关键字，待完善
        if(Identifier == "def"){
            return tok_def;
        }

        if(Identifier == "extern"){
            return tok_extern;
        }
        
        return tok_identifier;
    }

    // #3. 处理 Number
    // TODO: 默认数字不违规
    if(isdigit(LastChar) || LastChar == '.'){
        string num_str;
        do{
            num_str.push_back(static_cast<char>(LastChar));
            LastChar = getchar();
        }while(isdigit(LastChar) || LastChar == '.');
        
        NumVal = stod(num_str.c_str(), nullptr);
        return tok_number;
    }
    
    // #4. 处理注释
    if(LastChar == '#'){
        do{
            LastChar = getchar();
        }while(LastChar != '\n' && LastChar != EOF && LastChar != '\r');
    }

    // #5. 其余字符
    if(LastChar != EOF){
        return gettok();
    }

    return 0;
}

//===----------------------------------------------------------------------===//
// AST节点
//===----------------------------------------------------------------------===//

/// 表达式基类
class ExprAST{
public:
    virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST{
public:
    NumberExprAST(double val)
        : Val(val) {}
private:
    double Val;
};

class VariableExprAST : public ExprAST{
public:
    VariableExprAST(string name)
        : Name(name) {}
private:
    string Name;
};

class BinaryExprAST : public ExprAST{
public:
    BinaryExprAST(char op, unique_ptr<ExprAST> lhs, unique_ptr<ExprAST> rhs)
        : Op(op), Lhs(move(lhs)), Rhs(move(rhs)) {}
private:
    char Op;
    unique_ptr<ExprAST> Lhs, Rhs;
};

class CallExprAST : public ExprAST{
public:
    CallExprAST(string callee, vector<unique_ptr<ExprAST>> args)
        : Callee(callee), Args(move(args)) {}
private:
    string Callee;
    vector<unique_ptr<ExprAST>> Args;
};

/// 函数原型节点
/// 原型用于表示一个函数的原型，用于捕获函数的名称、各个函数的参数等
class PrototypeAST{
public:
    PrototypeAST(string name, vector<string> args)
        : Name(name), Args(move(args)){}
private:
    string Name;
    vector<string> Args;
};

/// 函数节点
class FunctionAST{
public:
    FunctionAST(unique_ptr<PrototypeAST> proto, unique_ptr<ExprAST> body)
        : Proto(move(proto)), Body(move(body)) {}
private:
    unique_ptr<PrototypeAST> Proto;
    unique_ptr<ExprAST> Body; 
};


//===----------------------------------------------------------------------===//
// 语法解析器
//===----------------------------------------------------------------------===//

/// 日志
std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "LogError: %s\n", Str);
  return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

static int CurTok;  // 当前token

/// 获取下一个token
static int getNextToken(){
    return CurTok = gettok();
}

static unique_ptr<ExprAST> ParseExpression();

/// 解析数字表达式
/// ::= number
static unique_ptr<ExprAST> ParseNumberExpr(){
    auto Result = make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return move(Result);
}

/// 解析括号表达式
/// parenexpr ::= '(' expression ')'
static unique_ptr<ExprAST> ParseParenEpxr(){
    getNextToken(); // eat (
    auto V = ParseExpression();
    if(!V)
        return nullptr;
    
    if(CurTok != ')')
        return LogError("expected ')'");
    
    getNextToken(); // eat )
    return V;
}

/// 解析标识符
///   ::= identifier
///   ::= identifier '(' expression* ')'
static unique_ptr<ExprAST> ParseIdentifierExpr(){
    string IdName = Identifier;
    getNextToken(); // eat Identifier;

    if(CurTok != '(')
        return make_unique<VariableExprAST>(IdName);

    // Call Expr
    getNextToken(); // eat (
    // 获取参数列表
    vector<unique_ptr<ExprAST>> Args;
    while(CurTok != ')'){
        if(auto Arg = ParseExpression()){
            Args.push_back(move(Arg));
        }else{
            return nullptr;
        }

        if(CurTok != ','){
            return LogError("expected ')' or ',' in argument list");
        }
        getNextToken();
    }

    getNextToken(); // eat )

    // 构造函数调用AST Node
    return make_unique<CallExprAST>(IdName, move(Args));
}

/// 解析主表达式
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static unique_ptr<ExprAST> ParsePrimary(){
    switch(CurTok){
        defalut:
            return LogError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenEpxr();
    }
}

/// 二元表达式优先级
static map<char, int> BinopPrecedence;

/// 获取优先级
static int GetTokPrecedence(){
    if(!isascii(CurTok)){
        return -1;
    }

    int TokPrec = BinopPrecedence[CurTok];
    if(TokPrec <= 0) return -1;
    return TokPrec;
}

/**
*   算符优先分析法
*   例子： 对于表达式 a+b+(c+d)*e*f+g，算符优先分析法会将其视为一系列由二元运算符分隔的主表达式
        因此，解析器会首先分析头部的主表达式a，然后一次分析[+, b]，[+, (c+d)]，[*， e]，[*, f]，[+, g]。
*/

/// 解析二元运算
static unique_ptr<ExprAST> ParseBinOpRhs(int ExprPrec, unique_ptr<ExprAST> Lhs){
    while(true){
        int TokPrec = GetTokPrecedence();

        // 优先级较低，先和上一层合并
        if(TokPrec < ExprPrec)
            return Lhs;
        
        int Binop = CurTok;
        getNextToken(); // eat binop

        auto Rhs = ParsePrimary();
        if(!Rhs)
            return nullptr;
        
        // 预测分析，当下一个 binop 具备更高的优先级
        int NextPrec = GetTokPrecedence();
        if(TokPrec < NextPrec){
            Rhs = ParseBinOpRhs(TokPrec+1, move(Rhs));
            if(!Rhs)
                return nullptr;
        }

        Lhs = make_unique<BinaryExprAST>(Binop, move(Lhs), move(Rhs));
    }
}

/// 解析表达式
static unique_ptr<ExprAST> ParseExpression(){
    auto Lhs = ParsePrimary();
    if(!Lhs) return nullptr;

    return ParseBinOpRhs(0, move(Lhs));
}

/// 解析函数原型
///   ::id '(' id* ')'
static unique_ptr<PrototypeAST> ParsePrototype(){
    if(CurTok != tok_identifier){
        return LogErrorP("expected function name in prototype");
    }

    string FnName = Identifier;
    getNextToken();

    if(CurTok != '('){
        return LogErrorP("expected '(' in prototype");
    }

    vector<string> ArgNames;
    while(getNextToken() == tok_identifier){
        ArgNames.push_back(Identifier);
    }

    if(CurTok != ')'){
        return LogErrorP("expected ')' in prototype");
    }

    getNextToken(); // eat ')'
    return make_unique<PrototypeAST>(FnName, move(ArgNames));
}

/// 解析函数定义
static unique_ptr<FunctionAST> ParseDefinition(){
    getNextToken(); // eat def
    auto Proto = ParsePrototype();
    if(!Proto) return nullptr;

    if(auto E = ParseExpression()){
        return make_unique<FunctionAST>(move(Proto), move(E));
    }

    return nullptr;
}

/// 解析函数声明
/// eg. extern func(arg)
static unique_ptr<PrototypeAST> ParseExtern(){
    getNextToken(); // eat extern
    return ParsePrototype();
}

/// 解析顶层表达式
static unique_ptr<FunctionAST> ParseTopLevelExpr(){
    if(auto E = ParseExpression()){
        auto Proto = make_unique<PrototypeAST>("__anon_expr", vector<string>());
        return make_unique<FunctionAST>(move(Proto), move(E));
    }
    return nullptr;
}

static void HandleDefinition(){
    if(ParseDefinition()){
        cout << "Parsed a function definition." << endl;
    }else{
        getNextToken();
    }
}

static void HandleExtern(){
    if(ParseExtern()){
        cout << "Parsed an extern." << endl;
    }else{
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    if (ParseTopLevelExpr()) {
        cout << "Parsed a top-level expr" << endl;
    } else {
        getNextToken();
    }
}

/// 驱动器
/// top ::= definition | external | expression | ';'
/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    cout << "ready> ";
    switch (CurTok) {
    case tok_eof:
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}


//===----------------------------------------------------------------------===//
// Main code.
//===----------------------------------------------------------------------===//

/// Logo
void PrintLogo(){
    std::cout << " ____  _  _  ____ " << std::endl;
    std::cout << "(    \\/ )( \\(  _ \\" << std::endl;
    std::cout << " ) D () __ ( ) __/" << std::endl;
    std::cout << "(____/\\_)(_/(__)" << std::endl;
    std::cout << std::endl;
}

/// 初始化符号优先级
void Init(){
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40; // 高优先级
}


int main(){
    PrintLogo();
    // test token
    //cout << gettok() << endl;
    
    MainLoop();
    
    return 0;
}
