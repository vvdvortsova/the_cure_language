#include <stack>
#include "lexer.h"
#include "frontend.h"


Node* buildTree(std::vector<Token*>* tokens) {
    auto iter = tokens->begin();
    Node* tree = getG(&iter);
    return tree;
}

Node* getG(std::vector<Token*>::iterator* iter) {
    Node* val = getE(iter);
    if(dynamic_cast<End*>(**iter) != nullptr) {
        return val;
    } else {
        //TODO syntx error
        return nullptr;
    }
}

Node* getE(std::vector<Token*>::iterator* iter) {
    Node* lval = getT(iter);
    while((**iter)->getTypeOP() == ADD || (**iter)->getTypeOP() == SUB ) {
        Token* op = **iter;
        (*iter)++;
        Node* rval = getT(iter);
        if((op)->getTypeOP() == ADD) {
            lval = createMathOpNode(lval, rval, ADD, (**iter)->getNumber());
        } else lval = createMathOpNode(lval, rval, SUB, (**iter)->getNumber());
    }
    return lval;
}

Node* createMathOpNode(Node* lNode, Node* rNode, MATH_OP_TYPE mathOpType, int index) {
    Data* data = new Data();
    data->type = CLASS_MATH_OP;
    data->value = mathOpType;

    Node* node =  new Node();

    node->index = index;
    node->leftChild = lNode;
    node->rightChild = rNode;
    node->data = data;
    return node;
}

Node* getT(std::vector<Token*>::iterator* iter) {
    Node* lval = getPOW(iter);
    while ((**iter)->getTypeOP() == MUL || (**iter)->getTypeOP() == DIV ) {
        Token* op = **iter;
        (*iter)++;
        Node* rval = getPOW(iter);
        if((op)->getTypeOP() == MUL) {
            lval = createMathOpNode(lval, rval, MUL, (**iter)->getNumber());
        } else
            lval = createMathOpNode(lval, rval, DIV, (**iter)->getNumber());
    }
    return lval;
}

Node* getPOW(std::vector<Token*>::iterator *pIterator) {
    Node* lval = getP(pIterator);
    while (dynamic_cast<Pow*>(**pIterator) != nullptr) {
        Token* op = **pIterator;
        (*pIterator)++;
        Node* rval = getP(pIterator);
        if(dynamic_cast<Pow*>(op) != nullptr) {
            lval = createMathOpNode(lval, rval, POW, (**pIterator)->getNumber());
        } else {
            //todo
            return nullptr;
        }
    }
    return lval;
}


Node* getP(std::vector<Token*>::iterator* iter) {
    if (dynamic_cast<Pair*>(**iter) != nullptr) {
        auto item = dynamic_cast<Pair*>(**iter);
        if(item->getTypePair() == OPEN) {
            (*iter)++;
            Node* lval = getE(iter);
            if(requirePair(iter)) {
                return lval;
            } else {
                //add message or exception and so on
                return nullptr;
            }
        }
    } else if(dynamic_cast<Num*>(**iter) != nullptr) {
        return getN(iter);
    } else {
        return getId(iter);
    }
    return nullptr;
}

Node* getId(std::vector<Token*>::iterator* iter) {
    if(dynamic_cast<Var*>(**iter) != nullptr) {
        Data* data = new Data();
        data->type = CLASS_VARIABLE;
        auto elem = dynamic_cast<Var*>(**iter);
        data->name = elem->getCh();
        Node* nodeVar =  new Node();
        nodeVar->leftChild = nullptr;
        nodeVar->rightChild = nullptr;
        nodeVar->index = elem->getNumber();
        nodeVar->data = data;
        (*iter)++;
        return nodeVar;
    } else if ((**iter)->getTypeOP() == SIN || (**iter)->getTypeOP() == COS || (**iter)->getTypeOP() == LOG) {
        Token* op = **iter;
        (*iter)++;
        if (dynamic_cast<Pair*>(**iter) != nullptr) {
            auto item = dynamic_cast<Pair*>(**iter);
            if(item->getTypePair() == OPEN) {
                (*iter)++;
                Node* rval = getE(iter);
                if(requirePair(iter)) {
                    if(op->getTypeOP() == COS) {
                        return createUnaryNodeMathFunc(rval, COS, (**iter)->getNumber());
                    } else  if (op->getTypeOP() == SIN){
                        return createUnaryNodeMathFunc(rval, SIN, (**iter)->getNumber());
                    } else {
                        return createUnaryNodeMathFunc(rval, LOG, (**iter)->getNumber());
                    }
                } else {
                    //add message or exception and so on
                    return nullptr;
                }
            }
        }
    }
    return nullptr;
}

Node* createUnaryNodeMathFunc(Node* val, MATH_OP_TYPE type, int number) {
    Data* data = new Data();
    data->type = CLASS_MATH_FUNC;
    data->value = type;

    Node* nodeVar =  new Node();
    nodeVar->leftChild = nullptr;
    nodeVar->rightChild = val;
    nodeVar->index = number;
    nodeVar->data = data;
    return nodeVar;
}

bool requirePair(std::vector<Token*>::iterator* iter) {
    if (dynamic_cast<Pair*>(**iter) != nullptr) {
        auto item = dynamic_cast<Pair*>(**iter);
        if(item->getTypePair() == CLOSE) {
            (*iter)++;
            return  true;
        }
    }
    return false;
}

Node* getN(std::vector<Token*>::iterator* iter) {
    if(dynamic_cast<Num*>(**iter) != nullptr) {


        Node* nodeNum = new Node();

        auto elem = dynamic_cast<Num*>(**iter);
        Data* data = new Data();
        data->type = CLASS_NUMBER;
        data->value = elem->getValue();

        nodeNum->leftChild = nullptr;
        nodeNum->rightChild = nullptr;
        nodeNum->index = elem->getNumber();
        nodeNum->data = data;
        (*iter)++;
        return nodeNum;
    }
    //ToDO add exception or so on
    return nullptr;
}



//toDo
double calculate(Node* tree) {
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
//        }
//    }
//    return tree->value;
}






