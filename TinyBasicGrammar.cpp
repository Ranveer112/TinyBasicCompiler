//
// Created by ranve on 9/25/2022.
//
#include <string>
#include <vector>
#include "TerminalToken.cpp"

class GrammarState {
public:
    GrammarState(const TerminalToken &tok) {
        this->tok = tok;
    }
    GrammarState(const  std::vector<std::vector<GrammarState *>> productions){
        this->productions=productions;
    }

private:
    TerminalToken tok; //is set to something value iff size(productions)==0
    std::vector<std::vector<GrammarState *>> productions;

    friend class TinyBasicGrammar;
};

class TinyBasicGrammar {
public:
    TinyBasicGrammar() {
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
        GrammarState* multiplicationOrDiv=new GrammarState({
    }

private:
    GrammarState *startingState;
};