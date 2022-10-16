//
// Created by ranve on 9/25/2022.
//
#pragma once

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <istream>
#include <map>
#include <set>
#include <unordered_set>

const std::string DERIVES_SYMBOL = "::=";
const std::string EPSILON = "ε";
const std::string WHITESPACE = " ";
const std::string UNITE_OP = "+";
const std::string KLEENE_OP = "*";
const std::string OR_OP = "|";
const std::string OPEN_PARENTHESIS = "(";
const std::string CLOSE_PARENTHESIS = ")";
const std::string ESCAPE = "\\";
const std::string ESCAPE_STR="\\\\";
const std::string NEWLINE_STR = "\\n";
const std::string TAB_STR = "\\t";
const std::string NEWLINE = "\n";
const std::string TAB = "\t";
const std::string DELIMITER=" ";

const std::string SPECIAL_CHARACTERS[2][2] = {{NEWLINE_STR, NEWLINE},
                                            {TAB_STR,     TAB}};

class GrammarState {
public:
    GrammarState(const std::string &name, const std::string &pattern) {
        this->name = name;
        this->pattern = pattern;
        this->terminal = true;
    }

    GrammarState(const std::string &name) {
        this->name = name;
        this->terminal = false;
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

    void setName(const std::string &name) {
        this->name = name;
    }

    void setPattern(const std::string &pattern) {
        this->pattern = pattern;
    }

    bool isEpsilon(const int &productionId) const {
        return this->productionsFirstSets[productionId].size() == 1 &&
               this->productionsFirstSets[productionId].find(EPSILON) !=
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

    void addInFirstSet(std::string symbol) {
        this->firstSet.insert(symbol);
    }

    void addInProductionFirstSet(int productionId, std::string symbol) {
        while (this->productionsFirstSets.size() <= productionId) {
            this->productionsFirstSets.push_back({});
        }
        this->productionsFirstSets[productionId].insert(symbol);
    }


    bool isTerminal() const {
        return this->terminal;
    }

    GrammarState() {}


    std::vector<std::set<std::string>>
            productionsFirstSets;
private:
    bool terminal;
    std::string name;
    std::string pattern; //if terminal
    std::vector<std::vector<GrammarState *>> productions;
    std::set<std::string> firstSet;
    /*std::vector<std::set<std::string>>
            productionsFirstSets;
*/
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
        while (std::getline(terminalFiles, line)) {
            if (line.find(DERIVES_SYMBOL) != std::string::npos) {
                for (auto specialCharacter: SPECIAL_CHARACTERS) {
                    while (line.find(specialCharacter[0]) != std::string::npos) {
                        int index = line.find(specialCharacter[0]);
                        line.erase(index, 2);
                        line.insert(index, specialCharacter[1]);
                    }
                }
                terminalFileLines.push_back(line);
            }
        }
        for (std::string &l: terminalFileLines) {
            std::string name = l.substr(0,
                                        l.find_first_of(WHITESPACE));
            std::size_t patternStartIndex = l.find_first_of(UNITE_OP + KLEENE_OP + OR_OP + OPEN_PARENTHESIS);
            std::size_t patternEndIndex = l.find_last_of(CLOSE_PARENTHESIS);
            std::string pattern = l.substr(patternStartIndex, patternEndIndex - patternStartIndex + 1);
            GrammarState *g = new GrammarState(name, pattern);
            grammarStates.insert({g->getName(), g});
            terminalStates.push_back(g);
        }
        for (std::string &l: nonTerminalFileLines) {
            std::string buffer = "";
            for (size_t i = 0; i < l.size(); i++) {
                if (l[i] == DELIMITER[0]) {
                    if (buffer.size() != 0 && buffer != DERIVES_SYMBOL) {
                        if (grammarStates.find(buffer) == grammarStates.end()) {
                            GrammarState *g = nullptr;
                            if (buffer != EPSILON) {
                                g = new GrammarState(buffer);
                            }
                            if (this->startingState == nullptr) {
                                this->startingState = g;
                            }
                            grammarStates.insert({buffer, g});
                        }
                    }
                    buffer.clear();

                } else {
                    buffer += l[i];
                }
            }
            if (buffer.size() != 0 && buffer != DERIVES_SYMBOL) {
                if (grammarStates.find(buffer) == grammarStates.end()) {
                    GrammarState *g = nullptr;
                    if (buffer != EPSILON) {
                        g = new GrammarState(buffer);
                    }
                    grammarStates.insert({buffer, g});
                }
            }
        }
        int productionId = 0;
        std::string lhsSymbol = "";
        for (std::string &l: nonTerminalFileLines) {
            std::string buffer = "";
            if (l.find(DERIVES_SYMBOL) == std::string::npos) {
                productionId++;
            } else {
                productionId = 0;
                lhsSymbol.clear();
            }
            for (size_t i = 0; i < l.size(); i++) {
                if (l[i] == DELIMITER[0]) {
                    if (buffer.size() != 0 && buffer != DERIVES_SYMBOL) {
                        if (lhsSymbol.size() != 0) {
                            grammarStates[lhsSymbol]->pushBackInProduction(productionId, grammarStates[buffer]);
                        } else {
                            lhsSymbol = buffer;
                        }
                    }
                    buffer.clear();

                } else {
                    buffer += l[i];
                }
            }

            if (buffer.size() != 0 && buffer != DERIVES_SYMBOL) {
                grammarStates[lhsSymbol]->pushBackInProduction(productionId, grammarStates[buffer]);
            }
        }
        computeFirstSets(grammarStates);

    }

    std::vector<GrammarState> getTerminalStates() const {
        std::vector<GrammarState> copyTerminalStates;
        for (GrammarState *terminalState: this->terminalStates) {
            copyTerminalStates.push_back(*terminalState);
        }
        return copyTerminalStates;
    }

    GrammarState *getStartingState() {
        return this->startingState;
    }

    friend std::ostream &operator<<(std::ostream &stream, Grammar &g) {
        std::unordered_set<std::string> visited;
        g.printHelper(stream, g.getStartingState(), visited);
        return stream;
    }


private:
    void printHelper(std::ostream &ostream, GrammarState *state, std::unordered_set<std::string> &visited) {
        visited.insert(state->getName());
        ostream << state->getName() << " ::=";
        if (state->isTerminal()) {
            ostream << state->getPattern() << "\n";
            return;
        } else {
            for (std::vector<GrammarState *> &production: state->productions) {
                for (GrammarState *symbolInProduction: production) {
                    if (symbolInProduction == nullptr) {
                        ostream << EPSILON << " ";
                    } else {
                        ostream << symbolInProduction->getName() << " ";
                    }
                }
                ostream << "\n";
            }
            for (std::vector<GrammarState *> &production: state->productions) {
                for (GrammarState *symbolInProduction: production) {
                    if (symbolInProduction != nullptr && visited.find(symbolInProduction->getName()) == visited.end()) {
                        ostream << "\n";
                        printHelper(ostream, symbolInProduction, visited);
                    }
                }
            }
        }
    }

    void computeFirstSets(std::map<std::string, GrammarState *> &allStates) {
        bool areChanging = false;
        for (GrammarState *terminalState: terminalStates) {
            terminalState->firstSet.insert(terminalState->name);
            areChanging = true;
        }
        while (areChanging) {
            areChanging = false;
            for (auto[symbolName, g]: allStates) {
                if (g != nullptr) {
                    for (int productionIdForSymbol = 0;
                         productionIdForSymbol < g->productions.size(); productionIdForSymbol++) {
                        std::vector<GrammarState *> &currProduction = g->productions[productionIdForSymbol];

                        std::vector<GrammarState *>::iterator rhsSymbolInProduction = currProduction.begin();

                        while (rhsSymbolInProduction != currProduction.end() && ((*rhsSymbolInProduction) == nullptr ||
                                                                                 (*rhsSymbolInProduction)->firstSet.find(
                                                                                         EPSILON) !=
                                                                                 (*rhsSymbolInProduction)->firstSet.end())) {
                            if ((*rhsSymbolInProduction) != nullptr) {
                                for (std::string element: (*rhsSymbolInProduction)->firstSet) {
                                    if (element != EPSILON && g->firstSet.find(element) == g->firstSet.end()) {
                                        g->addInFirstSet(element);
                                        g->addInProductionFirstSet(productionIdForSymbol, element);
                                        areChanging = true;
                                    }
                                }
                            }
                            rhsSymbolInProduction++;
                        }
                        if (rhsSymbolInProduction != currProduction.end()) {
                            for (std::string element: (*rhsSymbolInProduction)->firstSet) {
                                if (g->firstSet.find(element) == g->firstSet.end()) {
                                    g->addInFirstSet(element);
                                    g->addInProductionFirstSet(productionIdForSymbol, element);
                                    areChanging = true;
                                }
                            }
                        } else {
                            if (g->firstSet.find(EPSILON) == g->firstSet.end()) {
                                g->addInFirstSet(EPSILON);
                                g->addInProductionFirstSet(productionIdForSymbol, EPSILON);
                                areChanging = true;

                            }
                        }

                    }
                }
            }
        }

    }

    GrammarState *startingState = nullptr;
    std::vector<GrammarState *> terminalStates;
};