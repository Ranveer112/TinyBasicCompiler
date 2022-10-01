#include <iostream>
#include "NFA.cpp"
#include "Grammar.cpp"
#include "Parser.cpp"
#include <fstream>
#include <ostream>

int main() {
    Grammar g;
    NFA nfa(g);
    std::ifstream codeFile("input.txt");
    std::ifstream terminalFile("terminals.txt");
    std::ifstream  nonTerminalFile("nonTerminals.txt");
    std::ofstream outfile;
    outfile.open("output.txt");
    if (codeFile.is_open()) {
        std::string fileContent = "";
        std::string line = "";
        while (getline(codeFile, line, '\n')) {
            fileContent += line + "\n";
        }

        const std::pair<bool, std::vector<std::pair<GrammarState, std::string>>> result = nfa.generateTokens(fileContent);
        if (result.first) {
            std::cout<<"Code lexed properly"<<std::endl;
            Parser parser(g);
            parser.parse(result.second);



        }
        else{
            std::cout<<"Code not lexed properly"<<std::endl;
        }

    } else {
        std::cout << "File cannot be found" << std::endl;
    }
    return 0;
}
