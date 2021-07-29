#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "../grammar/CACTLexer.h"
#include "../grammar/CACTParser.h"
#include "../grammar/CACTBaseListener.h"

#include "semanticAnalysis.h"

using namespace antlr4;

extern int semanticErrors;

std::string filename;

std::string find_filename(std::string dir)
{
	int position =  dir.find_last_of("/");
	int end_pos = dir.find_last_of(".cact");
	if(end_pos != dir.npos)
		dir.replace(end_pos-4, 5, "");
	if(position == dir.npos)
		return dir;
	else return dir.substr(position + 1);
}

int main(int argc, const char* argv[]) {
    std::ifstream stream;
    stream.open(argv[1]);
	std::string dir(argv[1]);
	filename = find_filename(dir);
	
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
	if(array_vector.size() > 0)
	{
		stream.close();
		stream.open(argv[1]);	
		std::ofstream tempFile("temp.cact", std::fstream::out);
		char templine[256];
		std::string tempstring;
		while(getline(stream, tempstring))
		{
			std::string repl_str = tempstring;
			for(int i = 0; i < repl_str.length(); i++)
				if(repl_str[i] == ' ' || repl_str[i] == '\t')
				{
					repl_str.replace(i--, 1, std::string(""));
				}
			for(int i = 0; i < array_vector.size(); i++)
			{
				if(array_vector_str[i] == "0")
					continue;
				else if(repl_str.find(array_vector_str[i]) == repl_str.npos)
					continue;
				else {
					int id = 0;
					while(sym_table.lookup_all(std::string("i") + std::to_string(id)) != nullptr)
						id++;
					std::string iterator = std::string("i") + std::to_string(id);
					std::string replace_1 = "int " + iterator + " = 0;\n";
					std::string replace_2 = "while (" + iterator + " < " + std::to_string(array_vector[i].second) + ")\n";
					for(int j = 0; j < array_vector.size() ; j++)
					{
						int position;
						if((position = tempstring.find(array_vector[j].first)) != tempstring.npos)
						{
							tempstring.replace(position, array_vector[j].first.length(), array_vector[j].first + "[" + iterator + "]");	
						}
					}
					
					tempstring += "\n";
					tempstring += ("\t" + iterator + "=" + iterator + "+1;\n");
					tempstring = replace_1 + replace_2 + "{\n" + tempstring + "}";
					break;
				}
			}
			tempFile << tempstring << std::endl;
		}
		tempFile.close();
		array_vector.clear();
		array_vector_str.clear();
		std::ifstream stream;
		stream.open("temp.cact");
		sym_table.reset();
		ANTLRInputStream input(stream);
		CACTLexer lexer(&input);
		CommonTokenStream tokens(&lexer);
		CACTParser parser(&tokens);

		tree::ParseTree *tree = parser.compUnit();

		int parser_err = parser.getNumberOfSyntaxErrors(), lexer_err = lexer.getNumberOfSyntaxErrors();
		std::cout << std::to_string(lexer_err) + " lexer error(s)" << std::endl;
		std::cout << std::to_string(parser_err) + " parser error(s)" << std::endl;
		SemanticAnalysis listener;
		tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
	}
	compUnit->code.printTAC();
	remove("temp.cact");

    std::cout << semanticErrors  << " semantic errors" << std::endl;
    return parser_err + lexer_err + semanticErrors;

}
