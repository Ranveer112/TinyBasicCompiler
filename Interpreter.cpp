//
// Created by ranve on 1/2/2023.
//

/*
Semantics check we can push here

1)Expression are of two types 1)Label types
2)Variable type(let instiation)
3)Labels have global scope, and the not modifable
        For instance - (Assume welcome is a label) let welcome = 2(illegal)

4)

Let's maintain a symbol table, which holds the values for the vars and the AST node pointer for the labels
This way we can also check for



 Interpretor class


Information to maintain


1)Symbol table for each variables, and label. Denoting the current value, which is dynamic. Label values are
        consistent and computed before code execution is started.
Symbol table -> Two hashtables - one for variables(string(varname), int(computed value for the recentmost instiation)),
and other for labels(string(varname, ASTNode(the code associated with the label)).


2)Pointer to the ASTNode currently being executed(analogous to program counter)

3)Stack to simulate the callstack



Design

Functions needed:

Constructor which does some pre-processing-
1)Compute the labels and put them in the symbol table accordingly(name, ASTPointer)
2)Trim the var AST nodes for type "var" to such that ASTNode.code holds the name of the var.

1)Interpret variable/expr list
2)Interpret i/o operations
3)Interpret conditional
4)Interpret jumps
5)Interpret return
6)Interpret instantiation
7)Interpret expression
8)Interpret end

 */

#include "unordered_map"
#include "Parser.cpp"
#include "stack"

class Interpreter {
public:
    /*
     * Instatiates the following data member as such:
     *
     * 1) programTTree data member is same as the tree passed in.
     *
     * 2) programCounter is currently pointing to the root of the tree passed in
     *
     */
    Interpreter(ASTNode *programTree) {
        this->programTree = programTree;
        this->programCounter = this->programTree;
    }


    /*
     *  1)Interpret variable/expr list
        2)Interpret i/o operations
        3)Interpret conditional
        4)Interpret jumps
        5)Interpret return
        6)Interpret instantiation
        7)Interpret expression
        8)Interpret end

     */
private:
    /*
        * Traverses the tree and populates the symbolTable by <labelName, lineNo>
        * In addition, the "rootForLine" is also computed. ith entry in the container holds
        * the root node for the ith line in code.
        */
    void computeLabelsAndLineNo() {
        computeLabelsAndLineNoHelper(this->programTree);
    }

    void computeLabelsAndLineNoHelper(ASTNode *currNode) {
        if (currNode != nullptr && currNode->getSymbolName() == "prog") {
            //prog ::= epsilon
            //         miscspace line prog
            for (ASTNode *children: currNode->getSubtree()) {
                computeLabelsAndLineNoHelper(children);
            }
        } else if (currNode != nullptr && currNode->getSymbolName() == "line") {
            int lineNumber = this->rootForLine.size();
            this->rootForLine.push_back(currNode);
            for (ASTNode *children: currNode->getSubtree()) {
                if (children->getSymbolName() == "label") {
                    ASTNode *labelNode = children;
                    ASTNode *varNode = children->getSubtree()[0]; //label ::= var miscspace colon (Points to var Node)
                    std::string nameOfLabel = computeName(varNode);
                    if (tableForLabels.find(nameOfLabel) != tableForLabels.end()) {
                        tableForLabels[nameOfLabel] = lineNumber;
                    } else {
                        //error of duplicate label names
                        this->errorTrace.push_back("Duplicate label found for label :" + nameOfLabel);
                    }
                }
            }
        }
    }

    /*
     * Computes the "name" of the variable
     */
    std::string interpretName(ASTNode *varNode) {
        //not an epsilon node
        if (varNode != nullptr && varNode->getSubtree().size() > 0) {
            std::string name = "";
            for (ASTNode *childrenVarNode: varNode->getSubtree()) {
                name += interpretName(
                        childrenVarNode); //childrenVarNode could denote a node of var/successiveName/epsilon
            }
            return name;
        } else if (varNode != nullptr && varNode->getSubtree().size() == 0) { //terminal node(alpha)
            return varNode->getCode();
        }
    }

    std::string interpretOperator(ASTNode *operatorNode) {
        return
    }

    std::string interpretNumber(ASTNode *numberNode) {
        if (numberNode != nullptr) {
            //number :: = digit succesivenumber
            //successivenumber ::= epsilon
            //                     digit successivenumber
            std::string leadingDigit = numberNode->getSubtree()[0]->getCode();
            ASTNode *suffixDigitsNode = numberNode->getSubtree()[1];
            return leadingDigit + interpretNumber(suffixDigitsNode);
        } else {
            return "";
        }
    }

    //
    int computeValueFromOperatorsAndValues(std::vector<std::string> expression) {

    }

    int interpretValueOfTerm(ASTNode *termNode) {
        std::vector<std::string> termLine;
        accumulateFactors(termNode, termLine);
        //Now interpret the termLine
    }

    int interpretFactor(ASTNode *factorNode) {
        std::vector<ASTNode *> childrens = factorNode->getSubtree();
        if (childrens[0]->getSymbolName() == "var") {
            ASTNode *varNode = childrens[0];
            if (tableForVariables.find(interpretName(varNode)) == tableForVariables.end()) {
                this->errorTrace.push_back(
                        "Variable not defined is being called. Do an instantiation with keyword let");
                return -1;
            } else {
                return tableForVariables[interpretName(varNode)];
            }
        } else if (childrens[0]->getSymbolName() == "number") {
            ASTNode *numberNode = childrens[0];
            return stoi(interpretNumber(numberNode));
        } else if (childrens[1]->getSymbolName() == "expression") {
            ASTNode *expressionNode = childrens[1];
            return interpretValueOfExpression(expressionNode);
        } else {
            errorTrace.push_back("Parsing error");
            return -1;
        }
    }

    void accumulateFactors(ASTNode *termNode, std::vector<std::string> &termLine) {
        for (ASTNode *childOfTerms: termNode->getSubtree()) {
            if (childOfTerms->getSymbolName() == "factor") {
                termLine.push_back(std::to_string(interpretFactor(childOfTerms)));
            } else if (childOfTerms->getSymbolName() != "miscspace") {
                accumulateFactors(childOfTerms, termLine);
            }
        }
    }


    int interpretValueOfExpression(ASTNode *expression) {
        if (expression->getSymbolName() == "var") {
            ASTNode *varNode = expression;
            std::string nameOfVariable = interpretName(varNode);
            if (this->tableForVariables.find(nameOfVariable) == this->tableForVariables.end()) {
                this->errorTrace.push_back(
                        "Variable not defined is being called. Do an instantiation with keyword let");
                return -1;
            } else {
                return this->tableForVariables[interpretName(varNode)];
            }
        } else if (expression->getSymbolName() == "number") {
            ASTNode *numberNode = expression;
            return std::stoi(interpretNumber(numberNode));
        } else {
            std::vector<std::string> expressionLine;
            for (ASTNode *childExpressions: expression->getSubtree()) {
                if (childExpressions->getSymbolName() == "multiplicationOrDiv" ||
                    childExpressions->getSymbolName() == "additionOrSub") {
                    ASTNode *operatorNode = childExpressions;
                    expressionLine.push_back(interpretOperator(operatorNode));
                } else if (childExpressions->getSymbolName() != "miscspace" ||
                           childExpressions->getSymbolName() != "openParen"
                           || childExpressions->getSymbolName() != "closeParen") {

                    if (childExpressions->getSymbolName() == "term") {
                        expressionLine.push_back(std::to_string(interpretValueOfTerm(childExpressions)));
                    } else {
                        expressionLine.push_back(std::to_string(interpretValueOfExpression(childExpressions)));
                    }
                }

            }
            //TODO:
            return ..;
        }
    }

    ASTNode *programCounter;
    ASTNode *programTree;
    std::unordered_map<std::string, int> tableForLabels;
    std::vector<ASTNode *> rootForLine;
    std::unordered_map<std::string, int> tableForVariables;
    std::stack<ASTNode *> callStack;
    std::vector<std::string> errorTrace;

};