#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#pragma

#include "../grammar/CACTBaseListener.h"
#include "./symbolTable.h"

extern CACTParser::CompUnitContext *compUnit;
extern std::string filename;


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


	virtual void enterCompUnit(CACTParser::CompUnitContext * ctx) override;
	virtual void exitCompUnit(CACTParser::CompUnitContext * ctx) override;

	virtual void enterDecl(CACTParser::DeclContext * ctx) override;
	virtual void exitDecl(CACTParser::DeclContext * ctx) override;
		
	virtual void enterConstDecl(CACTParser::ConstDeclContext * ctx) override;
	virtual void exitConstDecl(CACTParser::ConstDeclContext * ctx) override;

	virtual void enterBType(CACTParser::BTypeContext * ctx) override;
	virtual void exitBType(CACTParser::BTypeContext * ctx) override ;

	virtual void enterConstDef(CACTParser::ConstDefContext * ctx) override;
	virtual void exitConstDef(CACTParser::ConstDefContext * ctx) override;

	virtual void enterConstInitVal(CACTParser::ConstInitValContext * ctx) override;
	virtual void exitConstInitVal(CACTParser::ConstInitValContext * ctx) override;

	virtual void enterVarDecl(CACTParser::VarDeclContext * ctx) override;
	virtual void exitVarDecl(CACTParser::VarDeclContext * ctx) override;

	virtual void enterVarDef(CACTParser::VarDefContext * ctx) override;
	virtual void exitVarDef(CACTParser::VarDefContext * ctx) override;

	virtual void enterFuncDef(CACTParser::FuncDefContext * ctx) override;
	virtual void exitFuncDef(CACTParser::FuncDefContext * ctx) override;

	virtual void enterFuncType(CACTParser::FuncTypeContext * ctx) override;
	virtual void exitFuncType(CACTParser::FuncTypeContext * ctx) override;

	virtual void enterFuncFParams(CACTParser::FuncFParamsContext * ctx) override;
	virtual void exitFuncFParams(CACTParser::FuncFParamsContext * ctx) override;

	virtual void enterFuncFParam(CACTParser::FuncFParamContext * ctx) override;
	virtual void exitFuncFParam(CACTParser::FuncFParamContext * ctx) override;

	virtual void enterBlock(CACTParser::BlockContext * ctx) override;
	virtual void exitBlock(CACTParser::BlockContext * ctx) override;
  
	virtual void enterBlockItem(CACTParser::BlockItemContext * ctx) override;
	virtual void exitBlockItem(CACTParser::BlockItemContext * ctx) override;

	virtual void enterStmt(CACTParser::StmtContext * ctx) override;
	virtual void exitStmt(CACTParser::StmtContext * ctx) override;

	virtual void enterExp(CACTParser::ExpContext * ctx) override;
	virtual void exitExp(CACTParser::ExpContext * ctx) override;

	virtual void enterCond(CACTParser::CondContext * /*ctx*/) override;
	virtual void exitCond(CACTParser::CondContext * ctx) override;

	virtual void enterLVal(CACTParser::LValContext * ctx) override;
	virtual void exitLVal(CACTParser::LValContext * ctx) override;

	virtual void enterPrimaryExp(CACTParser::PrimaryExpContext * /*ctx*/) override;
	virtual void exitPrimaryExp(CACTParser::PrimaryExpContext * ctx) override;

	virtual void enterNumber(CACTParser::NumberContext * /*ctx*/) override;
	virtual void exitNumber(CACTParser::NumberContext * ctx) override;

	virtual void enterUnaryExp(CACTParser::UnaryExpContext * ctx) override;
	virtual void exitUnaryExp(CACTParser::UnaryExpContext * ctx) override;

	virtual void enterUnaryOp(CACTParser::UnaryOpContext * /*ctx*/) override;
	virtual void exitUnaryOp(CACTParser::UnaryOpContext * /*ctx*/) override;

	virtual void enterFuncRParams(CACTParser::FuncRParamsContext * /*ctx*/) override;
	virtual void exitFuncRParams(CACTParser::FuncRParamsContext * /*ctx*/) override;

	virtual void enterMulExp(CACTParser::MulExpContext * /*ctx*/) override;
	virtual void exitMulExp(CACTParser::MulExpContext * ctx) override;

	virtual void enterAddExp(CACTParser::AddExpContext * /*ctx*/) override;
	virtual void exitAddExp(CACTParser::AddExpContext * ctx) override;

	virtual void enterRelExp(CACTParser::RelExpContext * /*ctx*/) override;
	virtual void exitRelExp(CACTParser::RelExpContext * ctx) override;

	virtual void enterEqExp(CACTParser::EqExpContext * /*ctx*/) override;
	virtual void exitEqExp(CACTParser::EqExpContext * ctx) override;

	virtual void enterLAndExp(CACTParser::LAndExpContext * /*ctx*/) override;
	virtual void exitLAndExp(CACTParser::LAndExpContext * ctx) override;

	virtual void enterLOrExp(CACTParser::LOrExpContext * /*ctx*/) override;
	virtual void exitLOrExp(CACTParser::LOrExpContext * ctx) override;

	virtual void enterConstExpNumber(CACTParser::ConstExpNumberContext * ctx) override;
	virtual void exitConstExpNumber(CACTParser::ConstExpNumberContext * ctx) override;

	virtual void enterConstExpBoolConst(CACTParser::ConstExpBoolConstContext * /*ctx*/) override;

	virtual void exitConstExpBoolConst(CACTParser::ConstExpBoolConstContext * ctx) override;


	virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
	virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
	virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override;
	virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override;

};


#endif