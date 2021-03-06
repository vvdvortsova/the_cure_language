#include <cassert>
#include <cmath>
#include "dumps.h"



#define  EQUATION_EPS 1e-9

void writeChild(std::ofstream &ofstream, Node *pNode);

/**
* @brief      Compare a double variable with zero
* @param[in]  a  double variable
* @return     0 if a == 0 else 1
*/
static bool isZero(double a) {
    return fabs(a) < EQUATION_EPS;
}

void gravizDump(char* outPath, Node* tree) {
    assert(outPath);
    assert(tree);
    int index = 0;
    std::ofstream myfile;
    myfile.open (outPath);
    myfile << "graph astdump {\n";
    gravizDeepWriting(myfile, tree, &index);
    myfile << "}\n";

    myfile.close();

}

void gravizDeepWriting(std::ofstream& myfile, Node* tree, int *index) {
    if(tree == nullptr)
        return;
    switch (tree->data->type) {
        case CLASS_NUMBER:
            return;
        case CLASS_VARIABLE:
            if(tree->rightChild) {
                myfile << "_" << tree->index << "V" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case CLASS_MATH_FUNC:
            myfile << "_" << tree->index << "MF" << tree->data->value << "[shape=box, color=red,label=\"" << getStringMathOpAndFunc(tree->data->value) << "\"]\n";
            if(tree->rightChild) {
                myfile << "_" << tree->index << "MF" << tree->data->value << " -- ";
                writeChild(myfile, tree->rightChild);
                // no left child - only right
            }
            break;
        case CLASS_MATH_OP:
            myfile << "_" << tree->index << "MO" << tree->data->value << "[shape=box, color=blue,label=\"" << getStringMathOpAndFunc(tree->data->value) << "\"]\n";
            if(tree->leftChild) {
                myfile << "_" << tree->index << "MO" << tree->data->value << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "MO" << tree->data->value << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case CLASS_SYSTEM_OP:
            myfile << "_" << tree->index << "SO" << "[shape=doublecircle, color=green,label=\"" << getStringSystemOp(tree->data->value) << "\"]\n";
            if(tree->leftChild) {
                myfile << "_" << tree->index << "SO" << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "SO" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case CLASS_POINT:
            myfile << "_" << tree->index << "P" << "[shape=star, color=green,label=\";\"]\n";
            if(tree->leftChild) {
                myfile << "_" << tree->index << "P" << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "P" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case CLASS_FUNC_NAME:
            if(tree->leftChild) {
                myfile << "_" << tree->index << "FN" << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "FN" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case DEF_FUNC:
            myfile << "_" << tree->index << "DF" << "[shape=doublecircle, color=blue,label=\"" << getStringSystemOp(tree->data->type) << "\"]\n";
            if(tree->leftChild) {
                myfile << "_" << tree->index << "DF" << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "DF" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case CLASS_OP:
            myfile << "_" << tree->index << "OP" << "[shape=doublecircle, color=blue,label=\"" << getStringSystemOp(tree->data->type) << "\"]\n";
            if(tree->leftChild) {
                myfile << "_" << tree->index << "OP" << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "OP" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case CLASS_BOOL_SIGN:
            myfile << "_" << tree->index << "BOP" << "[shape=doublecircle, color=blue,label=\"" << getStringBoolOp(tree->data->value) << "\"]\n";
            if(tree->leftChild) {
                myfile << "_" << tree->index << "BOP" << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "BOP" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        case CLASS_CALL_FUNC:
            myfile << "_" << tree->index << "CF" << " [shape=triangle,label=\"" << tree->data->name << "\"]\n";
            if(tree->leftChild) {
                myfile << "_" << tree->index << "CF" << " -- ";
                writeChild(myfile, tree->leftChild);
            }
            if(tree->rightChild) {
                myfile << "_" << tree->index << "CF" << " -- ";
                writeChild(myfile, tree->rightChild);
            }
            break;
        default:
            printf("Error in dump - undefined op class!(1)%d\n", tree->data->type);
            exit(EXIT_FAILURE);
    }
    gravizDeepWriting(myfile, tree->leftChild, index);
    gravizDeepWriting(myfile, tree->rightChild, index);
}

void writeChild(std::ofstream &myfile, Node *tree) {
    switch (tree->data->type) {
        case CLASS_VARIABLE:
            myfile << "_" << tree->index << "V" << ";\n";
            myfile << "_" << tree->index << "V" << " [shape=triangle,label=\"" << tree->data->name << "\"]\n";
            break;
        case CLASS_NUMBER:
            myfile << "_" << tree->index << "N" << tree->data->value << ";\n";
            myfile << "_" << tree->index << "N" << tree->data->value << " [label=\"" << tree->data->value << "\"]\n";
            break;
        case CLASS_MATH_OP:
            myfile << "_" << tree->index << "MO" << tree->data->value << ";\n";
            myfile << "_" << tree->index << "MO" << tree->data->value << " [label=\"" << tree->data->value << "\"]\n";
            break;
        case CLASS_MATH_FUNC:
            myfile << "_" << tree->index << "MF" << tree->data->value << ";\n";
            myfile << "_" << tree->index << "MF" << tree->data->value << " [label=\"" << tree->data->value << "\"]\n";
            break;
        case DEF_FUNC:
            myfile << "_" << tree->index << "DF" << ";\n";
            myfile << "_" << tree->index << "DF"  << "[shape=doublecircle, color=blue,label=\"" << getStringSystemOp(tree->data->type) << "\"]\n";
            break;
        case CLASS_FUNC_NAME:
            myfile << "_" << tree->index << "FN" << ";\n";
            if(tree->data->value != -1) {
                myfile << "_" << tree->index << "FN" << " [label=\"" << getStringSystemOp(tree->data->value) << "\"]\n";
                break;
            }
            myfile << "_" << tree->index << "FN" << " [label=\"" << tree->data->name << "\"]\n";
            break;
        case CLASS_SYSTEM_OP:
            myfile << "_" << tree->index << "SO" << ";\n";
            break;
        case CLASS_POINT:
            myfile << "_" << tree->index << "P" << ";\n";
            break;
        case CLASS_OP:
            myfile << "_" << tree->index << "OP" << ";\n";
            break;
        case CLASS_BOOL_SIGN:
            myfile << "_" << tree->index << "BOP" << ";\n";
            break;
        case CLASS_CALL_FUNC:
            myfile << "_" << tree->index << "CF" << ";\n";
            myfile << "_" << tree->index << "CF" << " [shape=triangle,label=\"" << tree->data->name << "\"]\n";
            break;
        default:
            myfile << "_" << tree->index << "_" << tree->data->type << ";\n";
    }
}
