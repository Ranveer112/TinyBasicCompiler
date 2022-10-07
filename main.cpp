#pragma once
#include <iostream>
#include "NFA.cpp"
#include "Grammar.cpp"
#include "Parser.cpp"
#include <fstream>
#include <ostream>

int main() {
    std::ifstream codeFile("input.txt");
    std::ifstream terminalFile("terminals.txt");
    std::ifstream nonTerminalFile("nonTerminals.txt");

    Grammar g(nonTerminalFile, terminalFile);
    NFA nfa(g);
    std::ofstream outfile;
    outfile.open("output.txt");
    if (codeFile.is_open()) {
        std::string fileContent = "";
        std::string line = "";
        while (getline(codeFile, line, '\n')) {
            fileContent += line + "\n";
        }
        const std::pair<bool, std::vector<std::pair<GrammarState, std::string>>> result=nfa.generateTokens(fileContent);
        if (result.first) {
            std::cout << "Code lexed properly" << std::endl;
            for(std::pair<GrammarState, std::string> t:result.second){
                std::cout<<t.first.getName()<<std::endl;
            }
            Parser parser(g);
            parser.parse(result.second);
            std::cout << parser.getStringOfAST();


        } else {
            std::cout << "Code not lexed properly" << std::endl;
        }

    } else {
        std::cout << "File cannot be found" << std::endl;
    }

    return 0;
}
