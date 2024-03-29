//
// Created by ranve on 9/28/2022.
//
#pragma once

#include "Grammar.cpp"
#include "Testing.cpp"

class ASTNode {
public:
    std::vector<ASTNode *> getSubtree() {
        return this->subtree;
    }
    std::string getSymbolName(){
        return this->symbolName;
    }
    std::string getCode(){
        return this->code;
    }
private:
    std::vector<ASTNode *> subtree;
    std::string symbolName;
    std::string code; //for terminal nodes, empty otherwise. For terminal holds the text associated with the token

    friend class Parser;

};

class Parser {
public:
    Parser(Grammar &grammar) {
        this->grammar = grammar;
    }

    bool canParse(const std::vector<std::pair<GrammarState, std::string>> &tokens) {
        this->root = new ASTNode;
        int tokenId = 0;
        parseFrom(this->grammar.getStartingState(), tokenId, root, tokens);
        bool didParse = (tokenId == tokens.size());
        trimAST(this->root);
        return didParse;
    }

    std::string getStringOfAST() const {
        std::string indent = "";
        return getStringHelper(indent, this->root);
    }


    const ASTNode *getRoot() {
        return this->root;
    }



private:
    ASTNode *root;
    Grammar grammar;

    std::string getStringHelper(std::string &indent, const ASTNode *node) const {
        std::string result = "";
        result += indent + "{\n";
        result += indent + "\tSymbolName: " + node->symbolName + "\n";
        result += indent + "\tCode :" + node->code + "\n";
        result += indent + "\tSubtree :\n";
        for (ASTNode *subtreeNode: node->subtree) {
            indent += "\t";
            result += getStringHelper(indent, subtreeNode);
            indent.pop_back();
        }
        result += indent + "}\n";
        return result;
    }
    //TODO :: Make error specific to the symbol
    void parseFrom(GrammarState *currentSymbol, int &tokenId,
                   ASTNode *currNode, const std::vector<std::pair<GrammarState, std::string>> &tokens) {
        if (tokenId == tokens.size()) {
            return;
        } else if (currentSymbol->isTerminal()) {
            if (tokens[tokenId].first.getName() == currentSymbol->getName()) {
                currNode->code = tokens[tokenId].second;
                currNode->symbolName = currentSymbol->getName();
                tokenId++;
            }
            return;
        } else {
            int productionToChoose = -1;
            const std::vector<std::vector<GrammarState *>> productions = currentSymbol->getProductions();
            for (int productionId = 0; productionId < productions.size(); productionId++) {
                if (currentSymbol->isViableProduction(tokens[tokenId].first, productionId)) {
                    productionToChoose = productionId;
                }
            }
            if (productionToChoose == -1) {
                for (int productionId = 0; productionId < currentSymbol->getProductions().size(); productionId++) {
                    if (currentSymbol->isEpsilon(productionId)) {
                        currNode->symbolName = currentSymbol->getName();
                    }
                }
                return;
            } else {
                currNode->symbolName = currentSymbol->getName();
                for (GrammarState *symbolFromProduction: productions[productionToChoose]) {
                    ASTNode *subtreeNode = new ASTNode;
                    parseFrom(symbolFromProduction, tokenId, subtreeNode, tokens);
                    currNode->subtree.push_back(subtreeNode);
                }
                return;
            }
        }
    }
};