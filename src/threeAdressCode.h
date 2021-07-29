#ifndef THREE_ADRESS_CODE_H
#define THREE_ADRESS_CODE_H

#include "./utils.h"
#include "./symbolTable.h"

// enum TACtype
// {
// 	TAC_ASSIGN,			// 赋值语句
// 	TAC_PARAM,			// 设置参数语句
// 	TAC_CALL,			// 函数调用语句
// 	TAC_RETURN,			// 函数返回语句
// 	TAC_IF_GOTO,		// 有条件满足跳转语句
// 	TAC_IF_FAULSE_GOTO,	// 有条件不满足跳转语句
// 	TAC_GOTO 			// 无条件跳转语句
// };

enum TACop
{
	TACOP_NONE,		// 无操作符
	TACOP_ADD,		// 双目 加法 +		result = arg1 + arg2
	TACOP_SUB,		// 双目 减法 -		result = arg1 - arg2
	TACOP_MUL,		// 双目 乘法 *		result = arg1 * arg2
	TACOP_DIV,		// 双目 除法 /		result = arg1 / arg2
	TACOP_MOD,		// 双目 取模 %		result = arg1 % arg2
	TACOP_MINUS,	// 单目 取负 -		result = -arg1
	TACOP_NOT,		// 单目 取否 ！		result = !arg1
	TACOP_ROFF,		// 双目 数组读访问 	result = arg1[arg2] 代表地址arg1往后arg2个字节处的变量值
	TACOP_WOFF,		// 双目 数组写访问 	result[arg1] = arg2 
	TACOP_ASSIGN,	// 赋值语句			result = arg1
	TACOP_PARAM, 	// 设置参数语句		param arg1
	TACOP_CALL,		// 函数调用语句		result = call arg1(arg2) 
	TACOP_RETURN,	// 函数返回语句		return arg1
	TACOP_J_LT,		// 函数跳转语句		if arg1 < arg2, goto result
	TACOP_J_LE,		// 函数跳转语句		if arg1 <= arg2, goto result
	TACOP_J_GT,		// 函数跳转语句		if arg1 > arg2, goto result
	TACOP_J_GE,		// 函数跳转语句		if arg1 >= arg2, goto result
	TACOP_J_EQ,		// 函数跳转语句		if arg1 == arg2, goto result
	TACOP_J_NE,		// 函数跳转语句		if arg1 != arg2, goto result
	TACOP_J,		// 函数跳转语句		goto arg1
	TACOP_LABEL		// 标号语句			label arg1:
};

enum TACrel
{
	TACREL_EQ,		// ==
	TACREL_NE,		// !=
	TACREL_LT,		// <
	TACREL_GT,		// >
	TACREL_LE,		// <=
	TACREL_GE		// >=
};

class TACoperand
{
	public:
	int type;			// 类型
	void* data;			// 数据
	std::string temp;	// 变量暂存名
	int size;			// 类型宽度
	bool imm;			// 是否为立即数，默认为1，变量为0


	TACoperand(){};

	void init(int type, bool imm)
	{
		this->type = type;
		this->imm = imm;
		switch(type)
		{
			case BTYPE_BOOL: size = 4 ; break;
			case BTYPE_INT: size = 4; break;
			case BTYPE_FLOAT: size = 4; break;
			case BTYPE_DOUBLE: size = 8; break;
			default: size = 0; break;
		}
		if(imm)
			data = malloc(size);
	}

	int copyImm(void* src)
	{
		if(imm == 0)
			return -1;
		memcpy(data, src, size);
		return 1;
	}

	int copyName(std::string name)
	{
		if(imm == 1)
			return -1;
		this->temp = name;
		return 1;
	}
};

struct TACExp
{
	TACop op;
	TACoperand operand1;
	TACoperand operand2;
};

struct TACBoolExp
{
	TACrel rel;
	TACoperand operand1;
	TACoperand operand2;
};

// 四元式
struct TACline
{
	TACop op;
	TACoperand arg1;
	TACoperand arg2;
	TACoperand result;
};

class threeAdressCode
{
	public:
	int num_lines;		// 三地址码行数
	int cur_line;
	TACline* lines;		// 存放三地址代码

	threeAdressCode(int num = 1024)
	{
		lines = (TACline*)malloc(sizeof(TACline) * num);
		num_lines = num;
		cur_line = 0;
	}

	void copyline(TACline *line1, TACline *line2){
		//memcpy(line1, line2, sizeof(TACline));
		line1->op = line2->op;
		if(line2->result.imm == 0){
			line1->result.temp = line2->result.temp;
		}
		line1->result.data = line2->result.data;
		line1->result.imm = line2->result.imm;
		line1->result.size = line2->result.size;
		line1->result.type = line2->result.type;

		if(line2->arg1.imm == 0){
			line1->arg1.temp = line2->arg1.temp;
		}
		line1->arg1.data = line2->arg1.data;
		line1->arg1.imm = line2->arg1.imm;
		line1->arg1.size = line2->arg1.size;
		line1->arg1.type = line2->arg1.type;
		
		if(line2->arg2.imm == 0){
			line1->arg2.temp = line2->arg2.temp;
		}
		line1->arg2.data = line2->arg2.data;
		line1->arg2.imm = line2->arg2.imm;
		line1->arg2.size = line2->arg2.size;
		line1->arg2.type = line2->arg2.type;
	}

	void addline(TACline line)
	{
		copyline(&(lines[cur_line]), &line);
		cur_line++;
		if(cur_line >= num_lines)
		{
			num_lines <<= 1;
			lines = (TACline*)realloc(lines, sizeof(TACline) * num_lines);
		}
	}

	void addlines(TACline lines[], int num)
	{
		for(int i = 0; i < num; i++)
			addline(lines[i]);
	}

	// 合并两个三地址代码段
	void codecat(threeAdressCode code)
	{
		addlines(code.lines, code.cur_line);
	}

	void printOperand(TACoperand arg){
		if(arg.imm == 0){
			std::cout << arg.temp;
		}
		else{
			if(arg.type == BTYPE_BOOL){
				std::cout << *(int *)arg.data;
			}
			else if(arg.type == BTYPE_INT){
				std::cout << *(int *)arg.data;
			}
			else if(arg.type == BTYPE_FLOAT){
				std::cout << *(float *)arg.data;
			}
			else if(arg.type == BTYPE_DOUBLE){
				std::cout << *(double *)arg.data;
			}
		}
	}

	void printTAC(){
		std::cout << "total lines : " << cur_line << std::endl;
		for(int i = 0; i < cur_line; i++){
			//std::cout << "line " << i << std::endl;
			if(lines[i].op == TACOP_NONE){
				printf("none\n");
			}
			else if(lines[i].op == TACOP_ADD){
				std::cout << "add" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				printOperand(lines[i].arg1);
				std::cout << " + ";
				printOperand(lines[i].arg2);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_SUB){
				std::cout << "sub" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				printOperand(lines[i].arg1);
				std::cout << " - ";
				printOperand(lines[i].arg2);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_MUL){
				std::cout << "mul" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				printOperand(lines[i].arg1);
				std::cout << " * ";
				printOperand(lines[i].arg2);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_DIV){
				std::cout << "div" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				printOperand(lines[i].arg1);
				std::cout << " / ";
				printOperand(lines[i].arg2);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_MOD){
				std::cout << "mod" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				printOperand(lines[i].arg1);
				std::cout << " % ";
				printOperand(lines[i].arg2);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_MINUS){
				std::cout << "minus" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				std::cout << "-";
				printOperand(lines[i].arg1);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_NOT){
				std::cout << "not" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				std::cout << "!";
				printOperand(lines[i].arg1);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_ROFF){
				std::cout << "read array" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				printOperand(lines[i].arg1);
				std::cout << "[";
				printOperand(lines[i].arg2);
				std::cout << "]" << std::endl;
			}
			else if(lines[i].op == TACOP_WOFF){
				std::cout << "write array" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << "[";
				printOperand(lines[i].arg1);
				std::cout << "] = ";
				printOperand(lines[i].arg2);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_ASSIGN){
				std::cout << "assign" << std::endl << "\t";
				printOperand(lines[i].result);
				std::cout << " = ";
				printOperand(lines[i].arg1);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_PARAM){
				std::cout << "param" << std::endl << "\t";
				printOperand(lines[i].arg1);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_CALL){
				std::cout << "call" << std::endl << "\t";
				if(lines[i].result.imm == 0){
					printOperand(lines[i].result);
					std::cout << " = ";
				}
				printOperand(lines[i].arg1);
				std::cout << "\tn_params : ";
				printOperand(lines[i].arg2);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_RETURN){
				std::cout << "return" << std::endl << "\t";
				printOperand(lines[i].arg1);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_J_LT){
				std::cout << "blt" << std::endl << "\t";
				std::cout << "if ";
				printOperand(lines[i].arg1);
				std::cout << " < ";
				printOperand(lines[i].arg2);
				std::cout << " goto ";
				printOperand(lines[i].result);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_J_LE){
				std::cout << "ble" << std::endl << "\t";
				std::cout << "if ";
				printOperand(lines[i].arg1);
				std::cout << " <= ";
				printOperand(lines[i].arg2);
				std::cout << " goto ";
				printOperand(lines[i].result);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_J_GT){
				std::cout << "bgt" << std::endl << "\t";
				std::cout << "if ";
				printOperand(lines[i].arg1);
				std::cout << " > ";
				printOperand(lines[i].arg2);
				std::cout << " goto ";
				printOperand(lines[i].result);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_J_GE){
				std::cout << "bge" << std::endl << "\t";
				std::cout << "if ";
				printOperand(lines[i].arg1);
				std::cout << " >= ";
				printOperand(lines[i].arg2);
				std::cout << " goto ";
				printOperand(lines[i].result);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_J_EQ){
				std::cout << "beq" << std::endl << "\t";
				std::cout << "if ";
				printOperand(lines[i].arg1);
				std::cout << " == ";
				printOperand(lines[i].arg2);
				std::cout << " goto ";
				printOperand(lines[i].result);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_J_NE){
				std::cout << "bne" << std::endl << "\t";
				std::cout << "if ";
				printOperand(lines[i].arg1);
				std::cout << " != ";
				printOperand(lines[i].arg2);
				std::cout << " goto ";
				printOperand(lines[i].result);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_J){
				std::cout << "j" << std::endl << "\tgoto ";
				printOperand(lines[i].arg1);
				std::cout << std::endl;
			}
			else if(lines[i].op == TACOP_LABEL){
				std::cout << "label" << std::endl << "\t";
				printOperand(lines[i].arg1);
				std::cout << " :" << std::endl;
			}
		}
	}
};

std::string mynewlabel();
std::string mynewtemp();

extern threeAdressCode TAC;
extern std::vector< std::pair< std::string, int > > array_vector;
extern std::vector< std::string > array_vector_str;

#endif