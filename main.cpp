#include <iostream>
#include "NFA.cpp"
#include <fstream>
#include <ostream>

int main() {
    std::vector<TerminalToken> terminalTokens;
    TerminalToken add("add", "(\\+)");
    TerminalToken sub("sub", "(-)");
    TerminalToken mult("mult", "(\\*)");
    TerminalToken div("div", "(/)");
    TerminalToken comma("comma", "(,)");
    TerminalToken digit("digit", "|((0)(1)(2)(3)(4)(5)(6)(7)(8)(9))");
    TerminalToken relop("relop", "|(+((<)|((>)(=)()))+((>)|((<)(=)()))(=))");
    TerminalToken relop2("relop", "|((<)(>))");
    TerminalToken
            alpha("alpha",
                  "|((a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(r)(s)(t)(u)(v)(w)(x)(y)(z)(A)(B)(C)(D)(E)(F)(G)(H)(I)(J)(K)(L)(M)(N)(O)(P)(Q)(R)(S)(T)(U)(V)(W)(X)(Y)(Z))");
    //TerminalToken whitespace("whitespace", "+(( )*(( )))");
    TerminalToken whitespace("whitespace", "( )");
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

    terminalTokens = {print, whitespace};/*add, sub,  mult, div, comma, digit, relop, alpha, whitespace, print, RETURN, IF, then, GOTO, input,
                      let, clear, end, list, gosub, STRING};*/

    NFA nfa(terminalTokens);
    std::ifstream myfile("input.txt");
    std::ofstream outfile;
    outfile.open("output.txt");
    if (myfile.is_open()) {
        std::string fileContent = "";
        std::string line = "";
        while (getline(myfile, line, '\n')) {
            fileContent += line;
        }

        std::pair<bool, std::vector<TerminalToken>> result=nfa.generateTokens(fileContent);
        if(result.first){
            for(TerminalToken token:result.second){
                outfile<<token.getName();
            }
        }

    }
    else{
        std::cout<<"File cannot be found"<<std::endl;
    }
    return 0;
}
