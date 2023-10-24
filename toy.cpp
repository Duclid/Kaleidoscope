/**********************************
*        ____  _  _  ____ 
*       (    \/ )( \(  _ \
*        ) D () __ ( ) __/
*       (____/\_)(_/(__)  
*
**********************************/

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "token.h"

using namespace llvm;

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
        std::string num_str;
        do{
            num_str.push_back(static_cast<char>(LastChar));
            LastChar = getchar();
        }while(isdigit(LastChar) || LastChar == '.');
        
        NumVal = strtod(num_str.c_str(), nullptr);
        return tok_number;
    }
    
    // #4. 处理注释
    if(LastChar == '#'){
        do{
            LastChar = getchar();
        }while(LastChar != '\n' && LastChar != EOF && LastChar != '\r');
    }

    // #5. 其余字符
    if(LastChar == EOF){
        return tok_eof;
    }

    // #6. 返回当前字符
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

//===----------------------------------------------------------------------===//
// AST节点
//===----------------------------------------------------------------------===//

/// 表达式基类
class ExprAST{
public:
    virtual ~ExprAST() = default;
    virtual Value* codegen() = 0;
};

class NumberExprAST : public ExprAST{
public:
    NumberExprAST(double val)
        : Val(val) {}

    Value* codegen () override;
private:
    double Val;
};

class VariableExprAST : public ExprAST{
public:
    VariableExprAST(std::string name)
        : Name(name) {}

    virtual Value* codegen() override;
private:
    std::string Name;
};

class BinaryExprAST : public ExprAST{
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
        : Op(op), Lhs(std::move(lhs)), Rhs(std::move(rhs)) {}

    virtual Value* codegen() override;
private:
    char Op;
    std::unique_ptr<ExprAST> Lhs, Rhs;
};

class CallExprAST : public ExprAST{
public:
    CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args)
        : Callee(callee), Args(std::move(args)) {}
    virtual Value* codegen() override;
private:
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
};

/// 函数原型节点
/// 原型用于表示一个函数的原型，用于捕获函数的名称、各个函数的参数等
class PrototypeAST{
public:
    PrototypeAST(std::string name, std::vector<std::string> args)
        : Name(name), Args(std::move(args)){}
    Function* codegen();
    const std::string& getName() {return Name;}
private:
    std::string Name;
    std::vector<std::string> Args;
};

/// 函数节点
class FunctionAST{
public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
        : Proto(std::move(proto)), Body(std::move(body)) {}
    Function* codegen();
private:
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body; 
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

static std::unique_ptr<ExprAST> ParseExpression();

/// 解析数字表达式
/// ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr(){
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

/// 解析括号表达式
/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenEpxr(){
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
static std::unique_ptr<ExprAST> ParseIdentifierExpr(){
    std::string IdName = Identifier;
    getNextToken(); // eat Identifier;

    if(CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);

    // Call Expr
    getNextToken(); // eat (
    // 获取参数列表
    std::vector<std::unique_ptr<ExprAST>> Args;
    while(CurTok != ')'){
        if(auto Arg = ParseExpression()){
            Args.push_back(std::move(Arg));
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
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// 解析主表达式
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary(){
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
    return nullptr;
}

/// 二元表达式优先级
static std::map<char, int> BinopPrecedence;

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
static std::unique_ptr<ExprAST> ParseBinOpRhs(int ExprPrec, std::unique_ptr<ExprAST> Lhs){
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
            Rhs = ParseBinOpRhs(TokPrec+1, std::move(Rhs));
            if(!Rhs)
                return nullptr;
        }

        Lhs = std::make_unique<BinaryExprAST>(Binop, std::move(Lhs), std::move(Rhs));
    }
}

/// 解析op表达式
static std::unique_ptr<ExprAST> ParseExpression(){
    auto Lhs = ParsePrimary();
    if(!Lhs) return nullptr;

    return ParseBinOpRhs(0, std::move(Lhs));
}

/// 解析函数原型
///   ::id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype(){
    if(CurTok != tok_identifier){
        return LogErrorP("expected function name in prototype");
    }

    std::string FnName = Identifier;
    getNextToken();

    if(CurTok != '('){
        return LogErrorP("expected '(' in prototype");
    }

    std::vector<std::string> ArgNames;
    while(getNextToken() == tok_identifier){
        ArgNames.push_back(Identifier);
    }

    if(CurTok != ')'){
        return LogErrorP("expected ')' in prototype");
    }

    getNextToken(); // eat ')'
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// 解析函数定义
static std::unique_ptr<FunctionAST> ParseDefinition(){
    getNextToken(); // eat def
    auto Proto = ParsePrototype();
    if(!Proto) return nullptr;

    if(auto E = ParseExpression()){
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }

    return nullptr;
}

/// 解析函数声明
/// eg. extern func(arg)
static std::unique_ptr<PrototypeAST> ParseExtern(){
    getNextToken(); // eat extern
    return ParsePrototype();
}

/// 解析顶层表达式
static std::unique_ptr<FunctionAST> ParseTopLevelExpr(){
    if(auto E = ParseExpression()){
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

static void HandleDefinition(){
    if(ParseDefinition()){
        std::cout << "Parsed a function definition." << std::endl;
    }else{
        getNextToken();
    }
}

static void HandleExtern(){
    if(ParseExtern()){
        std::cout << "Parsed an extern." << std::endl;
    }else{
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    if (ParseTopLevelExpr()) {
        std::cout << "Parsed a top-level expr" << std::endl;
    } else {
        getNextToken();
    }
}

/// 驱动器
/// top ::= definition | external | expression | ';'
/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
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
// 生成IR
//===----------------------------------------------------------------------===//

Value* LogErrorV(const char* str){
    LogError(str);
    return nullptr;
}

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<IRBuilder<>> Builder;
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;

Value* NumberExprAST::codegen(){
    return ConstantFP::get(*TheContext, APFloat(Val));
}

Value* VariableExprAST::codegen(){
    Value* V = NamedValues[Name];
    if(!V){
        LogErrorV("Unknow variable name");
    }
    return V;
}

Value* BinaryExprAST::codegen(){
    Value* L = Lhs->codegen();
    Value* R = Rhs->codegen();
    if(!L || !R){
        return nullptr;
    }

    switch(Op){
        case '+':
            return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder->CreateFSub(L, R, "subtmp");
        case '*':
            return Builder->CreateFMul(L, R, "multmp");
        case '<':
            L = Builder->CreateFCmpULT(L, R, "cmptmp");
            return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext), "booltmp");
        default:
            return LogErrorV("invalid binary operator");
    }
}

Value* CallExprAST::codegen(){
    Function *CalleeF = TheModule->getFunction(Callee);
    if(!CalleeF){
        return LogErrorV("Unknow function refrenced");
    }

    if(CalleeF->arg_size() != Args.size()){
        return LogErrorV("Incorrenct arguments passed");
    }

    std::vector<Value*> ArgsV;
    for(unsigned int i = 0, e = Args.size(); i != e;++i){
        ArgsV.push_back(Args[i]->codegen());
        if(!ArgsV.back()){  // 参数生成失败
            return nullptr;
        }
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

Function* PrototypeAST::codegen(){
    std::vector<Type*> Doubles(Args.size(), Type::getDoubleTy(*TheContext));
    FunctionType *FT = FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);
    // 创建一个函数：原型 链接模式 名称 写入的目标模块
    Function *F = Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

    unsigned int Idx = 0;
    for(auto& Arg : F->args()){
        Arg.setName(Args[Idx++]);
    }
    return F;
}

Function* FunctionAST::codegen(){
    Function *TheFunction = TheModule->getFunction(Proto->getName());

    if(!TheFunction){
        TheFunction = Proto->codegen();
    }

    if(!TheFunction){
        return nullptr;
    }

    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    NamedValues.clear();
    for(auto& Arg : TheFunction->args()){
        NamedValues[std::string(Arg.getName())] = &Arg;
    }

    if(Value* RetVal = Body->codegen()){
        Builder->CreateRet(RetVal);
        verifyFunction(*TheFunction);
        return TheFunction;
    }
    
    TheFunction->eraseFromParent();
    return nullptr;
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
    // cout << gettok() << endl;
    // Prime the first token.
    
    MainLoop();
    
    return 0;
}
