#include <memory>
#include <vector>

#include "parser.h"
#include "lex.h"

std::unique_ptr<NumberExprAST> Parser::ParseNumberExpr(){
    //std::unique_ptr<NumberExprAST> result(new NumberExprAST(lexer.num_val));
    double num = lexer.num_val;
    auto result = std::make_unique<NumberExprAST>(std::move(num));
    GetNextToken(); // 吃掉 number
    return std::move(result);
}

std::unique_ptr<ExprAST> Parser::ParseParenExpr(){
    lexer.GetToken(); // 吃掉 (
    auto result = ParseExpression();
    if(!result){
        return nullptr;
    }
    if(current_token_ != ')'){
        return LogError("expected ')' ");
    }
    lexer.GetToken(); // 吃掉 ）
    return result;
}

std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr(){
    // 捕获函数名称
    std::string call_name = lexer.identifier_str;
    GetNextToken(); // consume (
    
    /**
     * @todo 如果不是函数调用，是变量引用？
     * 
     */
    if(current_token_ != '('){
        //std::unique_ptr<VariableExprAST> result(new VariableExprAST(callee));
        return std::make_unique<VariableExprAST>(call_name);
    }
    
    std::vector<std::unique_ptr<ExprAST> > args;
    GetNextToken(); // consume )

    if(current_token_ != ')'){
        while(true){
            // 函数参数
            if(auto arg = ParseExpression()){
                args.push_back(std::move(arg));
            }else{
                return nullptr;
            }

            if(current_token_ == ')'){
                break;
            }

            if(current_token_ != ','){
                LogError("Expected ')' or ',' in arguments list");
            }

            GetNextToken(); // 吃掉 ','
        }
    }
    GetNextToken(); // 吃掉 ')'
    return std::make_unique<CallExprAST>(call_name, std::move(args));
}

std::unique_ptr<ExprAST> Parser::ParseExpression(){
    auto lhs = ParseParimary();
    if(lhs == nullptr){
        return nullptr;
    }
    return ParseBinOpRhs(0, std::move(lhs));
}

 /// 假设有 a + b * c
 /// 1. ParseBinOpRhs(0, a)
 /// 2. cur_token = '+' 
 ///    expr_prec = 0
 ///    token_prec = 30
 /// 3. 解析 b
 ///    rhs = b
 ///    cur_token = '*'
 /// 4. 递归 ParseBinOpRhs(token_prec+1, b) 解析 b * c 的优先级
 ///    意思就是 a+b 的优先级
/**
 * @brief 解析 binop 的右表达式
 *        复杂表达式可以抽象为一个 PrimaryExpr 跟着多个[binop, PrimaryExpr]二元组
 *        a + b + (c + d) * e * f + g
 *        [+, b], [+, (c+d)], [*, e], [*, f], [+, g]
 * @param expr_prec 
 * @param lhs 
 * @return std::unique_ptr<ExprAST> 
 */
std::unique_ptr<ExprAST> Parser::ParseBinOpRhs(int expr_prec,std::unique_ptr<ExprAST> lhs){
    while(true){
        int token_prec = GetBinopPrecedence();

        // 如果当前token不是二元操作符，current_precedence为-1, 结束任务
        // 如果遇到优先级更低的操作符，也结束任务
        if(token_prec < expr_prec){
            return lhs;
        }
        // 获取运算符
        int bin_op = current_token_;
        // 获取运算符的右表达式
        GetNextToken();

        // 解析右表达式
        auto rhs = ParseParimary();
        if(!rhs){
            return nullptr;
        }
        
        // 获取右表达式的运算符优先级
        // 两种可能：
        //  1. (lhs binop rhs) binop unparsed
        //  2. lhs binop (rhs binop unparsed)
        int next_prec = GetBinopPrecedence();
        if(token_prec < next_prec){
            /**
             * @todo 为什么+1
             * 
             */
            // 将高于current_prec的右边的操作符处理掉返回
            rhs = ParseBinOpRhs(token_prec+1, std::move(rhs));
            if(!rhs){
                return nullptr;
            }
        }
        // 合并左右子树，
        lhs = std::make_unique<BinaryExprAST>(bin_op, std::move(lhs), std::move(rhs));
    }
}

/**
 * @brief 处理函数声明
 * 
 * @return std::unique_ptr<PrototypeAST> 
 */
std::unique_ptr<PrototypeAST> Parser::ParsePrototype(){
  if (current_token_ != TOKEN_IDENTIFIER)
    return LogErrorP("Expected function name in prototype");

  std::string fn_name = lexer.identifier_str;
  GetNextToken();

  if (current_token_ != '(')
    return LogErrorP("Expected '(' in prototype");

  // Read the list of argument names.
  std::vector<std::string> arg_names;
  while (GetNextToken() == TOKEN_IDENTIFIER)
    arg_names.push_back(lexer.identifier_str);
  if (current_token_ != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  GetNextToken();  // eat ')'.

  return std::make_unique<PrototypeAST>(fn_name, std::move(arg_names));    
}


/**
 * @brief 解析函数定义
 * 
 * @return std::unique_ptr<FunctionAST> 
 */
std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
  GetNextToken();  // eat def.
  auto Proto = ParsePrototype();
  if (!Proto) return nullptr;

  if (auto body = ParseExpression())
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(body));
  return nullptr;
}

/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
  GetNextToken();  // eat extern.
  return ParsePrototype();
}

/// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
  if (auto expr = ParseExpression()) {
    // Make an anonymous proto.
    auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
  }
  return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseParimary(){
    switch(current_token_){
        default:
            LogError("unknown token when expecting an expression");
        case TOKEN_IDENTIFIER:
            return ParseIdentifierExpr();
        case TOKEN_NUMBER:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
    return nullptr;
}

int Parser::GetBinopPrecedence(){
    if(!isascii(current_token_)){
        return -1;
    }

    int token_prec = Parser::BinopPrecedence[current_token_];
    if(token_prec <= 0){
        return -1;
    }

    return token_prec;
}


void Parser::MainLoop(){
    while(true){
        fprintf(stderr, "ready> ");
        switch (current_token_) {
            case TOKEN_EOF:
                return;
            case ';': // ignore top-level semicolons.
                GetNextToken();
                break;
            case TOKEN_DEF:
                HandleDefinition();
                break;
            case TOKEN_EXTERN:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}

void Parser::HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery.
    GetNextToken();
  }
}

void Parser::HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery.
    GetNextToken();
  }
}

void Parser::HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    GetNextToken();
  }
}