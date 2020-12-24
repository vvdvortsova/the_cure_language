#ifndef THE_CURE_LANGUAGE_LANGUAGE_H
#define THE_CURE_LANGUAGE_LANGUAGE_H


#include <vector>
#include <fstream>

enum MATH_OP_TYPE {
    ADD = 0,
    SUB = 1,
    DIV = 2,
    MUL = 3,
    POW = 4,
    SIN = 7,
    COS = 8,
    LOG = 11
};

enum BOOL_OP {
    MORE = 21,
    LESS = 22,
    EQUAL = 23
};

enum TYPE {
    CLASS_NUMBER = 100,
    CLASS_VARIABLE = 101, // x y or so on
    CLASS_SYSTEM_OP = 102, // if else while
    CLASS_MATH_OP = 103, // + - / * ^
    CLASS_MATH_FUNC = 104, // cos sin
    CLASS_BOOL_SIGN = 105, // < > ==
    CLASS_FUNC_NAME = 106,
    CLASS_END = 228,
    CLASS_POINT = 229,
    CLASS_ASSIGN = 300
};

enum INTERNAL_OP {
    IF_OP = 200,
    IF_ELSE_OP = 201,
    WHILE_OP = 202,
    RETURN_OP = 203,
    PRINT_OP = 204,
    SCAN_OP = 205,
    DEF_FUNC = 206,
    DEF_VAR = 207,
    M = 208, // main function
    ELSE = 209
};


enum OP_PAIR {
    OPEN,
    CLOSE
};

struct Data {
    int type = -1; //GLOBAL TYPE
    double value = -1;// INTERNAL_OP, BOOL_OP, MATH_OP_TYPE OR VALUE
    char* name = nullptr; //name of function or variable
};

/**
 * struct is describing of the node of the ast tree
 */
typedef struct Node {
    double index;
    Node* leftChild;
    Node* rightChild;
    Data* data;
}Node;

#endif //THE_CURE_LANGUAGE_LANGUAGE_H
