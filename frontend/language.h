#ifndef THE_CURE_LANGUAGE_LANGUAGE_H
#define THE_CURE_LANGUAGE_LANGUAGE_H


#include <vector>
#include <fstream>
#include  "lexer.h"

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
    NUMBER = 100,
    VARIABLE = 101, // x y or so on
    SYSTEM_OPS = 102, // if else while
    MATH_OPS = 103, // + - / * ^
    MATH_FUNCS = 104, // cos sin
    BOOL_SIGNS = 105, // < > ==
    FUNC_NAMES = 106,
    END = 228,
    POINT = 229,
    ASSIGN = 300
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

/**
 * struct is describing of the node of the ast tree
 */
typedef struct Node {
    MATH_OP_TYPE type;
    int data_type;
    char chVar;
    double value;
    double index;
    Node* leftChild;
    Node* rightChild;
}Node;

//
///**
// * I use recursive descent algo
// * @param tokens
// * @return
// */
//Node* buildTree(std::vector<Token*>* tokens);
//
//const char* getNameOfOp(MATH_OP_TYPE type);
//
///**
// * Calculate the value of expression using tree
// * @param tree
// * @return
// */
//double calculate(Node* tree);
//
//Node* getG(std::vector<Token*>::iterator* iterator);
//
//Node* getE(std::vector<Token*>::iterator* iterator);
//
//Node* getT(std::vector<Token*>::iterator* iterator);
//
//Node* createNode(Node* lNode, Node* rNode, MATH_OP_TYPE type, int index);
//
//Node* getP(std::vector<Token*>::iterator* iterator);
//
//Node* getN(std::vector<Token*>::iterator* iterator);
//
//bool requirePair(std::vector<Token*>::iterator* iterator);
//
//Node* getPOW(std::vector<Token*>::iterator* pIterator);
//
//Node* getId(std::vector<Token*>::iterator* pIterator);
//
//Node* createUnaryNode(Node* pNode, MATH_OP_TYPE type, int number);


#endif //THE_CURE_LANGUAGE_LANGUAGE_H
