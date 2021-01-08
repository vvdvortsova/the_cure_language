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

Node* getA(std::vector<Token*>::iterator* iter);

Node* createVarNode(Token* varToken, Node* rval);

Node* createDefineVarNode(Token* pdef, Node* rVal);

Node* createUnarySystemOP(Node* rval, INTERNAL_OP op, int index);

Node* getD(std::vector<Token*>::iterator* iter);

Node* createPointNode(Node* lVal, Node* rVal, int countFuncs);

Node* getQ(std::vector<Token*>::iterator* iter);

Node* getS(std::vector<Token *>::iterator *iter);

Node* createOp(Node* rVal);

Node* getR(std::vector<Token*>::iterator* iter);

Node* getCondition(std::vector<Token*>::iterator* iter);

Node* getB(std::vector<Token*>::iterator* iter);

Node* createBoolOp(Node* lVal, Node* rVal, BOOL_OP op);

Node* getW(std::vector<Token*>::iterator* iter);

Node* getF(std::vector<Token*>::iterator* iter);

#endif //THE_CURE_LANGUAGE_FRONTEND_H
