#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#pragma

#include "../grammar/CACTBaseListener.h"
#include "./symbolTable.h"


/* static std::string str_dup(std::string str, int times)
{
  std::string ret;
  while(times--)
    ret += str;
  return ret;
} */

class SemanticAnalysis : public CACTBaseListener
{
private:
    /* data */
public:
    SemanticAnalysis(/* args */){}
    ~SemanticAnalysis(){}


	virtual void enterCompUnit(CACTParser::CompUnitContext * ctx) override { 
	// printf("Entering CompUnit\n");
	}
	virtual void exitCompUnit(CACTParser::CompUnitContext * ctx) override;

	virtual void enterDecl(CACTParser::DeclContext * ctx) override { 
		
	}
	virtual void exitDecl(CACTParser::DeclContext * ctx) override { 
		// printf("Exiting Decl\n");
		
	}
		

	virtual void enterConstDecl(CACTParser::ConstDeclContext * ctx) override;
	virtual void exitConstDecl(CACTParser::ConstDeclContext * ctx) override;

	virtual void enterBType(CACTParser::BTypeContext * ctx) override { 
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
	virtual void exitBType(CACTParser::BTypeContext * ctx) override { 
		// printf("Exiting BType\n");

	}

	virtual void enterConstDef(CACTParser::ConstDefContext * ctx) override { 
		// constDef : : Ident ('[' IntConst ']')*'=' constInitVal

	}
	virtual void exitConstDef(CACTParser::ConstDefContext * ctx) override { 
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
	}

	virtual void enterConstInitVal(CACTParser::ConstInitValContext * ctx) override { 
		// printf("Entering ConstInitVal\n");
	}
	virtual void exitConstInitVal(CACTParser::ConstInitValContext * ctx) override { 
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
		
	}

	virtual void enterVarDecl(CACTParser::VarDeclContext * ctx) override;

	virtual void exitVarDecl(CACTParser::VarDeclContext * ctx) override;

	virtual void enterVarDef(CACTParser::VarDefContext * ctx) override { 
		
	}
	virtual void exitVarDef(CACTParser::VarDefContext * ctx) override { 
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
	}

	virtual void enterFuncDef(CACTParser::FuncDefContext * ctx) override { 
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
	virtual void exitFuncDef(CACTParser::FuncDefContext * ctx) override {
		if(sym_table.curr_scope == NULL)return;
		sym_table.scope_ret();
		// printf("Exiting FuncDef\n");

	}

	virtual void enterFuncType(CACTParser::FuncTypeContext * ctx) override { 
		// printf("Entering FuncType\n");
	}

	virtual void exitFuncType(CACTParser::FuncTypeContext * ctx) override { 
		// printf("Exiting FuncType\n");

	}

	virtual void enterFuncFParams(CACTParser::FuncFParamsContext * ctx) override { 
		// printf("Entering FuncFParams\n");
	}
	virtual void exitFuncFParams(CACTParser::FuncFParamsContext * ctx) override { 
		if(sym_table.curr_scope == NULL)return;
		for(int i = 0; i < ctx->funcFParam().size(); i++){
			sym_table.addPara(ctx->funcFParam()[i]->Ident()->getText(), ctx->funcFParam()[i]->btype, 0);
		}
		// std::cout << sym_table.curr_scope->curr_func->cnt_para << std::endl;
		// std::cout << sym_table.curr_scope->curr_func->name << std::endl;
		// printf("Exiting FuncFParams\n");

	}

	virtual void enterFuncFParam(CACTParser::FuncFParamContext * ctx) override { 
		if(sym_table.curr_scope == NULL)return;
		if(ctx->bType()->getText() == "int"){
			ctx->btype = FUNC_INT;
		}
		else if(ctx->bType()->getText() == "float"){
			ctx->btype = FUNC_FLOAT;
		}
		else if(ctx->bType()->getText() == "double"){
			ctx->btype = FUNC_DOUBLE;
		}
		else if(ctx->bType()->getText() == "bool"){
			ctx->btype = FUNC_BOOL;
		}
		else if(ctx->bType()->getText() == "void"){
			ctx->btype = FUNC_VOID;
		}
		// printf("Entering FuncFParam\n");
	}
	virtual void exitFuncFParam(CACTParser::FuncFParamContext * ctx) override { 
		// printf("Exiting FuncFParam\n");

	}

	virtual void enterBlock(CACTParser::BlockContext * ctx) override { 
		int i;
		for(i = 0; i < ctx->blockItem().size(); i++)
		{
			ctx->blockItem()[i]->ret_type = ctx->ret_type;
			ctx->blockItem()[i]->in_loop = ctx->in_loop;
		}
		sym_table.addScope(ctx->getStart()->getLine());
	}
	virtual void exitBlock(CACTParser::BlockContext * ctx) override { 
		sym_table.scope_ret();
	}
  
	virtual void enterBlockItem(CACTParser::BlockItemContext * ctx) override { 
		if(ctx->decl())		// blockItem : decl
		{
			// intentionally left empty
		} else if(ctx->stmt()) {		// blockItem : stmt
			ctx->stmt()->ret_type = ctx->ret_type;
			ctx->stmt()->in_loop = ctx->in_loop;
		}
	}
	virtual void exitBlockItem(CACTParser::BlockItemContext * ctx) override { 
		// intentionally left empty
	}

	virtual void enterStmt(CACTParser::StmtContext * ctx) override { 
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
		} else if(ctx->block()) {		// stmt : block
			// sym_table.addScope(line);
			ctx->block()->in_loop = ctx->in_loop;
			ctx->block()->ret_type = ctx->ret_type;
			// sym_table.curr_scope = sym_table.curr_scope->curr_func->scope.children[line];
		} else if(ctx->getStart()->getText() == std::string("if")) {		// stmt : 'if' '(' cond ')' stmt ('else' stmt)?
			ctx->stmt().at(0)->in_loop = ctx->in_loop;
			ctx->stmt().at(1)->in_loop = ctx->in_loop;
			ctx->stmt().at(0)->ret_type = ctx->ret_type;
			ctx->stmt().at(1)->ret_type = ctx->ret_type;
		} else if(ctx->getStart()->getText() == std::string("while")) {		// stmt : 'while' '(' cond ')' stmt
			ctx->stmt().at(0)->in_loop = 1;
			ctx->stmt().at(0)->ret_type = ctx->ret_type;
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
		}
	}
	virtual void exitStmt(CACTParser::StmtContext * ctx) override { 
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
		
	}

	virtual void enterExp(CACTParser::ExpContext * ctx) override { 
		// intentionally left empty
	}
	virtual void exitExp(CACTParser::ExpContext * ctx) override { 
	// printf("Exiting Exp\n");
		if(ctx->addExp())		// exp : addExp
		{
			ctx->btype = ctx->addExp()->btype;
			ctx->array_len = ctx->addExp()->array_len;
		} else {		// exp : BoolConst
			ctx->btype = BTYPE_BOOL;
			ctx->array_len = 0;
		}
	}

	virtual void enterCond(CACTParser::CondContext * /*ctx*/) override { 
	// printf("Entering Cond\n");
	}
	virtual void exitCond(CACTParser::CondContext * ctx) override { 
		int line = ctx->getStart()->getLine();
    	if(ctx->lOrExp()->btype != BTYPE_BOOL)
    	{
      		std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
								<< ":" << "cond is not bool" << std::endl;
      		semanticErrors++;
    	}
	// printf("Exiting Cond\n");

	}

	virtual void enterLVal(CACTParser::LValContext * ctx) override { 
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
	virtual void exitLVal(CACTParser::LValContext * ctx) override { 
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
		} else {		//lVal : Ident

		}
	}

	virtual void enterPrimaryExp(CACTParser::PrimaryExpContext * /*ctx*/) override { 
    
	// printf("Entering PrimaryExp\n");
	}
	virtual void exitPrimaryExp(CACTParser::PrimaryExpContext * ctx) override { 
	if(ctx->number())
    {
        ctx->btype = ctx->number()->btype;
		ctx->array_len = 0;
    }
    else if(ctx->exp())
    {
        ctx->btype = ctx->exp()->btype;
		ctx->array_len = ctx->exp()->array_len;
    }
    else if(ctx->lVal())
    {
        ctx->btype = ctx->lVal()->btype;
		ctx->array_len = ctx->lVal()->array_len;
    }
	// printf("Exiting PrimaryExp\n");

	}

	virtual void enterNumber(CACTParser::NumberContext * /*ctx*/) override { 
	// printf("Entering Number\n");
	}
	virtual void exitNumber(CACTParser::NumberContext * ctx) override { 
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

	}

	virtual void enterUnaryExp(CACTParser::UnaryExpContext * ctx) override {
		
	// printf("Entering UnaryExp\n");
	}
	virtual void exitUnaryExp(CACTParser::UnaryExpContext * ctx) override { 
		if(ctx->primaryExp() != NULL){
			ctx->btype = ctx->primaryExp()->btype;
			ctx->array_len =ctx->primaryExp()->array_len;
		}
        else if(ctx->Ident() != NULL){
			FuncInfo * func;
			ctx->array_len = 0;
			if((func = sym_table.lookup_func(ctx->Ident()->getText())) == NULL){
				semanticErrors++;
				std::cout << "Undefined function\n";
				return;
			}
			ctx->btype = func->ret_type;
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
        }
		else{
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
		}
	// printf("Exiting UnaryExp\n");

	}

	virtual void enterUnaryOp(CACTParser::UnaryOpContext * /*ctx*/) override { 
	// printf("Entering UnaryOp\n");
	}
	virtual void exitUnaryOp(CACTParser::UnaryOpContext * /*ctx*/) override { 
	// printf("Exiting UnaryOp\n");

	}

	virtual void enterFuncRParams(CACTParser::FuncRParamsContext * /*ctx*/) override { 
	// printf("Entering FuncRParams\n");
	}
	virtual void exitFuncRParams(CACTParser::FuncRParamsContext * /*ctx*/) override { 
	// printf("Exiting FuncRParams\n");

	}

	virtual void enterMulExp(CACTParser::MulExpContext * /*ctx*/) override { 
	// printf("Entering MulExp\n");
	}
	virtual void exitMulExp(CACTParser::MulExpContext * ctx) override { 
		int line = ctx->getStart()->getLine();
    	if(ctx->mulExp())
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
    	}
    	else if(ctx->unaryExp())
    	{
        	ctx->btype = ctx->unaryExp()->btype;
			ctx->array_len = ctx->unaryExp()->array_len;
    	}
	// printf("Exiting MulExp\n");

	}

	virtual void enterAddExp(CACTParser::AddExpContext * /*ctx*/) override { 
	// printf("Entering AddExp\n");
	}
	virtual void exitAddExp(CACTParser::AddExpContext * ctx) override { 
	int line = ctx->getStart()->getLine();
    if(ctx->addExp())
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
    }
    else if(ctx->mulExp())
    {
        ctx->btype = ctx->mulExp()->btype;
		ctx->array_len = ctx->mulExp()->array_len;
    } 
	// printf("Exiting AddExp\n");

	}

	virtual void enterRelExp(CACTParser::RelExpContext * /*ctx*/) override { 
	// printf("Entering RelExp\n");
	}
	virtual void exitRelExp(CACTParser::RelExpContext * ctx) override { 
		int line = ctx->getStart()->getLine();
    	if(ctx->relExp())
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
			
   		}
    	else if(ctx->boolConst())
    	{
      		ctx->btype = BTYPE_BOOL;
    	}
    	else if(ctx->addExp())
    	{
      		if(ctx->addExp()->array_len != 0)
			{
				std::cout << line << ":" << ctx->getStart()->getCharPositionInLine() \
								<< ":" << "lor be an array" << std::endl;
            	semanticErrors++;
			}
			ctx->btype = ctx->addExp()->btype;
    	}
	
	// printf("Exiting RelExp\n");

	}

	virtual void enterEqExp(CACTParser::EqExpContext * /*ctx*/) override { 
	// printf("Entering EqExp\n");
	}
	virtual void exitEqExp(CACTParser::EqExpContext * ctx) override { 
		int line = ctx->getStart()->getLine();
    	if(ctx->eqExp())
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
			  
    	}
    	else if(ctx->relExp())
    	{
      		ctx->btype = ctx->relExp()->btype;
    	}
	// printf("Exiting EqExp\n");

	}

	virtual void enterLAndExp(CACTParser::LAndExpContext * /*ctx*/) override { 
	// printf("Entering LAndExp\n");
	}
	virtual void exitLAndExp(CACTParser::LAndExpContext * ctx) override { 
		int line = ctx->getStart()->getLine();
    	if(ctx->lAndExp())
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
    	}
    	else if(ctx->eqExp())
    	{
      		ctx->btype = ctx->eqExp()->btype;
  		}
	// printf("Exiting LAndExp\n");

	}

	virtual void enterLOrExp(CACTParser::LOrExpContext * /*ctx*/) override { 
	// printf("Entering LOrExp\n");
	}
	virtual void exitLOrExp(CACTParser::LOrExpContext * ctx) override { 
		int line = ctx->getStart()->getLine();
    	if(ctx->lOrExp())
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
    	}
    	else if(ctx->lAndExp())
    	{
      		ctx->btype = ctx->lAndExp()->btype;
    	}
	// printf("Exiting LOrExp\n");

	}

	virtual void enterConstExpNumber(CACTParser::ConstExpNumberContext * ctx) override { 
	// printf("Entering ConstExpNumber\n");
	}
	virtual void exitConstExpNumber(CACTParser::ConstExpNumberContext * ctx) override;

	virtual void enterConstExpBoolConst(CACTParser::ConstExpBoolConstContext * /*ctx*/) override { 
	// printf("Entering ConstExpBoolConst\n");
	}

	virtual void exitConstExpBoolConst(CACTParser::ConstExpBoolConstContext * ctx) override { 
	ctx->btype = BTYPE_BOOL;
	// printf("Exiting ConstExpBoolConst\n");
	}


	virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { 
	//printf("Entering a rule\n");
	}
	virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
	virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { 
	//    printf("visiting a terminal\n");
	}
	virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};


#endif