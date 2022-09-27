//
// Created by ranve on 9/25/2022.
//
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <istream>
#include "TerminalToken.cpp"
#include <map>

class GrammarState {
public:
    GrammarState(const std::string &name, const std::string &pattern) {
        this->name = name;
        this->pattern = pattern;
        this->isTerminal = false;
    }

    GrammarState(const std::string &name) {
        this->name = name;
        this->isTerminal = true;
    }

    std::string getName() {
        return this->name;
    }

    std::string getPattern() {
        return this->pattern;
    }

    std::vector<std::vector<GrammarState *>> getProductions() {
        return this->productions;
    }

    void pushBackInProduction(int id, GrammarState *g) {
        while (this->productions.size() <= id) {
            this->productions.push_back({});
        }
        this->productions[id].push_back(g);

    }

    GrammarState() {}

private:
    bool isTerminal;
    std::string name;
    std::string pattern; //if terminal
    std::vector<std::vector<GrammarState *>> productions;

    friend class TinyBasicGrammar;
};

class TinyBasicGrammar {
public:
    TinyBasicGrammar(std::ifstream &nonTerminalFiles, std::ifstream &terminalFiles) {
        std::map<std::string, GrammarState *> grammarStates;
        std::vector<std::string> nonTerminalFileLines;
        std::string line = "";
        const std::string derivesSymbol = "::=";
        const std::string epsilon = "ε";
        while (std::getline(nonTerminalFiles, line)) {
            nonTerminalFileLines.push_back(line);
        }
        std::vector<std::string> terminalFileLines;
        while (std::getline(nonTerminalFiles, line)) {
            if (line.find(derivesSymbol) != std::string::npos) {
                terminalFileLines.push_back(line);
            }
        }
        for (std::string &l: terminalFileLines) {
            std::string pattern = l.substr(l.find(derivesSymbol) + 2);
            std::string symbol = l.substr(0, l.find(derivesSymbol) - 1);
            if (grammarStates.find(symbol) == grammarStates.end()) {
                GrammarState *g = new GrammarState(symbol, pattern);
                grammarStates.insert({symbol, g});
                this->terminalStates.push_back(*grammarStates[symbol]);
            }
        }
        for (std::string &l: nonTerminalFileLines) {
            std::string buffer = "";
            for (size_t i = 0; i < l.size(); i++) {
                if (l[i] == ' ') {
                    if (buffer.size() != 0 && buffer != derivesSymbol) {
                        if (grammarStates.find(buffer) == grammarStates.end()) {
                            GrammarState *g = nullptr;
                            if (buffer != epsilon) {
                                g = new GrammarState(buffer);
                            }
                            grammarStates.insert({buffer, g});
                        }
                    }
                    buffer = "";

                } else {
                    buffer += l[i];
                }
            }
            if (buffer.size() != 0 && buffer != derivesSymbol) {
                if (grammarStates.find(buffer) == grammarStates.end()) {
                    GrammarState *g = nullptr;
                    if (buffer != epsilon) {
                        g = new GrammarState(buffer);
                    }
                    grammarStates.insert({buffer, g});
                }
            }
        }
        int productionId = 0;
        for (std::string &l: nonTerminalFileLines) {
            std::string buffer = "";
            std::string lhsSymbol = "";
            if (l.find(derivesSymbol) == std::string::npos) {
                productionId++;
            } else {
                productionId = 0;
            }
            for (size_t i = 0; i < l.size(); i++) {
                if (l[i] == ' ') {
                    if (buffer.size() != 0 && buffer != derivesSymbol) {
                        grammarStates[lhsSymbol]->pushBackInProduction(productionId, grammarStates[buffer]);
                    } else if (buffer == derivesSymbol) {
                        lhsSymbol = buffer;
                    }
                    buffer = "";

                } else {
                    buffer += l[i];
                }
            }
            if (buffer.size() != 0 && buffer != derivesSymbol) {
                grammarStates[lhsSymbol]->pushBackInProduction(productionId, grammarStates[buffer]);
            }
        }
    }

    std::vector<GrammarState> getTerminalStates() {
        std::vector<GrammarState> copyTerminalStates = this->terminalStates;
        return copyTerminalStates;
    }

private:
    GrammarState *startingState;
    std::vector<GrammarState> terminalStates;
};