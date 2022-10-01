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
#include <set>


const std::string derivesSymbol = "::=";
const std::string epsilon = "ε";

class GrammarState {
public:
    GrammarState(const std::string &name, const std::string &pattern) {
        this->name = name;
        this->pattern = pattern;
        this->terminal = false;
    }

    GrammarState(const std::string &name) {
        this->name = name;
        this->terminal = true;
    }

    std::string getName() const {
        return this->name;
    }

    std::string getPattern() const {
        return this->pattern;
    }

    const std::vector<std::vector<GrammarState *>> getProductions() const {
        return this->productions;
    }

    void pushBackInProduction(const int &id, GrammarState *g) {
        while (this->productions.size() <= id) {
            this->productions.push_back({});
        }
        this->productions[id].push_back(g);

    }

    bool isEpsilon(const int &productionId) const {
        return this->productionsFirstSets[productionId].size() == 1 &&
               this->productionsFirstSets[productionId].find(epsilon) ==
               this->productionsFirstSets[productionId].end();
    }

    bool isViableProduction(const GrammarState &token, int productionId) const {
        if (productionId < this->productions.size()) {
            if (isEpsilon((productionId))) {
                return false;
            } else {
                return this->productionsFirstSets[productionId].find(token.getName()) !=
                       this->productionsFirstSets[productionId].end();
            }
        } else {
            return false;
        }
    }

    bool isTerminal() const {
        return this->terminal;
    }

    GrammarState() {}

private:
    bool terminal;
    std::string name;
    std::string pattern; //if terminal
    std::vector<std::vector<GrammarState *>> productions;
    std::set<std::string> firstSet;
    std::vector<std::set<std::string>>
            productionsFirstSets;

    friend class Grammar;
};

class Grammar {
public:
    Grammar() {}

    Grammar(std::ifstream &nonTerminalFiles, std::ifstream &terminalFiles) {
        std::map<std::string, GrammarState *> grammarStates;
        std::vector<std::string> nonTerminalFileLines;
        std::string line = "";
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
                this->terminalStates.push_back(grammarStates[symbol]);
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

    std::vector<GrammarState> getTerminalStates() const {
        std::vector<GrammarState> copyTerminalStates;
        for (GrammarState *terminalState: this->terminalStates) {
            copyTerminalStates.push_back(*terminalState);
        }
        return copyTerminalStates;
    }
    GrammarState* getStartingState(){
        return this->startingState;
    }

private:

    void computeFirstSets(std::map<std::string, GrammarState *> allStates) {
        bool areChanging = false;
        for (GrammarState *terminalState: terminalStates) {
            terminalState->firstSet.insert(terminalState->name);
            areChanging = true;
        }
        while (areChanging) {
            areChanging = false;
            for (auto[symbolName, g]: allStates)
                for (int productionIdForSymbol = 0;
                     productionIdForSymbol < g->productions.size(); productionIdForSymbol++) {
                    std::vector<GrammarState *> &currProduction = g->productions[productionIdForSymbol];
                    std::vector<GrammarState *>::iterator rhsSymbolInProduction = currProduction.begin();
                    while (rhsSymbolInProduction != currProduction.end() && (*rhsSymbolInProduction) == nullptr ||
                           (*rhsSymbolInProduction)->firstSet.find(epsilon) !=
                           (*rhsSymbolInProduction)->firstSet.end()) {
                        if ((*rhsSymbolInProduction) == nullptr) {
                            g->firstSet.insert(epsilon);
                            areChanging = true;
                        } else {
                            for (std::string element: (*rhsSymbolInProduction)->firstSet) {
                                g->firstSet.insert(element);
                                areChanging = true;
                            }
                        }
                        rhsSymbolInProduction++;
                    }
                    if (rhsSymbolInProduction != currProduction.end()) {
                        if ((*rhsSymbolInProduction) == nullptr) {
                            g->firstSet.insert(epsilon);
                            areChanging = true;
                        } else {
                            for (std::string element: (*rhsSymbolInProduction)->firstSet) {
                                g->firstSet.insert(element);
                                areChanging = true;
                            }
                        }
                    }
                }
        }

    }

    GrammarState *startingState;
    std::vector<GrammarState *> terminalStates;
};