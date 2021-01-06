#ifndef THE_CURE_LANGUAGE_LEXER_H
#define THE_CURE_LANGUAGE_LEXER_H

#include "language.h"
#include <cstring>
/**
 * Super class for all tokens
 */
class Token {
    int num;
    int priority = 0;
    int type;
public:
    explicit Token(int _priority, int _type):priority(_priority), type(_type){};
    virtual ~Token() = default;
    void setNumber(int val){ this->num = val;}
    int getNumber() const { return num;}
    int getPriority() const { return priority;};
    int getTypeOP() const { return type;};
};

class Num: public Token {
    double value;
public:
    explicit Num(double val): Token(0, CLASS_NUMBER), value(val){}
    double getValue() const { return value;}
};

class Var:     public Token {
    char* ch;
public:
    explicit Var(char* _ch): Token(0, CLASS_VARIABLE), ch(_ch){}
    char* getCh() const { return ch;}
};

/**
 * cos, sin
 */
class MathFunc: public Token {
public:
    explicit MathFunc(int priority, int _type): Token(priority, _type){};
};

/**
 * +-/*
 */
class MathOp: public Token {
public:
    explicit MathOp(int priority, int _type): Token(priority, _type){};
};

/**
 * IF, ELSE, RETURN, WHILE, DEF_FUNC
 */
class SystemOP: public Token {
public:
    explicit SystemOP(int priority, int _type): Token(priority, _type){};
};

class Pow:     public Token {
public:
    explicit Pow(): Token(3, POW){}
};

class Pair:    public Token {
    OP_PAIR pType;
public:
    Pair(OP_PAIR _type): Token(0, _type), pType(_type){}
    OP_PAIR getTypePair() const { return pType;}
};

class FigurePair:    public Token {
    OP_PAIR pType;
public:
    FigurePair(OP_PAIR _type): Token(0, _type), pType(_type){}
    OP_PAIR getTypePair() const { return pType;}
};


class FuncName: public Token {
    char* ch;
public:
    explicit FuncName(char* _ch): Token(0, CLASS_FUNC_NAME), ch(_ch){}
    char* getCh() const { return ch;}
};

class CallFunc: public Token {
    char* ch;
public:
    explicit CallFunc(char* _ch): Token(0, CLASS_CALL_FUNC), ch(_ch){}
    char* getCh() const { return ch;}
};

class BoolSign: public Token {
public:
    explicit BoolSign(int _type): Token(2, _type){}
};

class End:     public Token {
public:
    explicit  End(): Token(0, CLASS_END){}
};

class Point:     public Token {
public:
    explicit  Point(): Token(0, CLASS_POINT){}
};

class AssignOP: public Token {
public:
    explicit  AssignOP(): Token(0, CLASS_ASSIGN){}
};

/**
 * Get tokens from exps
 * @param expr
 * @return
 */
std::vector<Token*> doLexer(char* expr);

char* getExpressionFromFile(char* fName, int* size);
int checkSystemOp(char* op, int* offset);
int checkMathOP(char* mathOp, int* offset);
int checkBoolOp(char* op, int* offset);

#endif //THE_CURE_LANGUAGE_LEXER_H
