#include "semanticAnalysis.h"
#include "threeAdressCode.h"
#include "genAssemble.h"
#include "symbolTable.h"

int semanticErrors;

extern threeAdressCode TAC;
CACTParser::CompUnitContext *compUnit;

static void check_main() {
    FuncInfo * main;
    if((main = sym_table.lookup_func("main")) == NULL) {
        semanticErrors++;
        std::cout << "No main function\n";
        return;
    }
    else if(main->ret_type != FUNC_INT) {
        semanticErrors++;
        std::cout << "Unavailable return type for main\n";
        return;
    }
    else if(main->cnt_para > 0) {
        semanticErrors++;
        std::cout << "Too much parameters in main\n";
        return;
    }
}

void SemanticAnalysis::enterCompUnit(CACTParser::CompUnitContext * ctx) { 
	// printf("Entering CompUnit\n");
}
void SemanticAnalysis::exitCompUnit(CACTParser::CompUnitContext * ctx)
{
    check_main();
    // printf("Exiting CompUnit\n");

	/** TAC */
	if(ctx->decl().size() > 0){
		TACline line;
		line.op = TACOP_LABEL;
		line.arg1.init(BTYPE_ANY, 0);
		line.arg1.copyName("global");
		line.arg2.init(BTYPE_ANY, 1);
		line.result.init(BTYPE_ANY, 1);
		ctx->code.addline(line);
		for(int i = 0; i < ctx->decl().size(); i++){
			ctx->code.addlines(ctx->decl()[i]->code.lines, ctx->decl()[i]->code.cur_line);
		}
	}
	if(semanticErrors)
		return;
	// ctx->code.printTAC();
	// here to access decl code
	// ...
	if(array_vector.size() == 0)
	{
		std::string dir = "../output/" + filename + ".s";
		FILE * fp = fopen(dir.c_str() , "w");
		genDecl(fp, ctx->code);

		for(int i = 0; i < ctx->funcDef().size(); i++){
			ctx->code.codecat(ctx->funcDef()[i]->code);
			// ctx->funcDef()[i]->code.printTAC();
		}

		// here to access fundef code 
		// ...
		for(int i = 0; i < ctx->funcDef().size(); i++){
			genFunc(fp, ctx->funcDef()[i]->code, ctx->funcDef()[i]->plist);
		}
		genFloat(fp);

		fclose(fp);
	} 
	else 
	{
		for(int i = 0; i < ctx->funcDef().size(); i++){
			ctx->code.codecat(ctx->funcDef()[i]->code);
			// ctx->funcDef()[i]->code.printTAC();
		}
	}

	
	

	compUnit = ctx;

	// compUnit = ctx;
}

void SemanticAnalysis::enterDecl(CACTParser::DeclContext * ctx)
{ 
		
}
void SemanticAnalysis::exitDecl(CACTParser::DeclContext * ctx)
{ 
	// printf("Exiting Decl\n");

	/** TAC */
	int line = ctx->getStart()->getLine();
	if(ctx->constDecl() != nullptr){
		ctx->code.addlines(ctx->constDecl()->code.lines, ctx->constDecl()->code.cur_line);
	}
	else if(ctx->varDecl() != nullptr){
		ctx->code.addlines(ctx->varDecl()->code.lines, ctx->varDecl()->code.cur_line);
	}
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

	/** TAC */
	for(int i = 0; i < ctx->constDef().size(); i++){
		ctx->code.addlines(ctx->constDef()[i]->code.lines, ctx->constDef()[i]->code.cur_line);
	}
}

void SemanticAnalysis::enterBType(CACTParser::BTypeContext * ctx)
{ 
	if(ctx->getText() == "int"){
		ctx->btype = BTYPE_INT;
	}
	else if(ctx->getText() == "float"){
		ctx->btype = BTYPE_FLOAT;
	}
	else if(ctx->getText() == "double"){
		ctx->btype = BTYPE_DOUBLE;
	}
	else if(ctx->getText() == "bool"){
		ctx->btype = BTYPE_BOOL;
	}
	// printf("Entering BType\n");
}
void SemanticAnalysis::exitBType(CACTParser::BTypeContext * ctx)
{ 
	// printf("Exiting BType\n");
}

void SemanticAnalysis::enterConstDef(CACTParser::ConstDefContext * ctx)
{ 
	// constDef : : Ident ('[' IntConst ']')*'=' constInitVal
	// printf("Entering ConstDef\n");
}
void SemanticAnalysis::exitConstDef(CACTParser::ConstDefContext * ctx)
{ 
	// printf("Exiting ConstDef\n");
	int line = ctx->getStart()->getLine();
	if((ctx->IntConst() != nullptr) && (ctx->constInitVal() != nullptr))		// constDef : Ident '[' IntConst ']' '=' constInitVal
	{
		
		ctx->array_len = atoi(ctx->IntConst()->getText().c_str());
		ctx->btype = ctx->constInitVal()->btype;
		if(ctx->constInitVal()->array_len == 0)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
						<< ":" << "assigning value of basic type to variable of array type is not allowed" << std::endl;
			semanticErrors++;
		}
		
	} else if(ctx->IntConst()){		// constDef : Ident '[' IntConst ']'
		ctx->array_len = atoi(ctx->IntConst()->getText().c_str());
		ctx->btype = BTYPE_ANY;
	} else if(ctx->constInitVal()){		//constDef : Ident '=' constInitVal
		ctx->array_len = 0;
		ctx->btype = ctx->constInitVal()->btype;
		if(ctx->constInitVal()->array_len != 0)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
						<< ":" << "assigning value of array type to variable of basic type is not allowed" << std::endl;
			semanticErrors++;
		}
	} else {		//constDef : Ident
		ctx->array_len = 0;
		ctx->btype = BTYPE_ANY;
	}

	/** TAC */
	if(ctx->IntConst() != nullptr){		// constDef : Ident '[' IntConst ']' '=' constInitVal
		TACline line;
		line.op = TACOP_NONE;
		int size = ctx->btype == BTYPE_DOUBLE ? 8 : 4;
		size = size * ctx->array_len;
		line.arg1.init(BTYPE_INT, 1);
		line.arg1.copyImm(&size);
		line.arg2.init(BTYPE_INT, 1);
		line.result.init(BTYPE_INT, 1);
		ctx->code.addline(line);
		for(int i = 0; i < ctx->array_len; i++){
			TACline line;
			line.op = TACOP_WOFF;

			line.result.init((btype)ctx->btype, 0);
			std::string scopeid = "0";
			if(sym_table.curr_scope)
				scopeid = std::to_string(sym_table.curr_scope->id);
			line.result.copyName(ctx->Ident()->getText() + "_" + scopeid);

			line.arg1.init(BTYPE_INT, 1);
			int off = i * line.result.size;
			line.arg1.copyImm(&off);

			line.arg2.init((btype)ctx->btype, 1);
			line.arg2.copyImm(ctx->constInitVal()->val[i]);

			ctx->code.addline(line);
		}
	}
	else{		//constDef : Ident '=' constInitVal
		TACline line;
		line.op = TACOP_ASSIGN;
		line.result.init((btype)ctx->btype, 0);
		std::string scopeid = "0";
		if(sym_table.curr_scope)
			scopeid = std::to_string(sym_table.curr_scope->id);
		line.result.copyName(ctx->Ident()->getText() + "_" + scopeid);
		line.arg1.init((btype)ctx->btype, 1);
		line.arg1.copyImm(ctx->constInitVal()->val[0]);
		line.arg2.init(BTYPE_ANY, 1);
		ctx->code.addline(line);
	}
}

void SemanticAnalysis::enterConstInitVal(CACTParser::ConstInitValContext * ctx)
{ 
	// printf("Entering ConstInitVal\n");
}
void SemanticAnalysis::exitConstInitVal(CACTParser::ConstInitValContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->getStart()->getText() == "{")		// constInitVal : '{' (constExp (',' constExp)* )? '}'
	{
		if(ctx->constExp().size() > 0)
		{
			ctx->btype = ctx->constExp()[0]->btype;
			ctx->array_len = ctx->constExp().size();
			for(int i = 0; i < ctx->constExp().size(); i++)
			{
				if(ctx->btype != ctx->constExp()[0]->btype)
				{
					std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
								<< ":" << "type of init values mismatched" << std::endl;
					semanticErrors++;
				}
			}
		} else {
			ctx->btype = BTYPE_ANY;
			ctx->array_len = 1;
		}
	} else {	// constInitVal : constExp
		ctx->btype = ctx->constExp()[0]->btype;
		ctx->array_len = 0;
	}
	
	/** TAC */
	if(ctx->array_len == 0){
		ctx->val = (char **)malloc(sizeof(char*));
		ctx->val[0] = ctx->constExp()[0]->val;
	}
	else{
		ctx->val = (char **)malloc(sizeof(char*) * ctx->array_len);
		for(int i = 0; i < ctx->array_len; i++){
			ctx->val[i] = ctx->constExp()[i]->val;
		}
	}
	// printf("Exiting ConstInitVal\n");
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

	for(int i = 0; i < ctx->varDef().size(); i++){
		ctx->code.addlines(ctx->varDef()[i]->code.lines, ctx->varDef()[i]->code.cur_line);
	}
	for(int i = 0; i < ctx->code.cur_line; i++){
		if(ctx->code.lines[i].op == TACOP_ASSIGN && ctx->code.lines[i].arg1.type == BTYPE_ANY){
			ctx->code.lines[i].result.init(ctx->bType()->btype, 0);
			ctx->code.lines[i].arg1.init(ctx->bType()->btype, 1);
			if(ctx->code.lines[i].arg1.type == BTYPE_DOUBLE){
				double d = 0;
				ctx->code.lines[i].arg1.copyImm(&d);
			}
			else if(ctx->code.lines[i].arg1.type == BTYPE_FLOAT){
				float f = 0;
				ctx->code.lines[i].arg1.copyImm(&f);
			}
			else{
				int d = 0;
				ctx->code.lines[i].arg1.copyImm(&d);
			}
		}
		else if(ctx->code.lines[i].op == TACOP_WOFF && ctx->code.lines[i].arg2.type == BTYPE_ANY){
			ctx->code.lines[i].result.init(ctx->bType()->btype, 0);
			ctx->code.lines[i].arg2.init(ctx->bType()->btype, 1);
			if(ctx->code.lines[i].arg2.type == BTYPE_DOUBLE){
				double d = 0;
				ctx->code.lines[i].arg2.copyImm(&d);
			}
			else if(ctx->code.lines[i].arg2.type == BTYPE_FLOAT){
				float f = 0;
				ctx->code.lines[i].arg2.copyImm(&f);
			}
			else{
				int d = 0;
				ctx->code.lines[i].arg2.copyImm(&d);
			}
			int off = *(int*)ctx->code.lines[i].arg1.data * ctx->code.lines->result.size;
			ctx->code.lines[i].arg1.copyImm(&off);
		}
		else if(ctx->code.lines[i].op == TACOP_NONE && ctx->code.lines[i].arg2.type == BTYPE_ANY){
			int size = *(int *)ctx->code.lines[i].arg1.data;
			int width = ctx->bType()->btype == BTYPE_DOUBLE ? 8 : 4;
			size = size * width;
			ctx->code.lines[i].arg1.copyImm(&size);
		}
	}
}

void SemanticAnalysis::enterVarDef(CACTParser::VarDefContext * ctx)
{ 
	
}
void SemanticAnalysis::exitVarDef(CACTParser::VarDefContext * ctx)
{ 
	// printf("Exiting VarDef\n");
	int line = ctx->getStart()->getLine();
	if(ctx->IntConst() && ctx->constInitVal())		// varDef : Ident '[' IntConst ']' '=' constInitVal
	{
		
		ctx->array_len = atoi(ctx->IntConst()->getText().c_str());
		ctx->btype = ctx->constInitVal()->btype;
		if(ctx->constInitVal()->array_len == 0)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
						<< ":" << "assigning value of basic type to variable of array type is not allowed" << std::endl;
			semanticErrors++;
		}
		
	} else if(ctx->IntConst()){		// varDef : Ident '[' IntConst ']'
		ctx->array_len = atoi(ctx->IntConst()->getText().c_str());
		ctx->btype = BTYPE_ANY;
	} else if(ctx->constInitVal()){		//varDef : Ident '=' constInitVal
		ctx->array_len = 0;
		ctx->btype = ctx->constInitVal()->btype;
		if(ctx->constInitVal()->array_len != 0)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
						<< ":" << "assigning value of array type to variable of basic type is not allowed" << std::endl;
			semanticErrors++;
		}
	} else {		//varDef : Ident
		ctx->array_len = 0;
		ctx->btype = BTYPE_ANY;
	}

	/** TAC */
	if(ctx->constInitVal() != nullptr){
		if(ctx->IntConst() != nullptr){		// varDef : Ident '[' IntConst ']' '=' constInitVal
			TACline line;
			line.op = TACOP_NONE;
			int size = ctx->btype == BTYPE_DOUBLE ? 8 : 4;
			size = size * ctx->array_len;
			line.arg1.init(BTYPE_INT, 1);
			line.arg1.copyImm(&size);
			line.arg2.init(BTYPE_INT, 1);
			line.result.init(BTYPE_INT, 1);
			ctx->code.addline(line);
			for(int i = 0; i < ctx->array_len; i++){
				TACline line;
				line.op = TACOP_WOFF;
				line.result.init((btype)ctx->btype, 0);
				std::string scopeid = "0";
				if(sym_table.curr_scope)
					scopeid = std::to_string(sym_table.curr_scope->id);
				line.result.copyName(ctx->Ident()->getText() + "_" + scopeid);
				line.arg1.init(BTYPE_INT, 1);
				int off = i * line.result.size;
				line.arg1.copyImm(&off);
				line.arg2.init((btype)ctx->btype, 1);
				line.arg2.copyImm(ctx->constInitVal()->val[i]);
				ctx->code.addline(line);
			}
		}
		else{		//varDef : Ident '=' constInitVal
			TACline line;
			line.op = TACOP_ASSIGN;
			line.result.init((btype)ctx->btype, 0);
			std::string scopeid = "0";
			if(sym_table.curr_scope)
				scopeid = std::to_string(sym_table.curr_scope->id);
			line.result.copyName(ctx->Ident()->getText() + "_" + scopeid);
			line.arg1.init((btype)ctx->btype, 1);
			line.arg1.copyImm(ctx->constInitVal()->val[0]);
			line.arg2.init(BTYPE_ANY, 1);
			ctx->code.addline(line);
		}
	}
	else{
		if(ctx->IntConst() != nullptr){		// varDef : Ident '[' IntConst ']' '=' constInitVal
			TACline line;
			line.op = TACOP_NONE;
			int size = ctx->array_len;
			line.arg1.init(BTYPE_INT, 1);
			line.arg1.copyImm(&size);
			line.arg2.init(BTYPE_ANY, 1);
			line.result.init(BTYPE_INT, 1);
			ctx->code.addline(line);
			for(int i = 0; i < ctx->array_len; i++){
				TACline line;
				line.op = TACOP_WOFF;
				line.result.init((btype)ctx->btype, 0);
				std::string scopeid = "0";
				if(sym_table.curr_scope)
					scopeid = std::to_string(sym_table.curr_scope->id);
				line.result.copyName(ctx->Ident()->getText() + "_" + scopeid);
				line.arg1.init(BTYPE_INT, 1);
				int off = i;
				line.arg1.copyImm(&off);
				line.arg2.init((btype)ctx->btype, 1);
				ctx->code.addline(line);
			}
		}
		else{		//varDef : Ident '=' constInitVal
			TACline line;
			line.op = TACOP_ASSIGN;
			line.result.init((btype)ctx->btype, 0);
			std::string scopeid = "0";
			if(sym_table.curr_scope)
				scopeid = std::to_string(sym_table.curr_scope->id);
			line.result.copyName(ctx->Ident()->getText() + "_" + scopeid);
			line.arg1.init((btype)ctx->btype, 1);
			line.arg2.init(BTYPE_ANY, 1);
			ctx->code.addline(line);
		}
	}
}

void SemanticAnalysis::enterFuncDef(CACTParser::FuncDefContext * ctx)
{ 
	if(ctx->funcType()->getText() == "int"){
		ctx->funcType()->ret_type = FUNC_INT;
	}
	else if(ctx->funcType()->getText() == "float"){
		ctx->funcType()->ret_type = FUNC_FLOAT;
	}
	else if(ctx->funcType()->getText() == "double"){
		ctx->funcType()->ret_type = FUNC_DOUBLE;
	}
	else if(ctx->funcType()->getText() == "bool"){
		ctx->funcType()->ret_type = FUNC_BOOL;
	}
	else if(ctx->funcType()->getText() == "void"){
		ctx->funcType()->ret_type = FUNC_VOID;
	}
	ctx->block()->ret_type = ctx->funcType()->ret_type;
	sym_table.addFunc(ctx->Ident()->getText(), ctx->funcType()->ret_type, ctx->getStart()->getLine());
	// printf("Entering FuncDef\n");
}
void SemanticAnalysis::exitFuncDef(CACTParser::FuncDefContext * ctx)
{
	int line = ctx->getStart()->getLine();
	if(sym_table.curr_scope == NULL)return;
	sym_table.scope_ret();
	// printf("Exiting FuncDef\n");

	/** TAC */
	TACline tacline;
	tacline.op = TACOP_LABEL;
	tacline.arg1.init(BTYPE_ANY, 0);
	tacline.arg1.copyName(ctx->Ident()->getText());
	tacline.arg2.init(BTYPE_ANY, 1);
	tacline.result.init(BTYPE_ANY, 1);
	ctx->code.addline(tacline);
	ctx->code.addlines(ctx->block()->code.lines, ctx->block()->code.cur_line);
	
	if(ctx->funcFParams() != nullptr) ctx->plist = ctx->funcFParams()->plist;
	else ctx->plist = nullptr;
}

void SemanticAnalysis::enterFuncType(CACTParser::FuncTypeContext * ctx)
{ 
	// printf("Entering FuncType\n");
}
void SemanticAnalysis::exitFuncType(CACTParser::FuncTypeContext * ctx)
{ 
	// printf("Exiting FuncType\n");

}

void SemanticAnalysis::enterFuncFParams(CACTParser::FuncFParamsContext * ctx)
{ 
	// printf("Entering FuncFParams\n");
}
void SemanticAnalysis::exitFuncFParams(CACTParser::FuncFParamsContext * ctx)
{ 
	if(sym_table.curr_scope == NULL)return;
	for(int i = 0; i < ctx->funcFParam().size(); i++){
		sym_table.addPara(ctx->funcFParam()[i]->Ident()->getText(), ctx->funcFParam()[i]->btype, 0);
	}
	int line = ctx->getStart()->getLine();
	// std::cout << sym_table.curr_scope->curr_func->cnt_para << std::endl;
	// std::cout << sym_table.curr_scope->curr_func->name << std::endl;
	// printf("Exiting FuncFParams\n");

	ctx->plist = (struct paramList *)malloc(sizeof(struct paramList));
	ctx->plist->numParams = ctx->funcFParam().size();
	ctx->plist->p = (struct param *)malloc(sizeof(struct param) * ctx->plist->numParams);
	for(int i = 0; i < ctx->funcFParam().size(); i++){
		ctx->plist->p[i].btype = ctx->funcFParam()[i]->btype;
		VarInfo * var = sym_table.lookup_all(ctx->funcFParam()[i]->Ident()->getText());
		ctx->plist->p[i].name = ctx->funcFParam()[i]->Ident()->getText() + "_" + std::to_string(var->scopeID);
	}
}

void SemanticAnalysis::enterFuncFParam(CACTParser::FuncFParamContext * ctx)
{ 
	if(sym_table.curr_scope == NULL)return;
	if(ctx->bType()->getText() == "int"){
		ctx->btype = (btype)FUNC_INT;
	}
	else if(ctx->bType()->getText() == "float"){
		ctx->btype = (btype)FUNC_FLOAT;
	}
	else if(ctx->bType()->getText() == "double"){
		ctx->btype = (btype)FUNC_DOUBLE;
	}
	else if(ctx->bType()->getText() == "bool"){
		ctx->btype = (btype)FUNC_BOOL;
	}
	else if(ctx->bType()->getText() == "void"){
		ctx->btype = (btype)FUNC_VOID;
	}
	// printf("Entering FuncFParam\n");
}
void SemanticAnalysis::exitFuncFParam(CACTParser::FuncFParamContext * ctx)
{ 
	// printf("Exiting FuncFParam\n");

}

void SemanticAnalysis::enterBlock(CACTParser::BlockContext * ctx)
{ 
	int i;
	for(i = 0; i < ctx->blockItem().size(); i++)
	{
		ctx->blockItem()[i]->ret_type = ctx->ret_type;
		ctx->blockItem()[i]->in_loop = ctx->in_loop;
		ctx->blockItem()[i]->contnext = ctx->contnext;
		ctx->blockItem()[i]->breaknext = ctx->breaknext;
	}
	sym_table.addScope(ctx->getStart()->getLine());
}
void SemanticAnalysis::exitBlock(CACTParser::BlockContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	int i;
	for(i = 0; i < ctx->blockItem().size(); i++)
	{
		ctx->code.codecat(ctx->blockItem()[i]->code);
	}
	sym_table.scope_ret();
}

void SemanticAnalysis::enterBlockItem(CACTParser::BlockItemContext * ctx)
{ 
	if(ctx->decl())		// blockItem : decl
	{
		// intentionally left empty
	} else if(ctx->stmt()) {		// blockItem : stmt
		ctx->stmt()->ret_type = ctx->ret_type;
		ctx->stmt()->in_loop = ctx->in_loop;
		ctx->stmt()->contnext = ctx->contnext;
		ctx->stmt()->breaknext = ctx->breaknext;
	}
}
void SemanticAnalysis::exitBlockItem(CACTParser::BlockItemContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->decl())		// blockItem : decl
	{
		ctx->code.codecat(ctx->decl()->code);
	} else if(ctx->stmt()) {		// blockItem : stmt
		ctx->code.codecat(ctx->stmt()->code);
	}
}

void SemanticAnalysis::enterStmt(CACTParser::StmtContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->lVal() && ctx->exp())		// stmt : lVal '=' exp ';'
	{
		VarInfo *lVal_info = sym_table.lookup_all(ctx->lVal()->Ident()->getText());
		if(lVal_info == nullptr)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
						<< ":" << "variable used without definition" << std::endl;
			semanticErrors++;
			return;
		}
		if(lVal_info->is_const)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
						<< ":" << "assigning value to const variable is not allowed" << std::endl;
			semanticErrors++;
		}

		// ctx->lVal()->temp = mynewtemp();
		ctx->exp()->temp = mynewtemp();
	} else if(ctx->block()) {		// stmt : block
		// sym_table.addScope(line);
		ctx->block()->in_loop = ctx->in_loop;
		ctx->block()->ret_type = ctx->ret_type;
		ctx->block()->contnext = ctx->contnext;
		ctx->block()->breaknext = ctx->breaknext;
		// sym_table.curr_scope = sym_table.curr_scope->curr_func->scope.children[line];
	} else if(ctx->getStart()->getText() == std::string("if")) {		// stmt : 'if' '(' cond ')' stmt ('else' stmt)?
		assert(ctx->stmt().size() == 1 || ctx->stmt().size() == 2);
		if(ctx->stmt().size() == 1)				// stmt : 'if' '(' cond ')' stmt
		{
			ctx->stmt().at(0)->in_loop = ctx->in_loop;
			ctx->stmt().at(0)->ret_type = ctx->ret_type;

			ctx->next = mynewlabel();
			ctx->cond()->t = mynewlabel();
			ctx->cond()->temp = mynewtemp();
			ctx->cond()->f = ctx->stmt().at(0)->next = ctx->next;
		} else if(ctx->stmt().size() == 2) {		// stmt : 'if' '(' cond ')' stmt 'else' stmt
			ctx->stmt().at(0)->in_loop = ctx->in_loop;
			ctx->stmt().at(1)->in_loop = ctx->in_loop;
			ctx->stmt().at(0)->ret_type = ctx->ret_type;
			ctx->stmt().at(1)->ret_type = ctx->ret_type;
			
			ctx->next = mynewlabel();
			ctx->cond()->t = mynewlabel();
			ctx->cond()->f = mynewlabel();
			ctx->cond()->temp = mynewtemp();
			ctx->stmt().at(0)->next = ctx->stmt().at(1)->next = ctx->next;
		}
	} else if(ctx->getStart()->getText() == std::string("while")) {		// stmt : 'while' '(' cond ')' stmt
		ctx->stmt().at(0)->in_loop = 1;
		ctx->stmt().at(0)->ret_type = ctx->ret_type;

		ctx->next = mynewlabel();
		ctx->cond()->t = mynewlabel();
		ctx->cond()->f = ctx->next;
		ctx->cond()->temp = mynewtemp();
		ctx->stmt().at(0)->next = mynewlabel();
		ctx->stmt().at(0)->breaknext = ctx->next;
		ctx->stmt().at(0)->contnext = ctx->stmt().at(0)->next;
	} else if(ctx->getStart()->getText() == std::string("break")) {		// stmt : 'break' ';'
		if(ctx->in_loop != 1)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "break statement not within a loop" << std::endl;
			semanticErrors++;
		}
	} else if(ctx->getStart()->getText() == std::string("continue")) {		// stmt : 'continue' ';'
		if(ctx->in_loop != 1)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "continue statement not within a loop" << std::endl;
			semanticErrors++;
		}
	} else if(ctx->getStart()->getText() == std::string("return")) {		// stmt : 'return' exp? ';'
		
	} else if(ctx->exp()) {		// stmt : (exp)?
		
	}
}
void SemanticAnalysis::exitStmt(CACTParser::StmtContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->lVal() && ctx->exp())		// stmt : lVal '=' exp ';'
	{
		VarInfo *lVal_info;
		lVal_info = sym_table.lookup_all(ctx->lVal()->Ident()->getText());
		//ctx->lVal()->array_len = lVal_info->array_len;
		if(ctx->lVal()->btype != ctx->exp()->btype || ctx->lVal()->array_len != ctx->exp()->array_len)
		{
			std::cout << line << ":" << ctx->exp()->getStart()->getCharPositionInLine() \
						<< ":" << "basic type or array len mismatch" << std::endl;
			semanticErrors++;
		}
	} else if(ctx->getStart()->getText() == std::string("return")) {		// stmt : 'return' exp? ';'
		if(ctx->exp())
		{
			if(ctx->ret_type != ctx->exp()->btype)
			{
				std::cout << line << ":" << ctx->exp()->getStart()->getCharPositionInLine() \
							<< ":" << "wrong return type" << std::endl;
				semanticErrors++;
			}
		} else {
			if(ctx->ret_type != FUNC_VOID)
			{
				std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "wrong return type" << std::endl;
				semanticErrors++;
			}
		}
	}

	// generate TAC
	if(ctx->lVal() && ctx->exp())		// stmt : lVal '=' exp ';'
	{
		if(ctx->lVal()->exp())		// lval为数组
		{
			ctx->code.codecat(ctx->exp()->code);
			ctx->code.codecat(ctx->lVal()->code);

			VarInfo* vi = sym_table.lookup_all(ctx->lVal()->Ident()->getText());
			TACline tacline;
			tacline.op = TACOP_WOFF;
			tacline.arg1.init(BTYPE_INT, 0);
			tacline.arg1.copyName(ctx->lVal()->temp);
			tacline.arg2.init(ctx->exp()->btype, 0);
			tacline.arg2.copyName(ctx->exp()->temp);
			tacline.result.init(ctx->lVal()->btype, 0);
			tacline.result.copyName(ctx->lVal()->Ident()->getText() + "_" + std::to_string(vi->scopeID));
			ctx->code.addline(tacline);
		} else {		// lVal为Ident
			if(ctx->lVal()->array_len > 0)
			{
				array_vector.push_back(std::pair<std::string, int>(ctx->lVal()->Ident()->getText(), ctx->lVal()->array_len));
				array_vector_str.push_back(ctx->getText());
			}
			ctx->code.codecat(ctx->exp()->code);
			VarInfo* vi = sym_table.lookup_all(ctx->lVal()->Ident()->getText());
			TACline tacline;
			tacline.op = TACOP_ASSIGN;
			tacline.arg1.init(ctx->exp()->btype, 0);
			tacline.arg2.init(BTYPE_ANY, 1);
			tacline.arg1.copyName(ctx->exp()->temp);
			tacline.result.init(ctx->lVal()->btype, 0);
			tacline.result.copyName(ctx->lVal()->Ident()->getText() + "_" + std::to_string(vi->scopeID));
			ctx->code.addline(tacline);
		}
		
	} else if(ctx->block()) {		// stmt : block
		ctx->code.codecat(ctx->block()->code);
	} else if(ctx->getStart()->getText() == std::string("if")) {		// stmt : 'if' '(' cond ')' stmt ('else' stmt)?
		assert(ctx->stmt().size() == 1 || ctx->stmt().size() == 2);
		if(ctx->stmt().size() == 1)				// stmt : 'if' '(' cond ')' stmt
		{
			// cond.code
			ctx->code.codecat(ctx->cond()->code);
			TACline label;
			label.op = TACOP_LABEL;
			label.arg1.init(BTYPE_ANY, 0);
			label.arg2.init(BTYPE_ANY, 1);
			label.result.init(BTYPE_ANY, 1);
			label.arg1.copyName(ctx->cond()->t);
			// label(cond.t)
			ctx->code.addline(label);
			// stmt_1.code
			ctx->code.codecat(ctx->stmt().at(0)->code);
			// ctx.next
			label.arg1.copyName(ctx->next);
			ctx->code.addline(label);
		} else if(ctx->stmt().size() == 2) {	// stmt : 'if' '(' cond ')' stmt 'else' stmt
			ctx->code = threeAdressCode();
			// cond.code
			ctx->code.codecat(ctx->cond()->code);
			TACline label;
			label.op = TACOP_LABEL;
			label.arg1.init(BTYPE_ANY, 0);
			label.arg2.init(BTYPE_ANY, 1);
			label.result.init(BTYPE_ANY, 1);
			label.arg1.copyName(ctx->cond()->t);
			// label(cond.t)
			ctx->code.addline(label);
			// stmt_1.code
			ctx->code.codecat(ctx->stmt().at(0)->code);
			TACline jump;
			jump.op = TACOP_J;
			jump.arg1.init(BTYPE_ANY, 0);
			jump.arg2.init(BTYPE_ANY, 1);
			jump.result.init(BTYPE_ANY, 1);
			jump.arg1.copyName(ctx->next);
			// gen('goto' stmt.next)
			ctx->code.addline(jump);
			label.arg1.copyName(ctx->cond()->f);
			// label(cond.f)
			ctx->code.addline(label);
			// stmt_2.code
			ctx->code.codecat(ctx->stmt().at(1)->code);
			// ctx.next
			label.arg1.copyName(ctx->next);
			ctx->code.addline(label);
		}
	} else if(ctx->getStart()->getText() == std::string("while")) {		// stmt : 'while' '(' cond ')' stmt
		ctx->stmt().at(0)->in_loop = 1;
		ctx->stmt().at(0)->ret_type = ctx->ret_type;

		TACline label, jump;
		label.op = TACOP_LABEL;
		label.arg1.init(BTYPE_ANY, 0);
		label.arg2.init(BTYPE_ANY, 1);
		label.result.init(BTYPE_ANY, 1);
		label.arg1.copyName(ctx->stmt().at(0)->next);
		// label(S1.next)
		ctx->code.addline(label);
		// cond.code
		ctx->code.codecat(ctx->cond()->code);
		label.arg1.copyName(ctx->cond()->t);
		// label(cond.true)
		ctx->code.addline(label);
		// stmt.code
		ctx->code.codecat(ctx->stmt().at(0)->code);
		jump.op = TACOP_J;
		jump.arg1.init(BTYPE_ANY, 0);
		jump.arg2.init(BTYPE_ANY, 1);
		jump.result.init(BTYPE_ANY, 1);
		jump.arg1.copyName(ctx->stmt().at(0)->next);
		// goto S1.next
		ctx->code.addline(jump);
		// ctx.next
		label.arg1.copyName(ctx->next);
		ctx->code.addline(label);
	} else if(ctx->getStart()->getText() == std::string("break")) {		// stmt : 'break' ';'
		TACline jump;
		jump.op = TACOP_J;
		jump.arg1.init(BTYPE_ANY, 0);
		jump.arg2.init(BTYPE_ANY, 1);
		jump.result.init(BTYPE_ANY, 1);
		jump.arg1.copyName(ctx->breaknext);
		ctx->code.addline(jump);
	} else if(ctx->getStart()->getText() == std::string("continue")) {		// stmt : 'continue' ';'
		TACline jump;
		jump.op = TACOP_J;
		jump.arg1.init(BTYPE_ANY, 0);
		jump.arg2.init(BTYPE_ANY, 1);
		jump.result.init(BTYPE_ANY, 1);
		jump.arg1.copyName(ctx->contnext);
		ctx->code.addline(jump);
	} else if(ctx->getStart()->getText() == std::string("return")) {		// stmt : 'return' exp? ';'
		if(ctx->exp())
			ctx->code.codecat(ctx->exp()->code);
		TACline ret;
		ret.op = TACOP_RETURN;
		if(ctx->exp())
		{
			ret.arg1.init(ctx->exp()->btype, 0);
			ret.arg1.copyName(ctx->exp()->temp);
		} else ret.arg1.init(BTYPE_ANY, 1);
		ret.arg2.init(BTYPE_ANY, 1);
		ret.result.init(BTYPE_ANY, 1);
		ctx->code.addline(ret);
	} else if(ctx->exp()) {		// stmt : (exp)?
		ctx->code.codecat(ctx->exp()->code);
	}
	
}

void SemanticAnalysis::enterExp(CACTParser::ExpContext * ctx)
{ 
	if(ctx->addExp())		// exp : addExp
	{
		ctx->temp = mynewtemp();
		ctx->addExp()->temp = ctx->temp;
	} else {		// exp : BoolConst
		ctx->temp = mynewtemp();
	}
}
void SemanticAnalysis::exitExp(CACTParser::ExpContext * ctx)
{ 
// printf("Exiting Exp\n");
	int line = ctx->getStart()->getLine();
	if(ctx->addExp())		// exp : addExp
	{
		ctx->btype = ctx->addExp()->btype;
		ctx->array_len = ctx->addExp()->array_len;
		ctx->code.codecat(ctx->addExp()->code);
	} else {		// exp : BoolConst
		ctx->btype = BTYPE_BOOL;
		ctx->array_len = 0;
		
		TACline tacline;
		int res = ctx->boolConst()->getText() == "true" ? 1 : 0;
		tacline.op = TACOP_ASSIGN;
		tacline.arg1.init(BTYPE_INT, 1);
		tacline.arg2.init(BTYPE_ANY, 1);
		tacline.arg1.copyImm(&res);
		tacline.result.init(BTYPE_ANY, 0);
		tacline.result.copyName(ctx->temp);
		ctx->code.addline(tacline);
	}
}

void SemanticAnalysis::enterCond(CACTParser::CondContext * ctx)
{ 
	// printf("Entering Cond\n");
	ctx->lOrExp()->temp = ctx->temp;
	ctx->lOrExp()->t = ctx->t;
	ctx->lOrExp()->f = ctx->f;
}
void SemanticAnalysis::exitCond(CACTParser::CondContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->lOrExp()->btype != BTYPE_BOOL)
	{
		std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "cond is not bool" << std::endl;
		semanticErrors++;
	}
	ctx->code.codecat(ctx->lOrExp()->code);
// printf("Exiting Cond\n");

}

void SemanticAnalysis::enterLVal(CACTParser::LValContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	VarInfo *lVal_info;
	lVal_info = sym_table.lookup_all(ctx->Ident()->getText());
	if(lVal_info == nullptr)
	{
		std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
					<< ":" << "variable used without definition" << std::endl;
		semanticErrors++;
		return;
	}
	ctx->btype = lVal_info->btype;
	ctx->array_len = lVal_info->array_len;
	if(ctx->exp())		// lVal : Ident ('[' exp ']')
	{
		if(lVal_info->array_len == 0)
		{
			std::cout << line << ":" << ctx->exp()->getStart()->getCharPositionInLine() - 1 \
					<< ":" << "non-array type variable cannot be indexed" << std::endl;
			semanticErrors++;
		}
		ctx->array_len = 0;
	} else {			// lVal : Ident

	}
}
void SemanticAnalysis::exitLVal(CACTParser::LValContext * ctx)
{ 
// printf("Exiting LVal\n");
	int line = ctx->getStart()->getLine();
	if(ctx->exp())		// lVal : Ident ('[' exp ']')
	{
		if(ctx->exp()->btype != BTYPE_INT)
		{
			std::cout << line << ":" << ctx->exp()->getStart()->getCharPositionInLine() \
						<< ":" << "array index must be of type \"int\"" << std::endl;
			semanticErrors++;
		}

		ctx->code.codecat(ctx->exp()->code);
		int width;
		if(ctx->btype == BTYPE_DOUBLE)
			width = 8;
		else width = 4;
		ctx->temp = mynewtemp();
		TACline tacline;
		std::string temp = mynewtemp();
		tacline.op = TACOP_ASSIGN;
		tacline.result.init(BTYPE_INT, 0);
		tacline.arg1.init(BTYPE_INT, 1);
		tacline.arg2.init(BTYPE_ANY, 1);
		tacline.result.copyName(temp);
		tacline.arg1.copyImm(&width);
		ctx->code.addline(tacline);

		tacline.op = TACOP_MUL;
		tacline.arg1.init(ctx->exp()->btype, 0);
		tacline.arg1.copyName(ctx->exp()->temp);
		tacline.arg2.init(BTYPE_INT, 0);
		tacline.arg2.copyName(temp);
		tacline.result.init(BTYPE_INT, 0);
		tacline.result.copyName(ctx->temp);
		ctx->code.addline(tacline);
	} else {		// lVal : Ident
		// TACline line;
		// line.op = TACOP_ASSIGN;
		// line.result.init(BTYPE_ANY, 0);
		// line.result.copyName(ctx->temp);
		// line.arg1.init(ctx->btype, 0);
		// line.arg2.init(BTYPE_ANY, 1);
		// line.arg1.copyName(ctx->Ident()->getText());
		// ctx->code.addline(line);
	}
}

void SemanticAnalysis::enterPrimaryExp(CACTParser::PrimaryExpContext * ctx)
{ 
	// printf("Entering PrimaryExp\n");
	if(ctx->number())		// primaryExp : number
	{
		
	}
	else if(ctx->exp())		// primaryExp : '(' exp ')'
	{
		ctx->exp()->temp = ctx->temp;
	}
	else if(ctx->lVal())	// primaryExp : lVal
	{

	}
}

static void getnumber(void* number, CACTParser::NumberContext *ctx)
{
	assert(ctx->btype == BTYPE_INT || ctx->btype == BTYPE_FLOAT || ctx->btype == BTYPE_DOUBLE);
	switch(ctx->btype)
	{
		case BTYPE_INT:
			*((int*)number) = atoi(ctx->getText().c_str());
			break;
		case BTYPE_FLOAT:
			*((float*)number) = (float)sci2db(ctx->getText().c_str());
			break;
		case BTYPE_DOUBLE:
			*((double*)number) = sci2db(ctx->getText().c_str());
			break;
		default:
			break;
	}
}

void SemanticAnalysis::exitPrimaryExp(CACTParser::PrimaryExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->number())		// primaryExp : number
	{
		ctx->btype = ctx->number()->btype;
		ctx->array_len = 0;

		TACline line;
		line.op = TACOP_ASSIGN;
		line.result.init(ctx->btype, 0);
		line.result.copyName(ctx->temp);
		line.arg1.init(ctx->number()->btype, 1);
		line.arg2.init(BTYPE_ANY, 1);
		void *number = malloc(8);
		getnumber(number, ctx->number());
		line.arg1.copyImm(number);
		ctx->code.addline(line);
		free(number);
	}
	else if(ctx->exp())		// primaryExp : '(' exp ')'
	{
		ctx->btype = ctx->exp()->btype;
		ctx->array_len = ctx->exp()->array_len;

		ctx->code.codecat(ctx->exp()->code);

		TACline line;
		line.op = TACOP_ASSIGN;
		line.result.init(ctx->btype, 0);
		line.result.copyName(ctx->temp);
		line.arg1.init(ctx->exp()->btype, 0);
		line.arg1.copyName(ctx->exp()->temp);
		line.arg2.init(BTYPE_ANY, 1);
		ctx->code.addline(line);
	}
	else if(ctx->lVal())	// primaryExp : lVal
	{
		ctx->btype = ctx->lVal()->btype;
		ctx->array_len = ctx->lVal()->array_len;

		ctx->code.codecat(ctx->lVal()->code);

		if(ctx->lVal()->exp())
		{
			VarInfo* vi = sym_table.lookup_all(ctx->lVal()->Ident()->getText());
			TACline line;
			line.op = TACOP_ROFF;
			line.arg1.init(ctx->lVal()->btype, 0);
			line.arg1.copyName(ctx->lVal()->Ident()->getText() + "_" + std::to_string(vi->scopeID));
			line.arg2.init(BTYPE_INT, 0);
			line.arg2.copyName(ctx->lVal()->temp);
			line.result.init(ctx->btype, 0);
			line.result.copyName(ctx->temp);
			ctx->code.addline(line);
		} else {
			if(ctx->lVal()->array_len > 0)
			{
				array_vector.push_back(std::pair<std::string, int>(ctx->lVal()->Ident()->getText(), ctx->lVal()->array_len));
				array_vector_str.push_back("0");
			}
			VarInfo* vi = sym_table.lookup_all(ctx->lVal()->Ident()->getText());
			TACline line;
			line.op = TACOP_ASSIGN;
			line.arg1.init(ctx->lVal()->btype, 0);
			line.arg2.init(BTYPE_ANY, 1);
			line.result.init(ctx->btype, 0);
			line.arg1.copyName(ctx->lVal()->Ident()->getText() + "_" + std::to_string(vi->scopeID));
			line.result.copyName(ctx->temp);
			ctx->code.addline(line);
		}
		
	}
	// printf("Exiting PrimaryExp\n");

}

void SemanticAnalysis::enterNumber(CACTParser::NumberContext * ctx)
{ 
// printf("Entering Number\n");
}
void SemanticAnalysis::exitNumber(CACTParser::NumberContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->IntConst())//IntConst
	{
		ctx->btype = BTYPE_INT;
	}
	else if(ctx->SmallConst())
	{
		ctx->btype = BTYPE_DOUBLE;
	}
	else if(ctx->ScienceConst())
	{
		ctx->btype = BTYPE_DOUBLE;
	}
	else if(ctx->FloatConst())
	{
		ctx->btype = BTYPE_FLOAT;
	}
	// printf("Exiting Number\n");

	/** TAC */
	if(ctx->IntConst()){
		ctx->val = (char *)malloc(sizeof(int));
		int a = atoi(ctx->IntConst()->getText().c_str());
		memcpy(ctx->val, &a, sizeof(int));
		//printf("%d\n", a);
	}
	else if(ctx->SmallConst()){
		ctx->val = (char *)malloc(sizeof(double));
		double d = sci2db(ctx->getText().c_str());
		//sscanf(ctx->SmallConst()->getText().c_str(), "%lf", &d);
		memcpy(ctx->val, &d, sizeof(double));
		//printf("%lf\n", d);
	}
	else if(ctx->ScienceConst()){
		ctx->val = (char *)malloc(sizeof(double));
		double d = sci2db(ctx->getText().c_str());
		memcpy(ctx->val, &d, sizeof(double));
		//printf("%lf\n", d);
	}
	else if(ctx->FloatConst()){
		ctx->val = (char *)malloc(sizeof(float));
		float f = (float)sci2db(ctx->getText().c_str());
		memcpy(ctx->val, &f, sizeof(double));
		//printf("%f\n", f);
	}
}

void SemanticAnalysis::enterUnaryExp(CACTParser::UnaryExpContext * ctx)
{
	if(ctx->primaryExp() != NULL){		// unaryExp : primaryExp
		ctx->primaryExp()->temp = ctx->temp;
	}
	else if(ctx->Ident() != NULL){		// unaryExp : Ident '(' funcRParams? ')'
		if(ctx->funcRParams())
			ctx->funcRParams()->temp = mynewtemp();
	}
	else{		// unaryExp : unaryOp unaryExp
		ctx->unaryExp()->temp = mynewtemp();
	}
}
void SemanticAnalysis::exitUnaryExp(CACTParser::UnaryExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->primaryExp() != NULL){		// unaryExp : primaryExp
		ctx->btype = ctx->primaryExp()->btype;
		ctx->array_len =ctx->primaryExp()->array_len;

		ctx->code.codecat(ctx->primaryExp()->code);
	}
	else if(ctx->Ident() != NULL){		// unaryExp : Ident '(' funcRParams? ')'
		FuncInfo * func;
		ctx->array_len = 0;
		if((func = sym_table.lookup_func(ctx->Ident()->getText())) == NULL){
			semanticErrors++;
			std::cout << "Undefined function\n";
			return;
		}
		ctx->btype = (btype)func->ret_type;
		int nRPara, nFPara = func->cnt_para;
		if(ctx->funcRParams() == NULL) nRPara = 0;
		else nRPara =ctx->funcRParams()->exp().size();
		if(nRPara != nFPara){
			semanticErrors++;
			std::cout << "Unmatched parameter number\n";
			return;
		}
		for(int i = 0; i < func->cnt_para; i++){
			if(ctx->funcRParams()->exp()[i]->btype != func->para.find(i)->second.btype){
				semanticErrors++;
				std::cout << "Unmatched parameter type\n";
				return;
			}
		}

		int numparam;
		if(ctx->funcRParams())
		{
			ctx->code.codecat(ctx->funcRParams()->code);
			numparam = ctx->funcRParams()->exp().size();
		} else numparam = 0;
		TACline line;
		line.op = TACOP_PARAM;
		line.arg2.init(BTYPE_INT, 1);
		line.result.init(BTYPE_INT, 1);
		for(int i = 0; i < numparam; i++)
		{
			line.arg1.init(ctx->funcRParams()->exp()[i]->btype, 0);
			line.arg1.copyName(ctx->funcRParams()->exp()[i]->temp);
			ctx->code.addline(line);
		}
		line.op = TACOP_CALL;
		line.arg1.init(BTYPE_ANY, 0);
		line.arg1.copyName(ctx->Ident()->getText());
		line.arg2.init(BTYPE_INT, 1);
		line.arg2.copyImm(&numparam);
		line.result.init(ctx->btype, 0);
		line.result.copyName(ctx->temp);
		ctx->code.addline(line);
	}
	else{		// unaryExp : unaryOp unaryExp
		if(ctx->unaryExp()->array_len != 0){
			semanticErrors++;
			std::cout << "array calculation error\n";
		}
		if(ctx->unaryOp()->getStart()->getText()==std::string("!") && ctx->unaryExp()->btype != BTYPE_BOOL){
			semanticErrors++;
			std::cout << "'!' used to unbool type\n";
		}
		else{
			ctx->btype = ctx->unaryExp()->btype;
			ctx->array_len = 0;
		}

		ctx->code.codecat(ctx->unaryExp()->code);
		TACline line;
		if(ctx->unaryOp()->getText() == "+")
			line.op = TACOP_ASSIGN;
		else if(ctx->unaryOp()->getText() == "-")
			line.op = TACOP_MINUS;
		else if(ctx->unaryOp()->getText() == "!")
			line.op = TACOP_NOT;
		line.result.init(BTYPE_ANY, 0);
		line.result.copyName(ctx->temp);
		line.arg1.init(BTYPE_ANY, 0);
		line.arg2.init(BTYPE_ANY, 1);
		line.arg1.copyName(ctx->unaryExp()->temp);
		ctx->code.addline(line);
	}
// printf("Exiting UnaryExp\n");

}

void SemanticAnalysis::enterUnaryOp(CACTParser::UnaryOpContext * ctx)
{ 
// printf("Entering UnaryOp\n");
}
void SemanticAnalysis::exitUnaryOp(CACTParser::UnaryOpContext * ctx)
{ 
// printf("Exiting UnaryOp\n");

}

void SemanticAnalysis::enterFuncRParams(CACTParser::FuncRParamsContext * ctx)
{ 
// printf("Entering FuncRParams\n");
}
void SemanticAnalysis::exitFuncRParams(CACTParser::FuncRParamsContext * ctx)
{ 
// printf("Exiting FuncRParams\n");
	int line = ctx->getStart()->getLine();
	for(int i = 0; i < ctx->exp().size(); i++)
	{
		ctx->code.codecat(ctx->exp()[i]->code);
	}

}

void SemanticAnalysis::enterMulExp(CACTParser::MulExpContext * ctx)
{ 
// printf("Entering MulExp\n");
	if(ctx->mulExp())	// mulExp ('*' | '/' | '%') unaryExp
	{
		ctx->mulExp()->temp = mynewtemp();
		ctx->unaryExp()->temp = mynewtemp();
	}
	else if(ctx->unaryExp())	// unaryExp
	{
		ctx->unaryExp()->temp = ctx->temp;
	}
}
void SemanticAnalysis::exitMulExp(CACTParser::MulExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->mulExp())		// mulExp ('*' | '/' | '%') unaryExp
	{
		if(ctx->mulExp()->btype != ctx->unaryExp()->btype || ctx->mulExp()->array_len != ctx->unaryExp()->array_len){
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "calculation between different types(btype or array_len)" << std::endl;
			semanticErrors++;
		}
		else{		
			ctx->btype = ctx->mulExp()->btype;
			ctx->array_len = ctx->mulExp()->array_len;
		}

		ctx->code.codecat(ctx->mulExp()->code);
		ctx->code.codecat(ctx->unaryExp()->code);
		TACline line;
		line.result.init(ctx->btype, 0);
		line.result.copyName(ctx->temp);
		line.arg1.init(ctx->mulExp()->btype, 0);
		line.arg1.copyName(ctx->mulExp()->temp);
		line.arg2.init(ctx->unaryExp()->btype, 0);
		line.arg2.copyName(ctx->unaryExp()->temp);

		if(ctx->getText().find("*") != ctx->getText().npos)		// mulExp '*' unaryExp
		{
			line.op = TACOP_MUL;
		} else if(ctx->getText().find("/") != ctx->getText().npos) {	// mulExp '/' unaryExp
			line.op = TACOP_DIV;
		} else if(ctx->getText().find("%") != ctx->getText().npos) {	// mulExp '%' unaryExp
			line.op = TACOP_MOD;
		}

		ctx->code.addline(line);
	}
	else if(ctx->unaryExp())		// unaryExp
	{
		ctx->btype = ctx->unaryExp()->btype;
		ctx->array_len = ctx->unaryExp()->array_len;

		ctx->code.codecat(ctx->unaryExp()->code);
	}
// printf("Exiting MulExp\n");

}

void SemanticAnalysis::enterAddExp(CACTParser::AddExpContext * ctx)
{ 
// printf("Entering AddExp\n");
	if(ctx->addExp())		// addExp ('+' | '-') mulExp
	{
		ctx->addExp()->temp = mynewtemp();
		ctx->mulExp()->temp = mynewtemp();

	}
	else if(ctx->mulExp())		// mulExp
	{
		ctx->mulExp()->temp = ctx->temp;
	} 
}
void SemanticAnalysis::exitAddExp(CACTParser::AddExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->addExp())		// addExp ('+' | '-') mulExp
	{
		if(ctx->addExp()->btype != ctx->mulExp()->btype || ctx->addExp()->array_len != ctx->mulExp()->array_len)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
								<< ":" << "calculation between different types(btype or array_len)" << std::endl;
			semanticErrors++;
		}
		else{
			ctx->btype = ctx->addExp()->btype;
			ctx->array_len = ctx->addExp()->array_len;
		}
		
		ctx->code.codecat(ctx->addExp()->code);
		ctx->code.codecat(ctx->mulExp()->code);
		TACline line;
		if(ctx->getText().find("+") != ctx->getText().npos)
			line.op = TACOP_ADD;
		else if(ctx->getText().find("-") != ctx->getText().npos)
			line.op = TACOP_SUB;
		else assert(0);
		line.result.init(ctx->btype, 0);
		line.result.copyName(ctx->temp);
		line.arg1.init(ctx->addExp()->btype, 0);
		line.arg1.copyName(ctx->addExp()->temp);
		line.arg2.init(ctx->mulExp()->btype, 0);
		line.arg2.copyName(ctx->mulExp()->temp);
		ctx->code.addline(line);
	}
	else if(ctx->mulExp())		// mulExp
	{
		ctx->btype = ctx->mulExp()->btype;
		ctx->array_len = ctx->mulExp()->array_len;

		ctx->code.codecat(ctx->mulExp()->code);
	} 
	// printf("Exiting AddExp\n");
}

void SemanticAnalysis::enterRelExp(CACTParser::RelExpContext * ctx)
{ 
// printf("Entering RelExp\n");
	if(ctx->relExp())		// relExp : relExp ('<' | '>' | '>=' | '<=') addExp
	{
		ctx->relExp()->temp = mynewtemp();
		ctx->addExp()->temp = mynewtemp();
	}
	else if(ctx->boolConst())		// relExp : boolConst
	{
		ctx->boolConst()->temp = ctx->temp;
	}
	else if(ctx->addExp())		// relExp : addExp
	{
		ctx->addExp()->temp = ctx->temp;
	}
}
void SemanticAnalysis::exitRelExp(CACTParser::RelExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->relExp())		// relExp : relExp ('<' | '>' | '>=' | '<=') addExp
	{
		if(ctx->relExp()->btype != ctx->addExp()->btype || ctx->addExp()->array_len != 0)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "comparation between different types(btype or array len)" << std::endl;
			semanticErrors++;
		}
		else{
			ctx->btype = BTYPE_BOOL;
		}
		
		ctx->code.codecat(ctx->relExp()->code);
		ctx->code.codecat(ctx->addExp()->code);
		TACline jump;
		if(ctx->getText().find("<") != ctx->getText().npos)
			jump.op = TACOP_J_LT;
		else if(ctx->getText().find(">") != ctx->getText().npos)
			jump.op = TACOP_J_GT;
		else if(ctx->getText().find("<=") != ctx->getText().npos)
			jump.op = TACOP_J_LE;
		else if(ctx->getText().find(">=") != ctx->getText().npos)
			jump.op = TACOP_J_GE;
		jump.arg1.init(BTYPE_BOOL, 0);
		jump.arg1.copyName(ctx->relExp()->temp);
		jump.arg2.init(BTYPE_BOOL, 0);
		jump.arg2.copyName(ctx->addExp()->temp);
		jump.result.init(BTYPE_ANY, 0);
		jump.result.copyName(ctx->t);
		ctx->code.addline(jump);
		
		jump.op = TACOP_J;
		jump.arg1.init(BTYPE_BOOL, 0);
		jump.arg1.copyName(ctx->f);
		jump.arg2.init(BTYPE_ANY, 1);
		jump.result.init(BTYPE_ANY, 1);
		ctx->code.addline(jump);
	}
	else if(ctx->boolConst())		// relExp : boolConst
	{
		ctx->btype = BTYPE_BOOL;

		TACline tacline;
		int res = ctx->getText() == "true" ? 1 : 0;
		tacline.op = TACOP_ASSIGN;
		tacline.arg1.init(BTYPE_INT, 1);
		tacline.arg2.init(BTYPE_ANY, 1);
		tacline.arg1.copyImm(&res);
		tacline.result.init(BTYPE_ANY, 0);
		tacline.result.copyName(ctx->temp);
		ctx->code.addline(tacline);
	}
	else if(ctx->addExp())		// relExp : addExp
	{
		if(ctx->addExp()->array_len != 0)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "lor be an array" << std::endl;
			semanticErrors++;
		}
		ctx->btype = ctx->addExp()->btype;
	
		ctx->code.codecat(ctx->addExp()->code);
	}

// printf("Exiting RelExp\n");

}

void SemanticAnalysis::enterEqExp(CACTParser::EqExpContext * ctx)
{ 
	// printf("Entering EqExp\n");
	if(ctx->eqExp())			// eqExp : eqExp ('==' | '!=') relExp
	{
		ctx->eqExp()->temp = mynewtemp();
		ctx->relExp()->temp = mynewtemp();
		ctx->relExp()->f = ctx->f;
		ctx->relExp()->t = ctx->t;
	}
	else if(ctx->relExp())		// eqExp : relExp
	{
		ctx->relExp()->f = ctx->f;
		ctx->relExp()->t = ctx->t;
		ctx->relExp()->temp = ctx->temp;
	}
}
void SemanticAnalysis::exitEqExp(CACTParser::EqExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->eqExp())			// eqExp : eqExp ('==' | '!=') relExp
	{
		if(ctx->eqExp()->btype != ctx->relExp()->btype)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "comparation between different types" << std::endl;
			semanticErrors++;
		}
		else{
			ctx->btype = BTYPE_BOOL;
		}
			
		ctx->code.codecat(ctx->eqExp()->code);
		ctx->code.codecat(ctx->relExp()->code);
		TACline jump;
		if(ctx->getText().find("==") != ctx->getText().npos)
			jump.op = TACOP_J_EQ;
		else if(ctx->getText().find("!=") != ctx->getText().npos)
			jump.op = TACOP_J_NE;
		jump.arg1.init(BTYPE_BOOL, 0);
		jump.arg1.copyName(ctx->eqExp()->temp);
		jump.arg2.init(BTYPE_BOOL, 0);
		jump.arg2.copyName(ctx->relExp()->temp);
		jump.result.init(BTYPE_ANY, 0);
		jump.result.copyName(ctx->t);
		ctx->code.addline(jump);
		
		jump.op = TACOP_J;
		jump.arg1.init(BTYPE_BOOL, 0);
		jump.arg1.copyName(ctx->f);
		jump.arg2.init(BTYPE_ANY, 1);
		jump.result.init(BTYPE_ANY, 1);
		ctx->code.addline(jump);
	}
	else if(ctx->relExp())		// eqExp : relExp
	{
		ctx->btype = ctx->relExp()->btype;

		ctx->code.codecat(ctx->relExp()->code);
	}
// printf("Exiting EqExp\n");

}

void SemanticAnalysis::enterLAndExp(CACTParser::LAndExpContext * ctx)
{ 
	// printf("Entering LAndExp\n");
	if(ctx->lAndExp())		// lAndExp : lAndExp '&&' eqExp
	{
		ctx->lAndExp()->t = mynewlabel();
		ctx->lAndExp()->f = ctx->f;
		ctx->eqExp()->t = ctx->t;
		ctx->eqExp()->f = ctx->f;
	}
	else if(ctx->eqExp())	// lAndExp : eqExp
	{
		ctx->eqExp()->t = ctx->t;
		ctx->eqExp()->f = ctx->f;
		ctx->eqExp()->temp = ctx->temp;
	}
}
void SemanticAnalysis::exitLAndExp(CACTParser::LAndExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->lAndExp())		// lAndExp : lAndExp '&&' eqExp
	{
		if(ctx->lAndExp()->btype != BTYPE_BOOL || ctx->eqExp()->btype != BTYPE_BOOL)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "logic calculation without an unbool parameter" << std::endl;
			semanticErrors++;
		}
		else{
			ctx->btype = BTYPE_BOOL;
		} 

		ctx->code.codecat(ctx->lAndExp()->code);
		TACline label;
		label.arg1.init(BTYPE_ANY, 0);
		label.arg2.init(BTYPE_ANY, 1);
		label.result.init(BTYPE_ANY, 1);
		label.arg1.copyName(ctx->lAndExp()->t);
		ctx->code.addline(label);
		ctx->code.codecat(ctx->eqExp()->code);
	}
	else if(ctx->eqExp())		// lAndExp : eqExp
	{
		ctx->btype = ctx->eqExp()->btype;

		ctx->code.codecat(ctx->eqExp()->code);
	}
// printf("Exiting LAndExp\n");

}

void SemanticAnalysis::enterLOrExp(CACTParser::LOrExpContext * ctx)
{ 
	// printf("Entering LOrExp\n");
	if(ctx->lOrExp())		// lOrExp: lOrExp '||' lAndExp
	{
		ctx->lOrExp()->t = ctx->t;
		ctx->lOrExp()->f = mynewlabel();
		ctx->lAndExp()->t = ctx->t;
		ctx->lAndExp()->f = ctx->f;
		
	}
	else if(ctx->lAndExp())		// lOrExp: lAndExp
	{
		ctx->lAndExp()->t = ctx->t;
		ctx->lAndExp()->f = ctx->f;
		ctx->lAndExp()->temp = ctx->temp;
	}
}
void SemanticAnalysis::exitLOrExp(CACTParser::LOrExpContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	if(ctx->lOrExp())		// lOrExp: lOrExp '||' lAndExp
	{
		if(ctx->lAndExp()->btype != BTYPE_BOOL || ctx->lOrExp()->btype != BTYPE_BOOL)
		{
			std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
							<< ":" << "logic calculation without an unbool parameter" << std::endl;
			semanticErrors++;
		}
		else{
			ctx->btype = BTYPE_BOOL;
		}

		ctx->code.codecat(ctx->lOrExp()->code);
		TACline label;
		label.arg1.init(BTYPE_ANY, 0);
		label.arg2.init(BTYPE_ANY, 1);
		label.result.init(BTYPE_ANY, 1);
		label.arg1.copyName(ctx->lOrExp()->f);
		ctx->code.addline(label);
		ctx->code.codecat(ctx->lAndExp()->code);
	}
	else if(ctx->lAndExp())		// lOrExp: lAndExp
	{
		ctx->btype = ctx->lAndExp()->btype;

		ctx->code.codecat(ctx->lAndExp()->code);
	}
// printf("Exiting LOrExp\n");

}

void SemanticAnalysis::enterConstExpNumber(CACTParser::ConstExpNumberContext * ctx)
{ 
	// printf("Entering ConstExpNumber\n");
}
void SemanticAnalysis::exitConstExpNumber(CACTParser::ConstExpNumberContext * ctx)
{
	int line = ctx->getStart()->getLine();
	if(ctx->number())
	{
		ctx->btype = ctx->number()->btype;
		ctx->val = ctx->number()->val;
	}
    // ctx->basic_or_array_and_type = ctx->number()->basic_or_array_and_type;
    // printf("Exiting ConstExpNumber\n");
}

void SemanticAnalysis::enterConstExpBoolConst(CACTParser::ConstExpBoolConstContext * ctx)
{ 
	// printf("Entering ConstExpBoolConst\n");

}
void SemanticAnalysis::exitConstExpBoolConst(CACTParser::ConstExpBoolConstContext * ctx)
{ 
	int line = ctx->getStart()->getLine();
	ctx->btype = BTYPE_BOOL;
	ctx->val = (char *)malloc(sizeof(int));
	if(ctx->getStart()->getText() == "true"){
		int b = 1;
		//printf("true\n");
		memcpy(ctx->val, &b, sizeof(int));
	}
	else{
		int b = 0;
		//printf("flase\n");
		memcpy(ctx->val, &b, sizeof(int));
	}
	//printf("Exiting ConstExpBoolConst\n");
}


void SemanticAnalysis::enterEveryRule(antlr4::ParserRuleContext * ctx)
{ 
	//printf("Entering a rule\n");
}
void SemanticAnalysis::exitEveryRule(antlr4::ParserRuleContext * ctx)
{ 

}

void SemanticAnalysis::visitTerminal(antlr4::tree::TerminalNode * node)
{ 
	//    printf("visiting a terminal\n");
}

void SemanticAnalysis::visitErrorNode(antlr4::tree::ErrorNode * node)
{ 

}
