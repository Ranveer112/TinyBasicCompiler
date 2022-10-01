//
// Created by ranve on 9/28/2022.
//
#include "Grammar.cpp"

class ASTNode {
    std::vector<ASTNode *> getSubtree() {
        return this->subtree;
    }

private:
    std::vector<ASTNode *> subtree;
    std::string symbolName;
    std::string code;

    friend class Parser;

};

class Parser {
public:
    Parser(Grammar &grammar) {
        this->grammar = grammar;
    }

    bool parse(const std::vector<std::pair<GrammarState, std::string>> &tokens) {
        this->root = new ASTNode;
        int tokenId = 0;
        return canParse(this->grammar.getStartingState(), tokenId, root, tokens);
    }

    bool canParse(GrammarState *currentSymbol, int &tokenId,
                  ASTNode *currNode, const std::vector<std::pair<GrammarState, std::string>> &tokens) {
        if (tokenId == tokens.size()) {
            return true;
        } else if (currentSymbol->isTerminal()) {
            if (tokens[tokenId].first.getName() != currentSymbol->getName()) {
                return false;
            } else {
                currNode->code = tokens[tokenId].second;
                currNode->symbolName = tokens[tokenId].first.getName();
                tokenId++;
                return true;
            }
        } else {
            int productionToChoose = -1;
            std::vector<ASTNode *> subtree;
            const std::vector<std::vector<GrammarState *>> productions = currentSymbol->getProductions();
            for (int productionId = 0; productionId < productions.size(); productionId++) {
                if (currentSymbol->isViableProduction(tokens[tokenId].first, productionId)) {
                    productionToChoose = productionId;
                }
            }
            if (productionToChoose == -1) {
                for (int productionId = 0; productionId < currentSymbol->getProductions().size(); productionId++) {
                    if (currentSymbol->isEpsilon(productionId)) {
                        currNode->symbolName = epsilon;
                        return true;
                    } else {
                        return false;
                    }
                }
            } else {
                bool possibleToParseViaCurrentProduction = true;
                currNode->code = tokens[tokenId].second;
                currNode->symbolName = tokens[tokenId].first.getName();
                for (GrammarState *symbolFromProduction: productions[productionToChoose]) {
                    ASTNode *subtreeNode = new ASTNode;
                    possibleToParseViaCurrentProduction =
                            possibleToParseViaCurrentProduction &&
                            canParse(symbolFromProduction, tokenId, subtreeNode, tokens);
                    subtree.push_back(subtreeNode);
                }
                currNode->subtree = subtree;
                return possibleToParseViaCurrentProduction;
            }
        }
    }
    std::string getStringOfAST() const{

    }
    const ASTNode *getRoot() {
        return this->root;
    }

private:
    ASTNode *root;
    Grammar grammar;
};