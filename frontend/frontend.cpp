#include <stack>
#include "lexer.h"
#include "frontend.h"

int countFuncDef = 0;

Node *getF(std::vector<Token *>::iterator *iter);

Node* buildTree(std::vector<Token*>* tokens) {
    auto iter = tokens->begin();
    Node* tree = getG(&iter);
    return tree;
}

Node* getG(std::vector<Token*>::iterator* iter) {
//    Node* lVal = getE(iter);
    int countFuncs = 0;
    Node* lVal = getD(iter);
    while((*(*iter + 1))->getTypeOP() != CLASS_END) {
        (*iter)++;
        Node* rVal = getD(iter);
        lVal = createPointNode(lVal, rVal, countFuncs);
        countFuncs++;
    }
    (*iter)++;

    if(dynamic_cast<End*>(**iter) != nullptr) {
        return lVal;
    } else {
        //TODO syntx error
        return nullptr;
    }
}

Node* createPointNode(Node* lVal, Node* rVal, int countFuncs) {
    //Point node
    Node* pointNode = new Node();
    Data* pointData = new Data();
    pointData->type = CLASS_POINT;

    pointNode->leftChild = lVal;
    pointNode->rightChild = rVal;
    pointNode->data = pointData;
    pointNode->index = countFuncs;

    return pointNode;
}

Node* getD(std::vector<Token *>::iterator* iter) {
    //def_func_Node
    if((**iter)->getTypeOP() == DEF_FUNC) {
        (*iter)++;
        Node* funcNode = new Node();
        Data* funcData = new Data();
        funcData->type = CLASS_FUNC_NAME;
        if((**iter)->getTypeOP() == M) { // if main function
            funcData->value = M;
        }
        else if (dynamic_cast<FuncName*>(**iter) != nullptr) {
            funcData->name = dynamic_cast<FuncName*>(**iter)->getCh();
        }
        funcNode->data = funcData;
        //look at the params
        (*iter)++;
        if((**iter)->getTypeOP() == OPEN) {
            (*iter)++;
        } else {
            printf("Unexpected system op during the recursive parser (1)\n");
            return nullptr;
        }
        Node* param = nullptr;
        if ((**iter)->getTypeOP() == CLASS_VARIABLE) { //our param, now only one
            param = new Node();
            Data* paramData = new Data();
            paramData->type = CLASS_VARIABLE;
            auto elem = dynamic_cast<Var*>(**iter);
            paramData->name = elem->getCh();
            param->data = paramData;
            param->index = (**iter)->getNumber();
            (*iter)++;// miss close ), now it is {
        }
        (*iter)++;//{
        (*iter)++;// some E
        // get a function body, list with op
        Node* body = getQ(iter);


        funcNode->leftChild = param;
        funcNode->rightChild = body;
        funcNode->index = countFuncDef++;

        if(requirePair(iter)) { //Close - read params
            Node* defNode = new Node();
            Data* defData = new Data();
            defData->type = DEF_FUNC;
            defNode->leftChild = funcNode;
            defNode->index = countFuncDef++;
            defNode->data = defData;
            return defNode;
        } else {
            printf("Unexpected system op during the recursive parser(2)\n");
            return nullptr;
        }

    } else {
        printf("Unexpected system op during the recursive parser(3)\n");
        return nullptr;
    }
}

Node* getQ(std::vector<Token *>::iterator* iter) {
    printf("recursive parser(4)%d\n",(**iter)->getTypeOP());
    Node* firstCommand = getS(iter);
    auto tmp = firstCommand;
    Node* nextCommand = nullptr;
    printf("recursive parser(5)%d\n",(**iter)->getTypeOP());
    while((**iter)->getTypeOP() == CLASS_POINT) {
        (*iter)++;
        if (dynamic_cast<FigurePair *>(**iter) != nullptr)
            break;
        printf("recursive parser(6)%d\n",(**iter)->getTypeOP());
        nextCommand = getS(iter);
        printf("recursive parser(7)%d\n",(**iter)->getTypeOP());
        firstCommand->rightChild = nextCommand;
        firstCommand = nextCommand;
    }
    printf("recursive parser(8)%d\n",firstCommand->data->type);
    return tmp;
}

Node* getS(std::vector<Token *>::iterator* iter) {
    Node* rVal = nullptr;
    if ((**iter)->getTypeOP() == RETURN_OP) {
        rVal = getR(iter);
    } else if((**iter)->getTypeOP() == IF_OP) {
        rVal = getCondition(iter);
    }
    else if((**iter)->getTypeOP() == WHILE_OP) {
        rVal = getW(iter);
    }
    else {
        rVal = getE(iter);
    }

    rVal = createOp(rVal);
    return rVal;
}

Node* getW(std::vector<Token*>::iterator* iter) {
    Node* nodeWhile = new Node();
    Data* dataWhile = new Data();
    dataWhile->type = CLASS_SYSTEM_OP;
    dataWhile->value = WHILE_OP;
    nodeWhile->data = dataWhile;
    nodeWhile->index = (**iter)->getNumber();
    (*iter)++;//miss while token
    if(!requirePair(iter)) {
        printf("if-block - requires open \"(\" \n");
        return nullptr;
    }
    Node* conditionWhile = getB(iter);
    if(!requirePair(iter)) {
        printf("if-block - requires close \")\" \n");
        return nullptr;
    } if(!requirePair(iter)) {
        printf("if-block - requires open \"{\" \n");
        return nullptr;
    }
    Node* bodyWhile = getQ(iter);
    if(!requirePair(iter)) {
        printf("if-block - requires close \"}\" \n");
        return nullptr;
    }
    nodeWhile->leftChild = conditionWhile;
    nodeWhile->rightChild = bodyWhile;
    return nodeWhile;
}

Node* getCondition(std::vector<Token*>::iterator* iter) {
    //try to understand it is just if block or if-else
    //make if block
    Node* ifNode = new Node();
    Data* ifData = new Data();
    ifData->type = CLASS_SYSTEM_OP;
    ifData->value = IF_OP;
    ifNode->data = ifData;
    ifNode->index = (**iter)->getNumber() + 1;
    (*iter)++; // miss if
    if(!requirePair(iter)) {
        printf("if-block - requires open \"(\" \n");
        return nullptr;
    }
    Node* ifParam = getB(iter);
    if(!requirePair(iter)) {
        printf("if-block - requires close \")\" \n");
        return nullptr;
    } if(!requirePair(iter)) {
        printf("if-block - requires open \"{\" \n");
        return nullptr;
    }

    Node* ifBody = getQ(iter);
    if(!requirePair(iter)) {
        printf("if-block - requires close \"}\" \n");
        return nullptr;
    }

    ifNode->leftChild = ifParam;
    ifNode->rightChild = ifBody;

    if((**iter)->getTypeOP() == ELSE) {
        Node* elseNode = new Node();
        Data* elseData = new Data();

        elseData->type = CLASS_SYSTEM_OP;
        elseData->value = ELSE;
        elseNode->data = elseData;
        elseNode->index = countFuncDef++;
        (*iter)++;//miss else token
        if(!requirePair(iter)) {
            printf("if-block - requires open \"{\" \n");
            return nullptr;
        }
        Node* elseBody = getQ(iter);
        if(!requirePair(iter)) {
            printf("if-block - requires close \"}\" \n");
            return nullptr;
        }
        elseNode->leftChild = elseBody;

        //create if-else Node
        Node* ifElseNode = new Node();
        Data* ifElseData = new Data();
        ifElseData->type = CLASS_SYSTEM_OP;
        ifElseData->value = IF_ELSE_OP;
        ifElseNode->data = ifElseData;
        ifElseNode->leftChild = ifNode;
        ifElseNode->rightChild = elseNode;
        ifElseNode->index = countFuncDef++;
        return ifElseNode;
    }
    return ifNode;
}

Node* getB(std::vector<Token*>::iterator* iter) {
    Node* lVal = getE(iter);
    //sign
    auto signToken = (**iter)->getTypeOP();
    (*iter)++;
    Node* rVal = getE(iter);

    if(signToken == MORE) {
        lVal = createBoolOp(lVal, rVal, MORE);
    } else if (signToken == LESS) {
        lVal = createBoolOp(lVal, rVal, LESS);
    } else {
        lVal = createBoolOp(lVal, rVal, EQUAL);
    }
    return lVal;
}

Node* createBoolOp(Node* lVal, Node* rVal, BOOL_OP op) {
    Node* boolNode = new Node();
    Data* boolData = new Data();
    boolData->type = CLASS_BOOL_SIGN;
    boolData->value = op;
    boolNode->data = boolData;
    boolNode->leftChild = lVal;
    boolNode->rightChild = rVal;
    boolNode->index = 5 + countFuncDef++;
    return boolNode;
}

Node* getR(std::vector<Token*>::iterator* iter) {
    Node* returnNode = new Node();
    Data* dataReturn = new Data();
    dataReturn->type = CLASS_SYSTEM_OP;
    dataReturn->value = RETURN_OP;
    returnNode->data = dataReturn;
    returnNode->index = countFuncDef++;
    (*iter)++;
    Node* value = getE(iter);
    returnNode->leftChild = value;
    return returnNode;
}

Node* createOp(Node *rVal) {
    Node* node = new Node();
    Data* data = new Data();
    data->type = CLASS_OP;
//    data->value = CLASS_OP;
    node->leftChild = rVal;
    node->data = data;
    node->index = countFuncDef += 2;
    return node;
}

Node* getE(std::vector<Token*>::iterator* iter) {
    Node* lval = nullptr;
    if((**iter)->getTypeOP() == DEF_VAR) {
        Token* op = **iter;
        (*iter)++;
        Node* rval = getA(iter);
        return createDefineVarNode(op, rval);
    } else {
        lval = getT(iter);
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
}

Node* createDefineVarNode(Token *varToken, Node *rval) {
    //Node for def_var
    Data* data = new Data();
    data->type = CLASS_SYSTEM_OP;
    data->value = DEF_VAR;
    auto elem = dynamic_cast<SystemOP*>(varToken);
    Node* varNode =  new Node();
    varNode->index = elem->getNumber();
    varNode->leftChild = nullptr;
    //add var
    varNode->rightChild = rval;
    varNode->data = data;
    return varNode;
}

Node* getA(std::vector<Token *>::iterator *iter) {
    Token* var = **iter;
    (*iter) += 2;// var and =
    Node* rval = getE(iter);
    return createVarNode(var, rval);
}

Node *createVarNode(Token *varToken, Node *rval) {
    //Node for var
    Data* data = new Data();
    data->type = CLASS_VARIABLE;
//    data->value = DEF_VAR;
    auto elem = dynamic_cast<Var*>(varToken);
    data->name = elem->getCh();
    Node* varNode =  new Node();
    varNode->index = elem->getNumber();
    varNode->leftChild = nullptr;
    //add value
    varNode->rightChild = rval;
    varNode->data = data;
    return varNode;
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
    } else if(dynamic_cast<CallFunc*>(**iter) != nullptr) {
        return getF(iter);
    }else {
        return getId(iter);
    }
    return nullptr;
}

Node* getF(std::vector<Token*>::iterator* iter) {
    Node* nodeCallF = new Node();
    nodeCallF->index = (**iter)->getNumber();
    Data* callData = new Data();
    callData->type = CLASS_CALL_FUNC;
    callData->name = dynamic_cast<CallFunc*>(**iter)->getCh();
    nodeCallF->data = callData;
    (*iter)++;//miss call name
    if(!requirePair(iter)) {
        printf("callExpr - requires open \"(\" \n");
        return nullptr;
    }
    Node* nodeCallBody = getE(iter);
    if(!requirePair(iter)) {
        printf("callExpr - requires close \")\" \n");
        return nullptr;
    }
    nodeCallF->leftChild = nodeCallBody;
    return nodeCallF;
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
    } else if ((**iter)->getTypeOP() == PRINT_OP || (**iter)->getTypeOP() == SCAN_OP) {
        Token* op = **iter;
        (*iter)++;
        if (dynamic_cast<Pair*>(**iter) != nullptr) {
            auto item = dynamic_cast<Pair*>(**iter);
            if(item->getTypePair() == OPEN) {
                (*iter)++;
                // this is var or num
                Node* rval = nullptr;
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
                } else {
                    rval = getN(iter);
                }
                if(requirePair(iter)) {
                    if(op->getTypeOP() == PRINT_OP) {
                        return createUnarySystemOP(rval, PRINT_OP, (**iter)->getNumber());
                    } else  if (op->getTypeOP() == SCAN_OP){
                        return createUnarySystemOP(rval, SCAN_OP, (**iter)->getNumber());
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

Node *createUnarySystemOP(Node *rval, INTERNAL_OP op, int index) {
    Data* data = new Data();
    data->type = CLASS_SYSTEM_OP;
    data->value = op;

    Node* nodeVar =  new Node();
    nodeVar->leftChild = nullptr;
    nodeVar->rightChild = rval;
    nodeVar->index = index;
    nodeVar->data = data;
    return nodeVar;
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
        if(item->getTypePair() == CLOSE || item->getTypePair() == OPEN) {
            (*iter)++;
            return  true;
        }
    } else if (dynamic_cast<FigurePair*>(**iter) != nullptr) {
        auto item = dynamic_cast<FigurePair*>(**iter);
        if(item->getTypePair() == CLOSE || item->getTypePair() == OPEN) {
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






