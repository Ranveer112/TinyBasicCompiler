#include <iostream>
#include "NFA.cpp"
#include "TinyBasicGrammar.cpp"
#include <fstream>
#include <ostream>

int main() {
    TinyBasicGrammar g;
    NFA nfa(g);
    std::ifstream codeFile("input.txt");
    std::ifstream terminalFile("terminals.txt");
    std::ifstream  nonTerminalFile("nonTerminals.txt");
    std::ofstream outfile;
    outfile.open("output.txt");
    if (codeFile.is_open()) {
        std::string fileContent = "";
        std::string line = "";
        while (getline(myfile, line, '\n')) {
            fileContent += line + "\n";
        }

        std::pair<bool, std::vector<std::pair<GrammarState, std::string>>> result = nfa.generateTokens(fileContent);
        if (result.first) {
            for (TerminalToken token: result.second) {
                outfile << token.getName() << "\n";
            }
        }

    } else {
        std::cout << "File cannot be found" << std::endl;
    }
    return 0;
}
