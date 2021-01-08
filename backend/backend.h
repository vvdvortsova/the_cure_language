#ifndef THE_CURE_LANGUAGE_BACKEND_H
#define THE_CURE_LANGUAGE_BACKEND_H

#include <vector>
#include "../frontend/language.h"



struct VarIndex {
    char* name;
    int index;
};

struct FuncScope {
    char* funcName = nullptr;
    std::vector<VarIndex*> vars;
};


enum ASM_OP {
    ASM_SQRT = 1,
    ASM_SIN = 2,
    ASM_COS = 3,
    ASM_OUT = 4,
    ASM_IN = 5,
    ASM_ADD,
    ASM_SUB,
    ASM_MUL,
    ASM_DIV,
    ASM_POW,
    ASM_MORE,
    ASM_LESS,
    ASM_EQUAL
};

const char* getAsmOp(int);

void buildBackend(Node* root, char* outPath);
void startParsing(Node* root, std::ofstream& file,  std::vector<FuncScope*>** FUNCS);

void initFuncScopes(std::vector<FuncScope*>** FUNCS);

void freeFuncsScopes();

void createFunctionASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS);

void createVariableASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS);

void createReturnASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS);

void createOutputInputAsm(Node* root, std::ofstream& file, int op, std::vector<FuncScope*>** FUNC);

void createMathOpASM(Node* root, std::ofstream& file, int op, std::vector<FuncScope*>** FUNCS);

void createBoolOpASM(Node* root, std::ofstream& file, int op, std::vector<FuncScope*>** FUNCS);

void createIFASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS);

void createCallFuncAsm(Node*root, std::ofstream& file, std::vector<FuncScope*>** FUNCS);

#endif //THE_CURE_LANGUAGE_BACKEND_H
