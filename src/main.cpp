#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "../grammar/CACTLexer.h"
#include "../grammar/CACTParser.h"
#include "../grammar/CACTBaseListener.h"

#include "semanticAnalysis.h"

using namespace antlr4;

extern int semanticErrors;

int main(int argc, const char* argv[]) {
    std::ifstream stream;
    stream.open(argv[1]);
    ANTLRInputStream input(stream);
    CACTLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CACTParser parser(&tokens);
    //std::cout << parser.getNumberOfSyntaxErrors() << std::endl;

    tree::ParseTree *tree = parser.compUnit();
	// std::cout << "-------------------------Print AST:--------------------------" << std::endl;
   	// std::cout << tree->toStringTree(&parser) << std::endl;


    int parser_err = parser.getNumberOfSyntaxErrors(), lexer_err = lexer.getNumberOfSyntaxErrors();
    std::cout << std::to_string(lexer_err) + " lexer error(s)" << std::endl;
    std::cout << std::to_string(parser_err) + " parser error(s)" << std::endl;
    SemanticAnalysis listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);


    std::cout << semanticErrors  << " semantic errors" << std::endl;
    return parser_err + lexer_err + semanticErrors;

}
