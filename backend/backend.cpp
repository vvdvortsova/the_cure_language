#include <assert.h>
#include <cstring>
#include "backend.h"

int countOfIf        = 0;
int ramFreeSpace     = 0;
int currentFunc = 0;//"main"

void createIfElseAsm(Node *root, std::ofstream &file, std::vector<FuncScope *> **FUNCS);

void createWhileASM(Node *root, std::ofstream &file, std::vector<FuncScope *> **FUNCS);

int getBoolType(int type);

const char* getAsmOp(int op) {
    switch(op) {
        case ASM_IN:
            return "in";
        case ASM_OUT:
            return "out";
        case ASM_ADD:
            return "add";
        case ASM_SUB:
            return "sub";
        case ASM_MUL:
            return "mul";
        case ASM_DIV:
            return "div";
        case ASM_MORE:
            return "jg";
        case ASM_LESS:
            return "jl";
        case ASM_EQUAL:
            return "je";
        default:
            return nullptr;
    }
}


void buildBackend(Node* root, char* outPath) {
    assert(root);
    std::vector<FuncScope*>* FUNCS;
    std::ofstream file;
    file.open (outPath);
    initFuncScopes(&FUNCS);
    file << "tmain:\n";
    startParsing(root, file, &FUNCS);
    freeFuncsScopes();
    file.close();
}

void freeFuncsScopes() {

}

void initFuncScopes(std::vector<FuncScope*>** FUNCS) {
    auto mainFunc = new FuncScope();
    mainFunc->funcName = "main";
    (*FUNCS) = new std::vector<FuncScope*>();
    (*FUNCS)->push_back(mainFunc);
}

void startParsing(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {
    if(!root)
        return;
    //firstly: start to print main
    switch(root->data->type) {
        case CLASS_OP:
        case CLASS_POINT:
        case DEF_FUNC:
            break;
        case CLASS_FUNC_NAME:
            switch(int(root->data->value)) {
                case M:// if find main
                    break;
                default://another function
                    createFunctionASM(root, file, FUNCS);
                    startParsing(root->rightChild, file, FUNCS);
                    return;
            }
            break;
        case CLASS_SYSTEM_OP:
            switch(int(root->data->value)) {
                case IF_ELSE_OP:
                    createIfElseAsm(root, file, FUNCS);
                    return;
                case IF_OP:
                    createIFASM(root, file, FUNCS);
                    return;
                case WHILE_OP:
                    createWhileASM(root, file, FUNCS);
                    break;
                case RETURN_OP:
                    createReturnASM(root, file, FUNCS);
                    return;
                case PRINT_OP: // toDO beautify
                    createOutputInputAsm(root, file, ASM_IN, FUNCS);
                    return;
                case SCAN_OP:
                    createOutputInputAsm(root, file, ASM_OUT, FUNCS);
                    return;
                case DEF_VAR:
                    startParsing(root->rightChild, file, FUNCS);
                    return;
                default:
                    break;
            }
            break;
        case CLASS_MATH_OP:// toDO beautify
            startParsing(root->rightChild, file, FUNCS);
            startParsing(root->leftChild, file, FUNCS);
            switch (int(root->data->value)) {
                case ADD:
                    createMathOpASM(root, file, ASM_ADD, FUNCS);
                    break;
                case SUB:
                    createMathOpASM(root, file, ASM_SUB, FUNCS);
                    break;
                case MUL:
                    createMathOpASM(root, file, ASM_MUL, FUNCS);
                    break;
                case DIV:
                    createMathOpASM(root, file, ASM_DIV, FUNCS);
                    break;
            }
            return;
        case CLASS_MATH_FUNC:
        case CLASS_BOOL_SIGN:
            startParsing(root->rightChild, file, FUNCS);
            startParsing(root->leftChild, file, FUNCS);
            return;
        case CLASS_CALL_FUNC:
            createCallFuncAsm(root, file, FUNCS);
            return;
        case CLASS_NUMBER:
            file << "push " << root->data->value << "\n";
            return;
        case CLASS_VARIABLE:
            createVariableASM(root, file, FUNCS);
            if(root->rightChild != nullptr) {
                if( root->rightChild->data->type == CLASS_CALL_FUNC && root->rightChild->leftChild != nullptr) {
                    file << "pop [" << ((**FUNCS)[currentFunc])->vars[0]->index << "]\n";
                }
            }
            return;
        default:
            printf("unexpected type of node %d", root->data->type);
            break;
    }
    startParsing(root->leftChild, file, FUNCS);
    startParsing(root->rightChild, file, FUNCS);
}

void createWhileASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {
    //params
    startParsing(root->leftChild, file, FUNCS);
    //prewhile
    int jePrewhile = countOfIf++;
    file << "tPreWhileCond" << jePrewhile << ":\n";
    file << getAsmOp(getBoolType(int(root->leftChild->data->value)));

    int jeCond = countOfIf++;
    file << " twhileCond" << jeCond << "\n";
    //point to main
    int pointToMain = countOfIf++;
    file << "jmp tMain" << pointToMain << "\n";
    file << "twhileCond" << jeCond << ":\n";
    //body
    startParsing(root->rightChild, file, FUNCS);
    //params
    startParsing(root->leftChild, file, FUNCS);
    file << "jmp tPreWhileCond" << jePrewhile << "\n";
    //point to main
    file << "tMain" << pointToMain << ":\n";
}

int getBoolType(int type) {
    switch (type) {
        case MORE:
            return ASM_MORE;
        case LESS:
            return ASM_LESS;
        case EQUAL:
            return ASM_EQUAL;
        default:
            printf("Unexpected type of bool\n");
            return -1;
    }
}

void createIfElseAsm(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {
    //push param on the stack
    startParsing(root->leftChild->leftChild, file, FUNCS);

    file << getAsmOp(getBoolType(int(root->leftChild->leftChild->data->value)));

    int jeCond = countOfIf++;
    file << " tifCond" << jeCond << "\n";
    int tmpPointMain = countOfIf++;

    //put if
    startParsing(root->leftChild->rightChild, file, FUNCS);
    file << "jmp tifMainCond" << tmpPointMain << "\n";


    //put else
    file << "tifCond" << jeCond << ":\n";
    startParsing(root->rightChild, file, FUNCS);
    file << "jmp" << " tifMainCond" << tmpPointMain << "\n";

    file << "tifMainCond"<< tmpPointMain << ":\n";

}

void createCallFuncAsm(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {
    //кладем текущий параметр на стек
    //только потом вызываем
    if(root->leftChild) {
        file << "push [" << ((**FUNCS)[currentFunc])->vars[0]->index << "]\n";
    }
    //put param on the stack
    startParsing(root->leftChild, file, FUNCS);
    root->data->name++;// _func
    file << "call" << " f" << (root->data->name) << "\n";
    root->data->name--;

}

void createIFASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {
    startParsing(root->leftChild, file, FUNCS);// try to find main first

    file << getAsmOp(getBoolType(int(root->leftChild->data->value)));

    int jeCond = countOfIf++;
    file << " tifCond" << jeCond << "\n";
    int tmpPointMain = countOfIf++;
    file << "jmp tifTempCond" << tmpPointMain << "\n";

    //describe if
    file << "tifCond" << jeCond << ":\n";
    startParsing(root->rightChild, file, FUNCS);
    file << "jmp" << " tifTempCond" << tmpPointMain << "\n";
    file << "tifTempCond"<< tmpPointMain << ":\n";
}


void createMathOpASM(Node* root, std::ofstream& file, int op, std::vector<FuncScope*>** FUNCS) {
    file << getAsmOp(op) << "\n";
}

void createOutputInputAsm(Node* root, std::ofstream& file, int op, std::vector<FuncScope *>** FUNCS) {
    if(root->rightChild->data->name) {
        int varIndexInRam = -1;
        int varIndexInVars = -1;
        for (int i = 0; i < ((**FUNCS)[currentFunc])->vars.size() ; ++i) {
            if(strcmp((**FUNCS)[currentFunc]->vars[i]->name, root->rightChild->data->name) == 0) {
                varIndexInRam = (**FUNCS)[currentFunc]->vars[i]->index;
                varIndexInVars = i;
                break;
            }
        }
        if(varIndexInRam == -1) {
            auto var = new VarIndex();
            var->index = ramFreeSpace++;
            var->name = root->data->name;
            (**FUNCS)[currentFunc]->vars.push_back(var);
        }
        if(op == ASM_IN) {
            file << getAsmOp(op) << "\n";
            file << "pop " << "[" << (**FUNCS)[currentFunc]->vars[varIndexInVars]->index << "]\n";
        } else {
            file << "push " << "[" << (**FUNCS)[currentFunc]->vars[varIndexInVars]->index << "]\n";
            file << getAsmOp(op) << "\n";
        }

    } else {
        startParsing(root->rightChild, file, FUNCS);
        file << getAsmOp(op) << "\n";
    }
}

void createReturnASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {
    if(currentFunc == 0) {
        file << "hlt \n";
        return;
    } else {
        // если есть возвр значение, то кладем его на стек
        if(root->leftChild) {
            startParsing(root->leftChild, file, FUNCS);
        }
        file << "ret \n";
        return;
    }
}

void createVariableASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {//toDo
    int varIndexInRAM = -1;
    int varIndexInVars = -1;
    for (int i = 0; i < ((**FUNCS)[currentFunc])->vars.size() ; ++i) {
        if(strcmp((**FUNCS)[currentFunc]->vars[i]->name, root->data->name) == 0) {
            varIndexInRAM = (**FUNCS)[currentFunc]->vars[i]->index;
            varIndexInVars = i;
            break;
        }
    }
    if(varIndexInRAM == -1 && varIndexInVars == -1) {
        auto var = new VarIndex();
        var->index = ramFreeSpace++;
        var->name = root->data->name;
        varIndexInVars = (**FUNCS)[currentFunc]->vars.size();
        (**FUNCS)[currentFunc]->vars.push_back(var);

    }
    if(root->rightChild) {
        if(root->rightChild->data->type == CLASS_NUMBER) {
            file << "push " << root->rightChild->data->value << "\n";
            file << "pop " << "[" << ((**FUNCS)[currentFunc])->vars[varIndexInVars]->index << "]\n";
        } else if(root->rightChild->data->type == CLASS_VARIABLE) {
            int indRAM = -1; //find another variable
            for (int i = 0; i < (**FUNCS)[currentFunc]->vars.size() ; ++i) {
                if(strcmp((**FUNCS)[currentFunc]->vars[i]->name, root->rightChild->data->name) == 0) {
                    indRAM = i;
                    break;
                }
            }
            if(indRAM == -1) {
                printf("Can't find this variable in scope %s\n", root->rightChild->data->name);
                exit(EXIT_FAILURE);
            } else {
                file << "push " << "[" << (**FUNCS)[currentFunc]->vars[indRAM]->index << "]\n";
                file << "pop " << "[" << (**FUNCS)[currentFunc]->vars[varIndexInVars]->index << "]\n";
            }
            return;
        } else {
            startParsing(root->rightChild, file, FUNCS);
            file << "pop " << "[" << (**FUNCS)[currentFunc]->vars[varIndexInVars]->index << "]\n";
        }
    } else {
        file << "push " << "[" << (**FUNCS)[currentFunc]->vars[varIndexInVars]->index << "]\n";
    }
}

void createFunctionASM(Node* root, std::ofstream& file, std::vector<FuncScope*>** FUNCS) {
    file.seekp(0, std::ios::end);// go the end of the file to describe function //toDo need more space
    file.seekp(file.tellp());
    file << "\nf" << root->data->name << ":\n";
    auto mainFunc = new FuncScope();
    mainFunc->funcName = root->data->name;
    (*FUNCS)->push_back(mainFunc);
    currentFunc = ++currentFunc;//change current function
    //add preambula if exist
    if(root->leftChild) {
        auto param = new VarIndex();
        param->name = root->leftChild->data->name;
        param->index = ramFreeSpace++;
        mainFunc->vars.push_back(param);
        file << "pop " << "[" << param->index << "]\n";
    }
}
