#include <iostream>
#include "TerminalToken.cpp"
#include "TinyBasicNFA.cpp"
int main() {
    TerminalToken add("add", "+");
    TerminalToken sub("sub", "-");
    TerminalToken mult("mult", "*");
    TerminalToken div("div", "/");
    TerminalToken comma("comma", ",");
    TerminalToken digit("digit", "|((0)(1)(2)(3)(4)(5)(6)(7)(8)(9)");
    return 0;
}
