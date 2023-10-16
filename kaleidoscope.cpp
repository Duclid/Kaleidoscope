#include <iostream>
#include <cctype>
#include <cstdio>
#include <ctype.h>

#include "token.h"
using namespace std;
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



int main(){
    cout << gettok() << endl;
    return 0;
}