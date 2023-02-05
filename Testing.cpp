//
// Created by ranve on 2/4/2023.
//

#pragma once

#include "Grammar.cpp";
#include "string";
#include "vector";
class Testing{
public:
    Testing(){

    }
    void startTests(){
        //each class function
        //will test each function
        testsGrammerClass();
        //testsNFAClass();
        //testsParserClass();
        //testsInterpreter();
    }
    std::vector<std::string> getTestingStats(){
        return this->logs;
    }
private:

    testsGrammerClass(){
        std::vector<Gramma
    }
    std::vector<std::string> logs;
};