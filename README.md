~~Compiler~~ Interpretor for TinyBasic


Grammer


```
    prog ::= ε
             | miscspace line prog

    line ::= number whitespace statement miscspace newline 
             | statement miscspace newline
 
    statement ::= print whitespace expr-list
                  | if whitespace expression miscspace relop miscspace expression whitespace then whitespace statement
                  | goto whitespace expression
                  | input whitespace var-list
                  | let whitespace var miscspace = miscspace expression
                  | gosub whitespace expression
                  | return
                  | clear
                  | list
                  | run
                  | end
 
    expr-list ::= stringOrExp miscspace successiveExpr-list

    successiveExpr-list::= ε
                           | comma miscspace stringOrExp miscspace successiveExpr-list

    stringOrExp ::= string
                    | expression 
 
    var-list ::= var miscspace successiveVar-list

    successiveVar-list ::= ε
                          | comma miscspace var miscspace successiveVar-list


    expression ::= additionOrSub term secondexpression 
  
    additionOrSubOrEpsilon ::= add
                               | sub
                               | ε 

    secondexpression ::= ε 
                         | additionOrSub term secondexpression

    additionOrSub ::= add
                      | sub
    
    term ::= factor miscSpace successiveTerm    

    successiveTerm ::= ε 
                       | multiplicationOrDiv miscSpace successiveTerm
 
    factor ::= number 
               | expression
 
    var  ::= alpha successiveVar

    successiveVar ::=  ε
                       | alpha successiveVar
 
    number ::= digit successiveNumber

    successiveNumber ::= ε
                         | digit successiveNumber

    miscspace ::= ε
                  | whitespace

    multiplicationOrDiv ::= mult
                            | div

    Terminals 

    add ::= +
    
    sub ::= - 
   
    mult ::= *
    
    div ::= /
 
    comma ::= ,
 
    digit ::= 0 | 1 | 2 | 3 | ... | 8 | 9
 
    relop ::= < (>|=|ε) | > (<|=|ε) | =

    alpha ::= (a | b | c .... z | A | B | C ... | Y | Z)
        
    whitespace ::= ( ) ( )*

    print ::= PRINT

    return ::= RETURN

    if ::= IF

    then  ::= THEN
  
    goto ::= GOTO

    input ::= INPUT

    let ::= LET

    clear ::= CLEAR
 
    end ::= END

    list ::= LIST

    gosub ::= GOSUB

    string ::= "( |!|#|$ ... -|.|/|0|1|2....|9|: ... @|A|B|C ... |X|Y|Z)*"
    
    newline ::= \n
```

Description about some keywords, and the errors associated with them

```
GOSUB-Subroutine jump, stack pointer modified(label not found errors)

GOTO-Conditional jump(label not found errors)

RETURN-Stack pointer is modified, goback to the place(no gosub were ever called)

END-End program execution here(when the counter reaches at this point,
                               note we might skip this using jumps)
                               
LET-Variable instatiations/assignment(a name taken by label cannot be used)

INPUT-Read the data from I/O into the var-list variables(a label name cannot be used in var-list)

IF-Conditional(vars in expression should be used labels/institated variables)

LIST-Lists the code at that calculated expression line(vars should be used label names/instantiated variables)

PRINT-Prints the value of the expression in the expr-list(vars should be used labels/instantiated variables)
```

