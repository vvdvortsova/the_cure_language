//#include <stack>
//#include <cmath>
//#include "language.h"
//#include "lexer.h"
//
//const char* getNameOfOp(MATH_OP_TYPE type) {
//    switch(type) {
//        case ADD:
//            return "+";
//        case SUB:
//            return "-";
//        case DIV:
//            return "/";
//        case MUL:
//            return "*";
//        case POW:
//            return "^";
//        case X:
//            return "x";
//        case Y:
//            return "y";
//        case SIN:
//            return "sin";
//        case COS:
//            return "cos";
//        case NUM:
//            return nullptr;
//    }
//    return nullptr;
//}
//
//Node* buildTree(std::vector<Token*>* tokens) {
//    auto iter = tokens->begin();
//    Node* tree = getG(&iter);
//    return tree;
//}
//
//Node* getG(std::vector<Token*>::iterator* iter) {
//    Node* val = getE(iter);
//    if(dynamic_cast<End*>(**iter) != nullptr) {
//        return val;
//    } else {
//        //TODO syntx error
//        return nullptr;
//    }
//}
//
//Node* getE(std::vector<Token*>::iterator* iter) {
//    Node* lval = getT(iter);
//    while(dynamic_cast<Add*>(**iter) != nullptr || dynamic_cast<Sub*>(**iter) != nullptr) {
//        Token* op = **iter;
//        (*iter)++;
//        Node* rval = getT(iter);
//        if(dynamic_cast<Add*>(op) != nullptr) {
//            lval = createNode(lval, rval, ADD,(**iter)->getNumber());
//        } else lval = createNode(lval, rval, SUB, (**iter)->getNumber());
//    }
//    return lval;
//}
//
//Node* createNode(Node* lNode, Node* rNode, MATH_OP_TYPE type, int index) {
//    Node* node =  new Node();
//    node->type = type;
//    node->value = 0;
//    node->index = index;
//    node->leftChild = lNode;
//    node->rightChild = rNode;
//    return node;
//}
//
//Node* getT(std::vector<Token*>::iterator* iter) {
//    Node* lval = getPOW(iter);
//    while (dynamic_cast<Mul*>(**iter) != nullptr || dynamic_cast<Div*>(**iter) != nullptr) {
//        Token* op = **iter;
//        (*iter)++;
//        Node* rval = getPOW(iter);
//        if(dynamic_cast<Mul*>(op) != nullptr) {
//            lval = createNode(lval, rval, MUL, (**iter)->getNumber());
//        } else
//            lval = createNode(lval, rval, DIV, (**iter)->getNumber());
//    }
//    return lval;
//}
//
//Node* getPOW(std::vector<Token*>::iterator *pIterator) {
//    Node* lval = getP(pIterator);
//    while (dynamic_cast<Pow*>(**pIterator) != nullptr) {
//        Token* op = **pIterator;
//        (*pIterator)++;
//        Node* rval = getP(pIterator);
//        if(dynamic_cast<Pow*>(op) != nullptr) {
//            lval = createNode(lval, rval, POW, (**pIterator)->getNumber());
//        } else {
//            //todo
//            return nullptr;
//        }
//    }
//    return lval;
//}
//
//
//Node* getP(std::vector<Token*>::iterator* iter) {
//    if (dynamic_cast<Pair*>(**iter) != nullptr) {
//        auto item = dynamic_cast<Pair*>(**iter);
//        if(item->getTypePair() == OPEN) {
//            (*iter)++;
//            Node* lval = getE(iter);
//            if(requirePair(iter)) {
//                return lval;
//            } else {
//                //add message or exception and so on
//                return nullptr;
//            }
//        }
//    } else if(dynamic_cast<Num*>(**iter) != nullptr) {
//        return getN(iter);
//    } else {
//        return getId(iter);
//    }
//    return nullptr;
//}
//
//Node* getId(std::vector<Token*>::iterator* iter) {
//    if(dynamic_cast<Var*>(**iter) != nullptr) {
//        Node* nodeVar =  new Node();
//        nodeVar->type = VAR;
//        auto elem = dynamic_cast<Var*>(**iter);
//        nodeVar->chVar = elem->getCh();
//        nodeVar->leftChild = nullptr;
//        nodeVar->rightChild = nullptr;
//        nodeVar->index = elem->getNumber();
//        (*iter)++;
//        return nodeVar;
//    } else if (dynamic_cast<Cos*>(**iter) != nullptr || dynamic_cast<Sin*>(**iter) != nullptr) {
//        Token* op = **iter;
//        (*iter)++;
//        if (dynamic_cast<Pair*>(**iter) != nullptr) {
//            auto item = dynamic_cast<Pair*>(**iter);
//            if(item->getTypePair() == OPEN) {
//                (*iter)++;
//                Node* rval = getE(iter);
//                if(requirePair(iter)) {
//                    if(dynamic_cast<Cos*>(op) != nullptr) {
//                        return createUnaryNode(rval, COS, (**iter)->getNumber());
//                    } else {
//                        return createUnaryNode(rval, SIN, (**iter)->getNumber());
//                    }
//                } else {
//                    //add message or exception and so on
//                    return nullptr;
//                }
//            }
//        }
//    }
//    return nullptr;
//}
//
//Node* createUnaryNode(Node* val, MATH_OP_TYPE type, int number) {
//    Node* nodeVar =  new Node();
//    nodeVar->type = type;
//    nodeVar->leftChild = nullptr;
//    nodeVar->rightChild = val;
//    nodeVar->index = number;
//    return nodeVar;
//}
//
//bool requirePair(std::vector<Token*>::iterator* iter) {
//    if (dynamic_cast<Pair*>(**iter) != nullptr) {
//        auto item = dynamic_cast<Pair*>(**iter);
//        if(item->getTypePair() == CLOSE) {
//            (*iter)++;
//            return  true;
//        }
//    }
//    return false;
//}
//
//Node* getN(std::vector<Token*>::iterator* iter) {
//    if(dynamic_cast<Num*>(**iter) != nullptr) {
//        Node* nodeNum = new Node();
//        nodeNum->type = NUM;
//        auto elem = dynamic_cast<Num*>(**iter);
//        nodeNum->value =  elem->getValue();
//        nodeNum->leftChild = nullptr;
//        nodeNum->rightChild = nullptr;
//        nodeNum->index = elem->getNumber();
//        (*iter)++;
//        return nodeNum;
//    }
//    //ToDO add exception or so on
//    return nullptr;
//}
//
//
//
//
//double calculate(Node* tree) {
//    if(tree == nullptr)
//        return 0;
//    if(tree->type == COS) {
//        double b = calculate(tree->rightChild);
//        return cos(b);
//    } else if(tree->type == SIN) {
//        double b = calculate(tree->rightChild);
//        return sin(b);
//    }
//    if(tree->type != NUM) {
//        double a = calculate(tree->leftChild);
//        double b = calculate(tree->rightChild);
//        switch (tree->type) {
//            case ADD:
//                return a + b;
//            case MUL:
//                return a * b;
//            case SUB:
//                return a - b;
//            case DIV:
//                return a / b;
//            case POW:
//                return pow(a,b);
//            case NUM:
//                break;
//        }
//    }
//    return tree->value;
//}
//
//
//
//
//
//
