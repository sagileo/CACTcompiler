#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "./utils.h"


enum btype{
    BTYPE_INT,
    BTYPE_FLOAT,
    BTYPE_DOUBLE,
    BTYPE_BOOL,
	BTYPE_ANY
};
enum functype{
    FUNC_INT,
    FUNC_FLOAT,
    FUNC_DOUBLE,
    FUNC_BOOL,
    FUNC_VOID
};

// enum{
//     TYPE_BASIC,
//     TYPE_ARRAY
// };

struct FuncInfo;

struct VarInfo
{
    std::string name;
    int btype;
    int is_const;
    //int vartype;    // basic or array
    int array_len;  // if this variable is basic type, then it is set to 0
};

struct ScopeTable
{
    int start_line;
    std::map<int, ScopeTable *> children;
    std::map<std::string, VarInfo> local_var;
    ScopeTable *parent;
    FuncInfo* curr_func;
};

struct FuncInfo
{
    std::string name;
    int ret_type;
    int cnt_para;
    std::map<int, VarInfo> para;
    ScopeTable scope;
};

class SymbolTable
{
    public:
        // global variable table
        std::map<std::string, VarInfo> global_var;
        // function table
        std::map<std::string, FuncInfo> func;
        // current scope
        ScopeTable* curr_scope;

    public :
        SymbolTable(){
            curr_scope = NULL;

            addFunc("print_int", FUNC_VOID, 0);
            addPara("a", BTYPE_INT, 0);
            scope_ret();

            addFunc("print_float", FUNC_VOID, 0);
            addPara("a", BTYPE_FLOAT, 0);
            scope_ret();

            addFunc("print_double", FUNC_VOID, 0);
            addPara("a", BTYPE_DOUBLE, 0);
            scope_ret();

            addFunc("print_bool", FUNC_VOID, 0);
            addPara("a", BTYPE_BOOL, 0);
            scope_ret();

            addFunc("get_int", FUNC_INT, 0);
            scope_ret();

            addFunc("get_float", FUNC_FLOAT, 0);
            scope_ret();

            addFunc("get_double", FUNC_DOUBLE, 0);
            scope_ret();
        };
        ~SymbolTable(){};


        //look up var in current scope
        VarInfo * lookup_curr(const std::string &name);
        //look up var in all scope
        VarInfo * lookup_all(const std::string &name);
        //look up function
        FuncInfo * lookup_func(const std::string &name);
        //look up parameter
        VarInfo *lookup_para(FuncInfo * func, int index);

        void addFunc(std::string name, int ret_type, int start_line);
        void addPara(std::string name, int btype, int len);
        void addVar(std::string name, int btype, int len, int is_const);
        void addScope(int start_line);

        void scope_ret();
};

extern int semanticErrors;
extern SymbolTable sym_table;

#endif