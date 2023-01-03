//
// Created by ranve on 1/2/2023.
//

/*
Semantics check we can push here

1)Variable are only of number(int) type.
2)Expression are of two types 1)Label types
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