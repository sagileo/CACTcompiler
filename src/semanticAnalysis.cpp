#include "semanticAnalysis.h"

int semanticErrors;

static void check_main(){
    FuncInfo * main;
    if((main = sym_table.lookup_func("main")) == NULL){
        semanticErrors++;
        std::cout << "No main function\n";
        return;
    }
    else if(main->ret_type != FUNC_INT){
        semanticErrors++;
        std::cout << "Unavailable return type for main\n";
        return;
    }
    else if(main->cnt_para > 0){
        semanticErrors++;
        std::cout << "Too much parameters in main\n";
        return;
    }
}

void SemanticAnalysis::exitCompUnit(CACTParser::CompUnitContext * ctx){
    check_main();
    // printf("Exiting CompUnit\n");
}

void SemanticAnalysis::enterConstDecl(CACTParser::ConstDeclContext * ctx)
{
	// constDecl : 'const' bType constDef (',' constDef)* ';'

    // printf("Entering ConstDecl\n");
}
void SemanticAnalysis::exitConstDecl(CACTParser::ConstDeclContext * ctx)
{
	int i;
	int line = ctx->getStart()->getLine();
	for(i = 0; i < ctx->constDef().size(); i++)
	{
		if(ctx->constDef()[i]->btype != ctx->bType()->btype && ctx->constDef()[i]->btype != BTYPE_ANY)
		{
			std::cout << line << ":" << ctx->constDef()[i]->getStart()->getCharPositionInLine() \
							<< ":" << "variable definition type mismatched" << std::endl;
			semanticErrors++;
		}	
	}


    std::cout << "const variable define: " << std::endl;
    for(const auto & const_def : ctx->constDef())
    {
        std::cout << "\tname: " << const_def->Ident()->getText().c_str() \
                << " type: " << ctx->bType()->getText().c_str() << std::endl;
		sym_table.addVar(const_def->Ident()->getText(), ctx->bType()->btype, const_def->array_len, 1);
    }
    // printf("Exiting ConstDecl\n");
}

void SemanticAnalysis::enterVarDecl(CACTParser::VarDeclContext * ctx)
{
    // printf("Entering VarDecl\n");
}
void SemanticAnalysis::exitVarDecl(CACTParser::VarDeclContext * ctx)
{
	int i;
	int line = ctx->getStart()->getLine();
	for(i = 0; i < ctx->varDef().size(); i++)
	{
		if(ctx->varDef()[i]->btype != ctx->bType()->btype && ctx->varDef()[i]->btype != BTYPE_ANY)
		{
			std::cout << line << ":" << ctx->varDef()[i]->getStart()->getCharPositionInLine() \
							<< ":" << "variable definition type mismatched" << std::endl;
			semanticErrors++;
		}
	}

    std::cout << "variable define: " << std::endl;
    for(const auto & var_def : ctx->varDef())
    {
        std::cout << "\tname: " << var_def->Ident()->getText().c_str() \
                << " type: " << ctx->bType()->getText().c_str() << std::endl;
		sym_table.addVar(var_def->Ident()->getText(), ctx->bType()->btype, var_def->array_len, 0);
    }
    // printf("Exiting VarDecl\n");
}

void SemanticAnalysis::exitConstExpNumber(CACTParser::ConstExpNumberContext * ctx)
{
	if(ctx->number())
	{
		ctx->btype = ctx->number()->btype;
	}
    // ctx->basic_or_array_and_type = ctx->number()->basic_or_array_and_type;
    // printf("Exiting ConstExpNumber\n");
}

