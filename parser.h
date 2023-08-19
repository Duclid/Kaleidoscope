#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "lex.h"

// 抽象语法树的基类
class ExprAST {
public:
  virtual ~ExprAST() = default;
};

// 数字表达式
class NumberExprAST : public ExprAST {
 public:
  NumberExprAST(double val) : val_(val) {}

 private:
  double val_;
};

// 变量表达式
class VariableExprAST : public ExprAST {
 public:
  VariableExprAST(const std::string& name) : name_(name) {}

 private:
  std::string name_;
};

// 二元操作表达式
class BinaryExprAST : public ExprAST {
 public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs,
                std::unique_ptr<ExprAST> rhs)
      : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

 private:
  char op_;
  std::unique_ptr<ExprAST> lhs_;
  std::unique_ptr<ExprAST> rhs_;
};

// 函数调用表达式
class CallExprAST : public ExprAST {
 public:
  CallExprAST(const std::string& callee,
              std::vector<std::unique_ptr<ExprAST> > args)
      : callee_(callee), args_(std::move(args)) {}

 private:
  std::string callee_;
  std::vector<std::unique_ptr<ExprAST> > args_;
};

// 函数声明
class PrototypeAST {
 public:
  PrototypeAST(const std::string& name, std::vector<std::string> args)
      : name_(name), args_(std::move(args)) {}

  const std::string& name() const { return name_; }

 private:
  std::string name_;
  std::vector<std::string> args_;
};

// 函数定义
class FunctionAST {
 public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body)
      : proto_(std::move(proto)), body_(std::move(body)) {}

 private:
  std::unique_ptr<PrototypeAST> proto_;
  std::unique_ptr<ExprAST> body_;
};

/**
 * @brief 语法解析器
 * 
 */
class Parser{

public:

  Parser() = default;

  // 优先级
  std::map<char, int> BinopPrecedence;

  // 获取优先级
  int GetBinopPrecedence();

  // 主解析器
  void MainLoop();

  // 读取下一个 token
  int GetNextToken(){ return current_token_ = lexer.GetToken(); }
  
  ~Parser() = default;
private:

  int current_token_; // 当前待处理的 token
  Lex lexer;          // 词法解析器

  // 处理数字 : number
  std::unique_ptr<NumberExprAST> ParseNumberExpr();

  // 处理括号表达式 : (expression)
  std::unique_ptr<ExprAST> ParseParenExpr();

  // 处理函数调用 : identifier (expression)
  std::unique_ptr<ExprAST> ParseIdentifierExpr();

  // 处理表达式
  std::unique_ptr<ExprAST> ParseExpression();

  // 处理 extren
  std::unique_ptr<PrototypeAST> ParseExtern();

  // 处理顶层匿名函数
  std::unique_ptr<FunctionAST> ParseTopLevelExpr();

  // 解析任意的主表达式，根据 token 类型选择不同的解析器
  std::unique_ptr<ExprAST> ParseParimary();

  /**
   * @brief 处理 op 的 rhs 表达式
   * @param expr_prec 符号优先级
   *        lhs op 的 lhs 表达式
   */
  std::unique_ptr<ExprAST> ParseBinOpRhs(int expr_prec,
                                              std::unique_ptr<ExprAST> lhs);

  // 处理函数声明
  std::unique_ptr<PrototypeAST> ParsePrototype();

  // 处理函数定义
  std::unique_ptr<FunctionAST> ParseDefinition();

  // 错误处理
  std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
  }
  // 错误处理
  std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
  }

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

void HandleDefinition();

void HandleExtern();

void HandleTopLevelExpression();

};


