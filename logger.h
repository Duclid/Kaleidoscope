#pragma once

/**
 * @brief 单例模式：日志类
 * 
 */
class Logger{

public:

    Logger& GetInstance(){
        static Logger instance;
        return instance;
    }


private:
    Logger(){}

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

};