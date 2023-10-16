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

#include "token.h"
using namespace std;

// ====================//
//      词法分析器      //
// ====================//

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


// ====================//
//      语法分析器      //
// ====================//

//////////////////////////////// 表达式 ////////////////////////////////
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

//////////////////////////////// 函数 ////////////////////////////////

/**
 * @brief 原型用于表示一个函数的原型，用于捕获函数的名称、各个函数的参数等
 * 
 */
class PrototypeAST{
public:
    PrototypeAST(string name, vector<unique_ptr<ExprAST>> args)
        : Name(name), Args(move(args)){}
private:
    string Name;
    vector<unique_ptr<ExprAST>> Args;
};

class FunctionAST{
public:
    FunctionAST(unique_ptr<PrototypeAST> proto, unique_ptr<ExprAST> body)
        : Proto(move(proto)), Body(move(body)) {}
private:
    unique_ptr<PrototypeAST> Proto;
    unique_ptr<ExprAST> Body; 
};


//////////////////////////////// Main ////////////////////////////////
void PrintLogo(){
    std::cout << " ____  _  _  ____ " << std::endl;
    std::cout << "(    \\/ )( \\(  _ \\" << std::endl;
    std::cout << " ) D () __ ( ) __/" << std::endl;
    std::cout << "(____/\\_)(_/(__)" << std::endl;
    std::cout << std::endl;
}

int main(){
    PrintLogo();
    // test token
    cout << gettok() << endl;
    return 0;
}
