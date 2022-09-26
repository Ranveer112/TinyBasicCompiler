#include <iostream>
#include "NFA.cpp"
#include "TinyBasicGrammar.cpp"
#include <fstream>
#include <ostream>

int main() {
    TinyBasicGrammar grammar();
    NFA nfa(grammar);
    std::ifstream myfile("input.txt");
    std::ofstream outfile;
    outfile.open("output.txt");
    if (myfile.is_open()) {
        std::string fileContent = "";
        std::string line = "";
        while (getline(myfile, line, '\n')) {
            fileContent += line + "\n";
        }

        std::pair<bool, std::vector<TerminalToken>> result = nfa.generateTokens(fileContent);
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
