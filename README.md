Compiler for TinyBasic


Grammer
```

    NON-TERMINALS:  
    
    line ::= number statement CR | statement CR
 
    statement ::= print expr-list
                  if expression relop expression then statement
                  goto expression
                  input var-list
                  let var = expression
                  gosum expression
                  return
                  clear
                  list
                  run
                  end
 
    expr-list ::= (string|expression) (comma (string|expression) )*
 
    var-list ::= var (comma var)*
 
    expression ::= whitespace* (sign|ε) whitespace* term whitespace* (sign whitespace* term)* whitespace* 
 
    term ::= factor whitespace*  (ops whitespace*  factor)* whitespace* 
 
    factor ::= var | number | (expression)
 
    number ::= (whitespace*) digit digit* (whitespace*)
 
 
    TERMINALS:
    
    var ::= whitespace* (A | B | C ... | Y | Z) whitespace*
    
    digit ::= 0 | 1 | 2 | 3 | ... | 8 | 9
 
    relop ::= whitespace* (< (>|=|ε) | > (<|=|ε) | =) whitespace*

    string ::= whitespace* " ( |!|#|$ ... -|.|/|digit|: ... @|A|B|C ... |X|Y|Z)* " whitespace*
    
    sign ::= whitespace* (+|-) whitespace*
    
    ops ::= whitespace* (*|/) whitespace* 
    
    comma ::= whitespace* , whitespace*
        
    print ::= whitespace* PRINT whitespace*
    
    if ::= whitespace* IF whitespace*
    
    then ::= whitespace* THEN whitespace*
    
    goto ::= whitespace* GOTO whitespace*
    
    input ::= whitespace* INPUT whitespace*
    
    let ::= whitespace* LET whitespace*
    
    gosub ::= whitespace* GOSUB whitespace*
    
    return  ::= whitespace* RETURN whitespace*
    
    clear ::= whitespace* CLEAR whitespace*
    
    list ::= whitespace* LIST whitespace*
    
    run ::= whitespace* RUN whitespace*
    
    end ::= whitespace* END whitespace*
```