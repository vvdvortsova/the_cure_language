#ifndef THE_CURE_LANGUAGE_FRONTEND_H
#define THE_CURE_LANGUAGE_FRONTEND_H

#include "lexer.h"
/**
 * I use recursive descent algo
 * @param tokens
 * @return
 */
Node* buildTree(std::vector<Token*>* tokens);

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

Node* getA(std::vector<Token *>::iterator *iter);

Node* createVarNode(Token *varToken, Node *rval);

Node* createDefineVarNode(Token *pdef, Node *rVal);

Node* createUnarySystemOP(Node *rval, INTERNAL_OP op, int index);

#endif //THE_CURE_LANGUAGE_FRONTEND_H
