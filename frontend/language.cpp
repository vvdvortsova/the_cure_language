#include "language.h"

const char* getStringMathOpAndFunc(int num) {
    const char* op = nullptr;
    switch (num) {
        case ADD:
            op = "+";
            break;
        case SUB:
            op = "-";
            break;
        case MUL:
            op = "*";
            break;
        case DIV:
            op = "/";
            break;
        case POW:
            op = "^";
            break;
        case SIN:
            op = "sin";
            break;
        case COS:
            op = "cos";
            break;
        case LOG:
            op = "log";
            break;
        default:
            op = nullptr;
            break;
    }
    return op;
}

const char* getStringBoolOp(int num) {
    const char* op = nullptr;
    switch (num) {
        case MORE:
            op = ">";
            break;
        case LESS:
            op = "<";
            break;
        case EQUAL:
            op = "=";
            break;
        default:
            op = nullptr;
            break;
    }
    return op;
}

const char* getStringSystemOp(int num) {
    const char* op = nullptr;
    switch (num) {
        case IF_OP:
            op = "IF";
            break;
        case IF_ELSE_OP:
            op = "IF_ELSE";
            break;
        case WHILE_OP:
            op = "while";
            break;
        case RETURN_OP:
            op = "return";
            break;
        case PRINT_OP:
            op = "print";
            break;
        case SCAN_OP:
            op = "scan";
            break;
        case DEF_FUNC:
            op = "def_func";
            break;
        case DEF_VAR:
            op = "def_var";
            break;
        case M:
            op = "main";
            break;
        case ELSE:
            op = "else";
            break;
        default:
            op = nullptr;
            break;
    }
    return op;
}
