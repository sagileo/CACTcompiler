grammar CACT;

// The language generated
options {
    language = Cpp;
}

@header {
    #include <vector>
    #include <string>
	#include "../src/threeAdressCode.h"
	#include "../src/genAssemble.h"
}

/********** Parser **********/
compUnit
	locals[
		threeAdressCode code,
	]
    : (funcDef | decl)+ EOF
    ;

decl
	locals[
		threeAdressCode code,
	]
    : constDecl
    | varDecl
    ;

constDecl
	locals[
		threeAdressCode code,
	]
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
		int array_len,
		threeAdressCode code,
	]
    : Ident ('[' IntConst ']')?'=' constInitVal
    ;

constInitVal
    locals[
		int btype,
        int array_len,
		char ** val
    ]
    : constExp                                             
    | '{' (constExp (',' constExp)* )? '}'
    ;

varDecl
	locals[
		threeAdressCode code,
	]
    : bType varDef (',' varDef)* ';'
    ;

varDef
	locals[
		int btype,
		int array_len,
		threeAdressCode code,
	]
    : Ident ('[' IntConst ']')? ('=' constInitVal)?
    ;

funcDef
	locals[
		threeAdressCode code,
		struct paramList * plist,
	]
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
	locals[
		struct paramList * plist,
	]
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
		int ret_type,
		std::string breaknext,
		std::string contnext,
		threeAdressCode code,
	]
    : '{' (blockItem)* '}'
    ;

blockItem
	locals[
		int in_loop,
		int ret_type,
		std::string breaknext,
		std::string contnext,
		threeAdressCode code,
	]
    : decl
    | stmt
    ;

stmt
	locals[
		int in_loop,
		int ret_type,
		std::string next,
		std::string breaknext,
		std::string contnext,
		threeAdressCode code,
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
        int array_len,
		std::string temp,
		threeAdressCode code,
    ]
    : addExp
    | boolConst
    ;

cond
	locals[
		std::string t,
		std::string f,
		threeAdressCode code,
		std::string temp,
	]
    : lOrExp
    ;

lVal
    locals[
        int btype,
        int array_len,
		std::string temp,
		threeAdressCode code,
    ]
    : Ident ('[' exp ']')?
    ;

primaryExp
    locals[
        int btype,
        int array_len,
		std::string temp,
		threeAdressCode code,
    ]
    : '(' exp ')'
    | lVal
    | number
    ;

number
    locals[
        int btype,
		char * val,
		threeAdressCode code,
    ]
    : IntConst		// BTYPE_INT
    | SmallConst	// BTYPE_DOUBLE
    | ScienceConst	// BTYPE_DOUBLE
    | FloatConst	// BTYPE_FLOAT
    ;

unaryExp
    locals[
        int btype,
        int array_len,
		std::string temp,
		threeAdressCode code,
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
	locals[
		std::string temp,
		threeAdressCode code,
	]
    : exp (',' exp)*
    ;

mulExp
	locals[
        int btype,
        int array_len,
		std::string temp,
		threeAdressCode code,
    ]
    : unaryExp
    | mulExp ('*' | '/' | '%') unaryExp
    ;

addExp
	locals[
        int btype,
        int array_len,
		std::string temp,
		threeAdressCode code,
    ]
    : mulExp
    | addExp ('+' | '-') mulExp
    ;

relExp
    locals[
        int btype,
		threeAdressCode code,
		std::string temp,
		std::string t,
		std::string f,
    ]
    : addExp
    | relExp ('<' | '>' | '>=' | '<=') addExp
    | boolConst
    ;

eqExp
    locals[
        int btype,
		threeAdressCode code,
		std::string temp,
		std::string t,
		std::string f,
    ]
    : relExp
    | eqExp ('==' | '!=') relExp
    ;

lAndExp
    locals[
        int btype,
		threeAdressCode code,
		std::string temp,
		std::string t,
		std::string f,
    ]
    : eqExp
    | lAndExp '&&' eqExp
    ;

lOrExp
    locals[
        int btype,
		threeAdressCode code,
		std::string temp,
		std::string t,
		std::string f,
    ]
    : lAndExp
    | lOrExp '||' lAndExp
    ;

constExp
    locals[
        int btype,
		char * val,
		std::string temp,
		threeAdressCode code,
    ]
    : number            #constExpNumber
    | boolConst         #constExpBoolConst
    ;

boolConst
	locals[
		char * val,
		std::string temp,
		threeAdressCode code,
	]
	: 'true' | 'false';


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