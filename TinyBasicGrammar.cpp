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
        const std::string derivesSymbol = ":=";
        while (std::getline(nonTerminalFiles, line)) {
            nonTerminalFileLines.push_back(line);
        }
        std::vector<std::string> terminalFileLines;
        while (std::getline(nonTerminalFiles, line)) {
            terminalFileLines.push_back(line);
        }
        for (std::string &l: terminalFileLines) {
            std::string pattern = l.substr(l.find(derivesSymbol) + 2);
            std::string symbol = l.substr(0, l.find(derivesSymbol) - 1);
            if (grammarStates.find(symbol) == grammarStates.end()) {
                GrammarState *g = new GrammarState(symbol, pattern);
                grammarStates.insert({symbol, g});
            }
        }
        for (std::string &l: nonTerminalFileLines) {
            std::string buffer = "";
            for (size_t i = 0; i < l.size(); i++) {
                if (l[i] == ' ') {
                    if (buffer.size() != 0 && buffer != derivesSymbol) {
                        if (grammarStates.find(buffer) == grammarStates.end()) {
                            GrammarState *g = new GrammarState(buffer);
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
                    GrammarState *g = new GrammarState(buffer);
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
            }
            for (size_t i = 0; i < l.size(); i++) {
                if (l[i] == ' ') {
                    if (buffer.size() != 0 && buffer != derivesSymbol) {
                        grammarStates[lhsSymbol]->getProductions()[productionId].push_back(grammarStates[buffer]);
                    } else if (buffer == derivesSymbol) {
                        lhsSymbol = buffer;
                    }
                    buffer = "";

                } else {
                    buffer += l[i];
                }
            }
            if (buffer.size() != 0 && buffer != derivesSymbol) {
                grammarStates[lhsSymbol]->getProductions()[productionId].push_back(grammarStates[buffer]);
            }
        }
    }

    /*
    TerminalToken add("add", "(\\+)");
    TerminalToken sub("sub", "(-)");
    TerminalToken mult("mult", "(\\*)");
    TerminalToken div("div", "(/)");
    TerminalToken comma("comma", "(,)");
    TerminalToken digit("digit", "|((0)(1)(2)(3)(4)(5)(6)(7)(8)(9))");
    TerminalToken relop("relop", "|(+((<)|((>)(=)()))+((>)|((<)(=)()))(=))");
    TerminalToken
            alpha("alpha",
                  "|((a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(r)(s)(t)(u)(v)(w)(x)(y)(z)(A)(B)(C)(D)(E)(F)(G)(H)(I)(J)(K)(L)(M)(N)(O)(P)(Q)(R)(S)(T)(U)(V)(W)(X)(Y)(Z))");
    TerminalToken whitespace("whitespace", "+(( )*(( )))");
    TerminalToken print("print", "+((P)(R)(I)(N)(T))");
    TerminalToken RETURN("return", "+((R)(E)(T)(U)(R)(N))");
    TerminalToken IF("if", "+((I)(F))");
    TerminalToken then("then", "+((T)(H)(E)(N))");
    TerminalToken GOTO("goto", "+((G)(O)(T)(O))");
    TerminalToken input("input", "+((I)(N)(P)(U)(T))");
    TerminalToken let("let", "+((L)(E)(T))");
    TerminalToken clear("clear", "+((C)(L)(E)(A)(R)");
    TerminalToken end("end", "+((E)(N)(D))");
    TerminalToken list("list", "+((L)(I)(S)(T))");
    TerminalToken gosub("gosub", "+((G)(O)(S)(U)(B))");
    TerminalToken STRING("string", "+((\")*(|(" + alpha.getPattern() + digit.getPattern() + "))(\"))");
    TerminalToken newline("newline", "(\n)");
    GrammarState *stringState = new GrammarState(STRING);
    GrammarState *newlineState = new GrammarState(newline);
    GrammarState *gosubState = new GrammarState(gosub);
    GrammarState *listState = new GrammarState(list);
    GrammarState *endState = new GrammarState(end);
    GrammarState *clearState = new GrammarState(clear);
    GrammarState *letState = new GrammarState(let);
    GrammarState *inputState = new GrammarState(input);
    GrammarState *gotoState = new GrammarState(GOTO);
    GrammarState *ifState = new GrammarState(IF);
    GrammarState *printState = new GrammarState(print);
    GrammarState *returnState = new GrammarState(RETURN);
    GrammarState *whitespaceState = new GrammarState(whitespace);
    GrammarState *alphaState = new GrammarState(alpha);
    GrammarState *relopState = new GrammarState(relop);
    GrammarState *digitState = new GrammarState(digit);
    GrammarState *commaState = new GrammarState(comma);
    GrammarState *divState = new GrammarState(div);
    GrammarState *multState = new GrammarState(mult);
    GrammarState *subState = new GrammarState(sub);
    GrammarState *addState = new GrammarState(add);

    GrammarState *multiplicationOrDiv = new GrammarState();
    GrammarState *miscSpace = new GrammarState();
    GrammarState *successiveNumber = new GrammarState();
    GrammarState *number = new GrammarState();
    GrammarState *successiveVar = new GrammarState();
    GrammarState *var = new GrammarState();
    GrammarState *factor = new GrammarState();
    GrammarState *successiveTerm = new GrammarState();
    GrammarState *term = new GrammarState();
    GrammarState *additionOrSub = new GrammarState();
    GrammarState *secondExpression = new GrammarState();
    GrammarState *expression = new GrammarState();
    GrammarState *successiveVarList = new GrammarState();
    GrammarState *varList = new GrammarState();
    GrammarState *stringOrExp = new GrammarState();
    GrammarState *successiveExprList = new GrammarState();
    GrammarState *exprList = new GrammarState();
    GrammarState *statement = new GrammarState();
    GrammarState *line = new GrammarState();
    GrammarState *prog = new GrammarState();
    std::vector<std::vector<GrammarState *>> multiplicationOrDivProductions = {{multState},
                                                                               {divState}};
    std::vector<std::vector<GrammarState *>> miscSpaceProductions = {{nullptr},
                                                                     {whitespaceState}};
    std::vector<std::vector<GrammarState *>> successiveNumberProductions = {{nullptr},
                                                                            {digitState, successiveNumber}};
    std::vector<std::vector<GrammarState *>> numberProductions = {{digitState}, successiveNumber};
    std::vector<std::vector<GrammarState *>> successiveVarProductions = {{nullptr},
                                                                         {alphaState, successiveVar}};
    std::vector<std::vector<GrammarState *>> varProductions = {{alphaState, successiveVar}};
    std::vector<std::vector<GrammarState *>> factorProductions = {{number},
                                                                  {expression}};
    std::vector<std::vector<GrammarState *>> successiveTermProductions = {{nullptr},
                                                                          {multiplicationOrDiv, miscSpace, successiveTerm}};
    std::vector<std::vector<GrammarState *>> termProductions = {{factor, miscSpace, successiveTerm}};

    std::vector<std::vector<GrammarState *>>
            additionOrSubProductions = {{addState},
                                        {subState}};
    std::vector<std::vector<GrammarState *>> secondExpressionProductions = {{nullptr}, {additionOrSub, term,
                                                                                        secondExpression}};
    std::vector<std::vector<GrammarState *>> expressionProductions = {{additionOrSub, term, secondExpression}};
    std::vector<std::vector<GrammarState *>> successiveVarListProductions = {{nullptr},
                                                                             {commaState, miscSpace, var, miscSpace,
                                                                              successiveVarList}};
    std::vector<std::vector<GrammarState *>> varListProductions = {{var, miscSpace, successiveVarList}};

    std::vector<std::vector<GrammarState*>> stringOrExpProductions={{stringState}, {expression}};

    std::vector<std::vector<GrammarState*>> successiveExprListProductions={{}};
    */


private:
    GrammarState *startingState;
};