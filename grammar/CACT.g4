grammar CACT;

// The language generated
options {
    language = Cpp;
}

@header {
    #include <vector>
    #include <string>
}

/********** Parser **********/
compUnit
    : (funcDef | decl)+ EOF
    ;

decl
    : constDecl
    | varDecl
    ;

constDecl
    : 'const' bType constDef (',' constDef)* ';'
    ;

bType
    : 'int'
    | 'bool'
    | 'float'
    | 'double'
    ;

constDef
    : Ident ('[' IntConst ']')*'=' constInitVal
    ;

constInitVal
    locals[
        int basic_or_array_and_type
    ]
    : constExp                                             
    | '{' (constExp (',' constExp)* )? '}'
    ;

varDecl
    : bType varDef (',' varDef)* ';'
    ;

/* varDecl
    : bType varDecl_ ';'
    ;

varDecl_
    : varDef varDecl__
    ;

varDecl__
    : (',' varDef varDecl__)?
    ; */

varDef
    : Ident ('[' IntConst ']')? ('=' constInitVal)?
    ;

funcDef
    : funcType Ident '(' (funcFParams)? ')' block;

funcType
    : 'void'
    | 'int'
    | 'float'
    | 'double'
    | 'bool'
    ;

funcFParams
    : funcFParam (','funcFParam)*
    ;

funcFParam
    : bType Ident ('['']')?
    ;

block
    : '{' (blockItem)* '}'
    ;

blockItem
    : decl
    | stmt
    ;

stmt
    : lVal '=' exp ';'
    | (exp)? ';'
    | block
    | 'if' '(' cond ')' stmt ('else' stmt)?
    | 'while' '(' cond ')' stmt
    | 'break' ';'
    | 'continue' ';'
    | 'return' exp? ';'
    ;

exp
    : addExp
    | BoolConst
    ;

cond
    : lOrExp
    ;

lVal
    : Ident ('[' exp ']')?
    ;

primaryExp
    : '(' exp ')'
    | lVal
    | number
    ;

/* number
    locals[
        int basic_or_array_and_type,
    ]
    : IntConst
    | SmallConst 
    | SmallConst FloatSign
    | ScienceConst
    | ScienceConst FloatSign
    ; */

number
    locals[
        int basic_or_array_and_type,
    ]
    : IntConst
    | SmallConst
    | ScienceConst
    | FloatConst
    ;

unaryExp
    : primaryExp
    | Ident '(' funcRParams? ')'
    | unaryOp unaryExp
    ;

unaryOp
    : '+'
    | '-'
    | '!'
    ;

funcRParams
    : exp (',' exp)*
    ;

mulExp
    : unaryExp
    | mulExp ('*' | '/' | '%') unaryExp
    ;

addExp
    : mulExp
    | addExp ('+' | '-') mulExp
    ;

relExp
    : addExp
    | relExp ('<' | '>' | '>=' | '<=') addExp
    | BoolConst
    ;

eqExp
    : relExp
    | eqExp ('==' | '!=') relExp
    ;

lAndExp
    : eqExp
    | lAndExp '&&' eqExp
    ;

lOrExp
    : lAndExp
    | lOrExp '||' lAndExp
    ;

constExp
    locals[
        int basic_or_array_and_type,
    ]
    : number            #constExpNumber
    | BoolConst         #constExpBoolConst
    ;


/********** Lexer **********/
Ident
    : IdentNondigit [a-zA-Z_0-9]*
    ;

/* fragment
IdentNondigit
    : [a-eA-E_]
    | [g-zG-Z_]
    | FloatSign
    ; */

fragment
IdentNondigit
    : [a-eA-Eg-zG-Z_]
    | FloatSign
    ;

FloatSign
    : 'f' 
    | 'F'
    ;

fragment
Digit
    : [0-9]
    ;

IntConst
    : DecimalConst 
    | OctalConst
    | HexadecimalConst
    ;

BoolConst : 'true' | 'false';

SmallConst
    : DecimalConst? '.' Digit*
    ;

ScienceConst
    : (SmallConst | DecimalConst) ('e' | 'E') ('+' | '-')? DecimalConst
    ;

DoubleConst
    : SmallConst | ScienceConst
    ;

FloatConst
    : DoubleConst ('f' | 'F')
    ;

fragment
DecimalConst
    : '0'
    | NonzeroDigit Digit*
    ;

fragment
OctalConst
    : '0' OctalDigit+
    ;

fragment
HexadecimalConst
    : HexadecimalPrefix HexadecimalDigit+
    ;

fragment
NonzeroDigit
    : [1-9]
    ;

fragment
OctalDigit
    : [0-7]
    ;

fragment
HexadecimalPrefix
    : '0x'
    | '0X'
    ;

fragment
HexadecimalDigit
    : [0-9a-fA-F]
    ;

NewLine
    : ('\r' '\n'? | '\n') 
      -> skip
    ;

WhiteSpace
    : [ \t]+ 
      -> skip
    ;

BlockComment
    : '/*' .*? '*/'
      -> skip
    ;

LineComment
    : '//' ~[\r\n]*
      -> skip
    ;