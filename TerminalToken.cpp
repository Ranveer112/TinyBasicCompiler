//
// Created by ranve on 9/9/2022.
//
#include <string>

class TerminalToken {
public:
    TerminalToken(){}

    TerminalToken(std::string name, std::string pattern) {
        this->name = name;
        this->pattern = pattern;
    }

    std::string getPattern() {
        return this->pattern;
    }

    std::string getName() {
        return this->name;
    }

private:
    std::string name;
    //pattern in postfix, '/' will be escape chracter
    //for example |(+(a)(b))*(d))

    std::string pattern;
};