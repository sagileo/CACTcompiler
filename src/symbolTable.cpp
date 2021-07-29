#include "symbolTable.h"

SymbolTable sym_table;

int getNum(){
	static int scopeNum = 1;
	return scopeNum++;
}

VarInfo * SymbolTable::lookup_curr(const std::string &name){
    if(curr_scope == NULL){
        if(this->global_var.count(name) > 0){
            return &(this->global_var[name]);
        }
        else return NULL;
    }
    else{
        if(curr_scope->local_var.count(name) > 0){
            return &(curr_scope->local_var[name]);
        }
        else return NULL;
    }
}

VarInfo * SymbolTable::lookup_all(const std::string &name){
    ScopeTable * scope = curr_scope;
    if(scope != NULL){
        VarInfo * para;
        for(int i = 0; (para = lookup_para(scope->curr_func, i)) != NULL; i++){
            if(para->name == name){
                return para;
            }
        }
    }
    while(scope != NULL){
        if(scope->local_var.count(name) > 0){
            return &(scope->local_var[name]);
        }
        else scope = scope->parent;
    }
    if(this->global_var.count(name) > 0){
        return &(this->global_var[name]);
    }
    else return NULL;
}

FuncInfo * SymbolTable::lookup_func(const std::string &name){
    if(this->func.count(name) > 0){
        return &(this->func[name]);
    }
    else return NULL;
}

VarInfo * SymbolTable::lookup_para(FuncInfo * func, int index){
    if(func->para.count(index) > 0){
        return &(func->para[index]);
    }
    else return NULL;
}

void SymbolTable::addFunc(std::string name, int ret_type, int start_line){
    if(lookup_func(name) != NULL){
        semanticErrors++;
        std::cout << "Function rename\n";
    }
    FuncInfo *func = new FuncInfo;
    func->name = name;
    func->ret_type = ret_type;
    func->cnt_para = 0;
    func->scope.start_line = start_line;
    func->scope.curr_func = func;
    func->scope.parent = NULL;
    curr_scope = &(func->scope);
    this->func.insert(std::pair<std::string, FuncInfo>(name, *func));
    curr_scope->curr_func = &(this->func[name]);
}

void SymbolTable::addPara(std::string name, int btype, int len){
    if(lookup_curr(name) != NULL){
        semanticErrors++;
        std::cout << "Parameter rename\n";
        return;
    }
    VarInfo * var = new VarInfo;
    var->name = name;
    var->btype = btype;
    var->array_len = len;
    curr_scope->curr_func->para.insert(std::pair<int, VarInfo>(curr_scope->curr_func->cnt_para, *var));
    curr_scope->curr_func->cnt_para++;
}

void SymbolTable::addVar(std::string name, int btype, int len, int is_const){
    if(lookup_curr(name) != NULL){
        semanticErrors++;
        std::cout << "Variable rename\n";
        return;
    }
    if(curr_scope != NULL){
        VarInfo * para;
        for(int i = 0; (para = lookup_para(curr_scope->curr_func, i)) != NULL; i++){
            if(para->name == name){
                semanticErrors++;
                std::cout << "Same name for parameter and variable\n";
                return;
            }
        }
    }
    VarInfo * var = new VarInfo;
    var->btype = btype;
    var->array_len = len;
    var->is_const = is_const;
    if(curr_scope != NULL){
		var->scopeID = curr_scope->id;
        curr_scope->local_var.insert(std::pair<std::string, VarInfo>(name, *var));
    }
    else{
		var->scopeID = 0;
		global_var.insert(std::pair<std::string, VarInfo>(name, *var));
	}
}

void SymbolTable::addScope(int start_line){
    ScopeTable * scope = new ScopeTable;
	scope->id = getNum();
    scope->curr_func = curr_scope->curr_func;
    scope->parent = curr_scope;
    scope->start_line = start_line;
    curr_scope = scope;
}

void SymbolTable::scope_ret(){
    curr_scope = curr_scope->parent;
}