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
	locals[
		int btype
	]
    : 'int'
    | 'bool'
    | 'float'
    | 'double'
    ;

constDef
	locals[
		int btype,
		int array_len
	]
    : Ident ('[' IntConst ']')?'=' constInitVal
    ;

constInitVal
    locals[
		int btype,
        int array_len
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
	locals[
		int btype,
		int array_len
	]
    : Ident ('[' IntConst ']')? ('=' constInitVal)?
    ;

funcDef
    : funcType Ident '(' (funcFParams)? ')' block;

funcType
    locals[
        int ret_type
    ]
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
    locals[
        int btype
    ]
    : bType Ident ('['']')?
    ;

block
	locals[
		int in_loop,
		int ret_type
	]
    : '{' (blockItem)* '}'
    ;

blockItem
	locals[
		int in_loop,
		int ret_type
	]
    : decl
    | stmt
    ;

stmt
	locals[
		int in_loop,
		int ret_type
	]
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
    locals[
        int btype,
        int array_len
    ]
    : addExp
    | boolConst
    ;

cond
    : lOrExp
    ;

lVal
    locals[
        int btype,
        int array_len,
    ]
    : Ident ('[' exp ']')?
    ;

primaryExp
    locals[
        int btype,
        int array_len,
    ]
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
        int btype
    ]
    : IntConst
    | SmallConst
    | ScienceConst
    | FloatConst
    ;

unaryExp
    locals[
        int btype,
        int array_len
    ]
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
	locals[
        int btype,
        int array_len
    ]
    : unaryExp
    | mulExp ('*' | '/' | '%') unaryExp
    ;

addExp
	locals[
        int btype,
        int array_len
    ]
    : mulExp
    | addExp ('+' | '-') mulExp
    ;

relExp
    locals[
        int btype,
    ]
    : addExp
    | relExp ('<' | '>' | '>=' | '<=') addExp
    | boolConst
    ;

eqExp
    locals[
        int btype,
    ]
    : relExp
    | eqExp ('==' | '!=') relExp
    ;

lAndExp
    locals[
        int btype,
    ]
    : eqExp
    | lAndExp '&&' eqExp
    ;

lOrExp
    locals[
        int btype,
    ]
    : lAndExp
    | lOrExp '||' lAndExp
    ;

// constExp
//     locals[
//         int basic_or_array_and_type,
//     ]
//     : number            #constExpNumber
//     | BoolConst         #constExpBoolConst
//     ;
constExp
    locals[
        int btype,
    ]
    : number            #constExpNumber
    | boolConst         #constExpBoolConst
    ;

boolConst : 'true' | 'false';


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