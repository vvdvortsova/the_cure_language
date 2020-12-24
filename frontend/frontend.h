#ifndef THE_CURE_LANGUAGE_FRONTEND_H
#define THE_CURE_LANGUAGE_FRONTEND_H

#include "lexer.h"
/**
 * I use recursive descent algo
 * @param tokens
 * @return
 */
Node* buildTree(std::vector<Token*>* tokens);

const char* getNameOfOp(MATH_OP_TYPE type);

/**
 * Calculate the value of expression using tree
 * @param tree
 * @return
 */
double calculate(Node* tree);

Node* getG(std::vector<Token*>::iterator* iterator);

Node* getE(std::vector<Token*>::iterator* iterator);

Node* getT(std::vector<Token*>::iterator* iterator);

Node* createMathOpNode(Node* lNode, Node* rNode, MATH_OP_TYPE mathOpType, int index);

Node* getP(std::vector<Token*>::iterator* iterator);

Node* getN(std::vector<Token*>::iterator* iterator);

bool requirePair(std::vector<Token*>::iterator* iterator);

Node* getPOW(std::vector<Token*>::iterator* pIterator);

Node* getId(std::vector<Token*>::iterator* pIterator);

Node* createUnaryNodeMathFunc(Node* val, MATH_OP_TYPE type, int number);


#endif //THE_CURE_LANGUAGE_FRONTEND_H
