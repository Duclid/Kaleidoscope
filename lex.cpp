#include "lex.h"
#include <cctype>

int Lex::GetToken(){

    // 忽略空白字符
    while(std::isspace(last_char)){
        last_char = getchar();
    }

    // 识别字符串
    if(std::isalpha(last_char)){
        identifier_str = last_char;
        while (isalnum((last_char = getchar()))) {
            identifier_str += last_char;
        }
        if (identifier_str == "def") {
            return TOKEN_DEF;
        } else if (identifier_str == "extern") {
            return TOKEN_EXTERN;
        } else {
            return TOKEN_IDENTIFIER;
        }
    }

    // 识别数值
    if (isdigit(last_char) || last_char == '.') {
        std::string num_str;

        do{
            num_str += last_char;
            last_char = getchar();
        }while (isdigit(last_char) || last_char == '.');

        num_val = strtod(num_str.c_str(), nullptr);
        return TOKEN_NUMBER;
    }

    // 忽略注释行
    if (last_char == '#') {
        do {
            last_char = getchar();
        } while (last_char != EOF && last_char != '\n' && last_char != '\r');
        // 如果不是文件结束，再读取下一行数据
        if (last_char != EOF) {
            return GetToken();
        }
    }

    // 识别文件结束
    if (last_char == EOF) {
        return TOKEN_EOF;
    }

    // 直接返回ASCII
    int this_char = last_char;
    last_char = getchar();
    return this_char;
}