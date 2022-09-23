Compiler for TinyBasic


Grammer


```
    prog ::= ε
             | miscspace line prog

    line ::= number whitespace statement miscspace newline 
             | statement miscspace newline
 
    statement ::= PRINT whitespace expr-list
                  | IF whitespace expression miscspace relop miscspace expression whitespace THEN whitespace statement
                  | GOTO whitespace expression
                  | INPUT whitespace var-list
                  | LET whitespace var miscspace = miscspace expression
                  | GOSUB whitespace expression
                  | RETURN
                  | CLEAR
                  | LIST
                  | RUN
                  | END
 
    expr-list ::= stringOrExp miscspace successiveExpr-list

    successiveExpr-list::= ε
                           | comma miscspace stringOrExp miscspace successiveExpr-list

    stringOrExp ::= string
                    | expression 
 
    var-list ::= var miscspace successiveVar-list

    successiveVar-list ::= ε
                          | comma miscspace var miscspace successiveVar-list


    expression ::= additionOrSubOrEpsilon term secondexpression 
  
    additionOrSubOrEpsilon ::= add
                               | sub
                               | ε 

    secondexpression ::= ε 
                         | additionOrSub term secondexpression

    additionOrSub ::= add
                      | sub
    
    term ::= factor successiveTerm

    successiveTerm ::= ε 
                       | miscSpace multiplicationOrDiv miscSpace successiveTerm
 
    factor ::= var 
               | number 
               | (expression)
 
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