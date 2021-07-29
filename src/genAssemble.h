#ifndef GEN_ASSEMBLE_H
#define GEN_ASSEMBLE_H

#include "./symbolTable.h"
#include "./list.h"
#include "./threeAdressCode.h"

//需要一张表记录变量存储位置
struct address
{
	list_head list;
	std::string id;
	int off;
	int type;//1是寄存器，2内存，3全局
	std::string addr;
	int width;
};

struct floatc
{
	list_head list;
	std::string label;
	int type;
	void * data;
};

struct param{
	int btype;
	std::string name;
};

struct paramList{
	int numParams;
	struct param * p;
};

std::string lookup(std::string id, int off);

int global(std::string id);

std::string alloc(std::string id, int off, int width, int * seq);

void genDecl(FILE * fp, threeAdressCode code);

void genFunc(FILE * fp, threeAdressCode code, struct paramList * param);

void genFloat(FILE * fp);

#endif