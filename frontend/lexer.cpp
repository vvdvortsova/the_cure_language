#include "lexer.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <cmath>
#include <iostream>

int getTypeOfMathOp(char* name, int len) {
    if(strncmp(name,"+",1) == 0) return ADD;
    if(strncmp(name,"-",1) == 0) return SUB;
    if(strncmp(name,"*",1) == 0) return MUL;
    if(strncmp(name,"/",1) == 0) return DIV;
    return -1;
}

char* getExpressionFromFile(char* fName, int* size) {
    assert(fName != NULL);

    FILE* file = NULL;
    file = fopen(fName, "r");
    if(!file) {
        fprintf(stderr,"Do not open = %s\n", fName);
        exit(EXIT_FAILURE);
    }

    // define size of file
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* arr = static_cast<char *>(calloc(*size, *size * sizeof(char)));
    if (arr) {
        int res = fread(arr, 1, *size, file);
        if(res != *size) {
            fprintf(stderr,"Do not read file to buffer = %s\n", fName);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
    return arr;
}

bool getDoubleNumber(char* mnemonicStart, double* number, char** endptr) {
    char* endPtr = NULL;
    //cast string to double
    *number = strtod(mnemonicStart, &endPtr);
    *endptr = endPtr;
    return endPtr != mnemonicStart;
}


// todo: put space before }$ to get success  - don't worry i fixed it somehow

/**
 * Simple Lexer
 * @param expr
 * @return tokens
 */
std::vector<Token*> doLexer(char* expr) {
    assert(expr != nullptr);
    int countOP = 0;
    std::vector<Token*> tokens;
    int offset = 0;
    char* token = strtok(expr, " ");
    double num = 0;
    char *next = nullptr;
    while( token != nullptr ) {
        printf( " %s\n", token );
        for (; token != nullptr && ((strncmp(token,"",1) != 0) && (strncmp(token,"\n",1) != 0)); ) {
            if (strncmp(token, "(", 1) == 0) {
                auto elem = new Pair(OPEN);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, ";", 1) == 0) {
                auto elem = new Point();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, "$", 1) == 0) {
                auto elem = new End();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, "=", 1) == 0) {
                auto elem = new AssignOP();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, "{", 1) == 0) {
                auto elem = new FigurePair(OPEN);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, "+", 1) == 0 || strncmp(token, "-", 1) == 0) {
                auto elem = new MathOp(1, getTypeOfMathOp(token, 1));
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, "/", 1) == 0 || strncmp(token, "*", 1) == 0) {
                auto elem = new MathOp(2, getTypeOfMathOp(token, 1));
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, "^", 1) == 0) {
                auto elem = new Pow();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, ")", 1) == 0) {
                auto elem = new Pair(CLOSE);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            } else if (strncmp(token, "}", 1) == 0) {
                auto elem = new FigurePair(CLOSE);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token++;
                else break;
            }
                //toDo add macro and len
            else if (checkMathOP(token, &offset) != -1) {
                Token *elem = nullptr;
                elem = new MathFunc(2, checkMathOP(token, &offset));//MATH
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token += offset;
                else break;
            } else if (checkBoolOp(token, &offset) != -1) { // BOOL_OP
                Token *elem = nullptr;
                elem = new BoolSign(checkBoolOp(token, &offset)); //toDO:you need to put space before bool sign! please
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token += offset;
                else break;
            } else if (checkSystemOp(token, &offset) != -1) { // SYSTEM_OP
                Token *elem = nullptr;
                elem = new SystemOP(2, checkSystemOp(token, &offset));

                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token += offset;
                else break;
            } else if(getDoubleNumber(token, &num, &next)){
                    auto elem = new Num(num);
                    elem->setNumber(countOP++);
                    tokens.push_back(elem);
                    if ((strncmp(next, "", 1) != 0) && (strncmp(next, "\n", 1) != 0)) token = next;
                    else token = nullptr;
            } else if (tokens[tokens.size() - 1]->getTypeOP() == DEF_VAR
            || tokens[tokens.size() - 1]->getTypeOP() == OPEN
            || dynamic_cast<BoolSign*>(tokens[tokens.size() - 1]) != nullptr
            || dynamic_cast<MathOp*>(tokens[tokens.size() - 1]) != nullptr
            || dynamic_cast<AssignOP*>(tokens[tokens.size() - 1]) != nullptr) { // VAR //toDO:you need to put space before variable! please
                Token *elem = new Var(token);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token += strlen(token);
                else break;
            } else if (tokens[tokens.size() - 1]->getTypeOP() == DEF_FUNC) { // FUNC //toDO:you need to put space before funcName! please
                Token *elem = new FuncName(token);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if (strlen(token) > 1)token += strlen(token);
                else break;
            } else {

                fprintf(stderr, "Syntax Error: Unexpected sign: %s!\n", token);
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(nullptr, " ");
    }

    //for debug
    std::vector<Token*>::iterator it;
    for (it = std::begin(tokens); it != std::end(tokens) ; ++it) {
        if (dynamic_cast<FigurePair*>(*it) != nullptr) {
            std::cout << "FigurePair{" << dynamic_cast<FigurePair*>(*it)->getTypePair() << "}\n";
        } else if (dynamic_cast<Pair*>(*it) != nullptr) {
            std::cout << "Pair(" << dynamic_cast<Pair*>(*it)->getTypePair() << ")\n";
        } else if (dynamic_cast<SystemOP*>(*it) != nullptr) {
            switch (dynamic_cast<SystemOP*>(*it)->getTypeOP()) {
                case IF_OP:
                    std::cout << "if\n";
                    break;
                case ELSE:
                    std::cout << "else\n";
                    break;
                case RETURN_OP:
                    std::cout << "return\n";
                    break;
                case WHILE_OP:
                    std::cout << "while\n";
                    break;
                case DEF_FUNC:
                    std::cout << "def func\n";
                    break;
                case PRINT_OP:
                    std::cout << "print\n";
                    break;
                case SCAN_OP:
                    std::cout << "scan\n";
                    break;
                case DEF_VAR:
                    std::cout << "def var\n";
                    break;
                case M:
                    std::cout << "main\n";
                    break;
                default:
                    std::cout << dynamic_cast<SystemOP*>(*it)->getTypeOP() <<"????\n";
                    break;
            }
        } else if (dynamic_cast<Var*>(*it) != nullptr) {
            std::cout << "Var(" << dynamic_cast<Var*>(*it)->getCh() << ")\n";
        } else if (dynamic_cast<MathFunc*>(*it) != nullptr) {
            if(dynamic_cast<MathFunc*>(*it)->getTypeOP() == COS)
                std::cout << "cos\n";
            else
                std::cout << "sin\n";
        } else if (dynamic_cast<MathOp*>(*it) != nullptr) {
            switch (dynamic_cast<MathOp*>(*it)->getTypeOP()) {
                case ADD:
                    std::cout << "add\n";
                    break;
                case SUB:
                    std::cout << "sub\n";
                    break;
                case MUL:
                    std::cout << "mul\n";
                    break;
                case DIV:
                    std::cout << "div\n";
                    break;
                default:
                    std::cout << "????\n";
                    break;
            }
        } else if (dynamic_cast<Num*>(*it) != nullptr) {
            std::cout << "Num(" << dynamic_cast<Num*>(*it)->getValue() << ")\n";
        } else if (dynamic_cast<FuncName*>(*it) != nullptr) {
            std::cout << "func " << dynamic_cast<FuncName*>(*it)->getCh() << "\n";
        } else if (dynamic_cast<BoolSign*>(*it) != nullptr) {
            switch (dynamic_cast<BoolSign*>(*it)->getTypeOP()) {
                case MORE:
                    std::cout << ">\n";
                    break;
                case LESS:
                    std::cout << "<\n";
                    break;
                case EQUAL:
                    std::cout << "==\n";
                    break;
                default:
                    std::cout << "????\n";
                    break;
            }
        } else if (dynamic_cast<End*>(*it) != nullptr) {
            std::cout << "end\n";
        } else if (dynamic_cast<Point*>(*it) != nullptr) {
            std::cout << "point;\n";
        } else if (dynamic_cast<AssignOP*>(*it) != nullptr) {
            std::cout << "=\n";
        }
    }

    return tokens;
}

int checkBoolOp(char* op, int* offset) {
#define BOOL_OP_SYS
#define BOOL_OP_FUNC(name, number, length) {\
            if (strncmp(#name, op, length) == 0){\
                *offset = length;\
                return number;}}

#include "chooseTheCure.h"

#undef BOOL_OP_SYS
    *offset = 0;
    return -1;
}


int checkSystemOp(char* op, int* offset) {
#define SYSTEM
#define SYSTEM_OP(name, number, length) {\
            if (strncmp(#name, op, length) == 0){\
                *offset = length;\
                return number;}}

#include "chooseTheCure.h"

#undef SYSTEM
    *offset = 0;
    return -1;
}

int checkMathOP(char* mathOp, int* offset) {
#define MATH
#define OP_MATH_FUNC(name, type, length) {\
            if (strncmp(#name, mathOp, length) == 0){\
                *offset = length;\
                return type;}}

#include "chooseTheCure.h"

#undef MATH
    *offset = 0;
    return -1;
}