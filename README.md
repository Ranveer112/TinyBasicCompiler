Compiler for TinyBasic


Grammer


```
    line ::= (whitespace| ε) number whitespace statement (whitespace| ε) CR 
           | (whitespace| ε) statement (whitespace| ε) CR
 
    statement ::= PRINT whitespace expr-list
                  | IF whitespace expression (whitespace| ε) relop (whitespace| ε) expression whitespace THEN whitespace statement
                  | GOTO whitespace expression
                  | INPUT whitespace var-list
                  | LET whitespace var (whitespace| ε) = (whitespace| ε) expression
                  | GOSUB whitespace expression
                  | RETURN
                  | CLEAR
                  | LIST
                  | RUN
                  | END
 
    expr-list ::= (string|expression)
                  |(string|expression) comma (whitespace| ε) expr-list
 
    var-list ::= var 
                 | var (whitespace| ε) comma (whitespace| ε) var-list
 
    expression ::= (add|sub|ε) term secondexpression

    secondexpression ::= ε 
                         | (add|sub) term secondexpression
    
    term ::= factor 
            | factor (whitespace| ε) (mult|div) (whitespace| ε) term 
 
    factor ::= var 
               | number 
               | (expression)
 
    var ::= alpha 
            | (alpha var)
 
    number ::= digit 
               | (digit | number)

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
```