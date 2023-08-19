#pragma once
#include <string>
#include <iostream>

enum Token {
  TOKEN_EOF = -1,         // 文件结束标识符
  
  // 关键字
  TOKEN_DEF = -2,         // 关键字def
  TOKEN_EXTERN = -3,      // 关键字extern

  // primary
  TOKEN_IDENTIFIER = -4,  // 名字
  TOKEN_NUMBER = -5       // 数值
};

/**
 * @brief 词法解析器
 * 
 */
class Lex{

public:

    std::string identifier_str;   // 名字
    double num_val;               // 数字
    int last_char;
    // 从标准输入解析一个Token并返回
    Lex() = default;
    int GetToken();

};

