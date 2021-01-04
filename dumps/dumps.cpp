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
            return;
        case CLASS_MATH_FUNC:
            myfile << "_" << tree->index << "MF" << tree->data->value << "[shape=box, color=red,label=\"" << getStringMathOpAndFunc(tree->data->value) << "\"]\n";
            myfile << "_" << tree->index << "MF" << tree->data->value << " -- ";
            writeChild(myfile, tree->rightChild);
            // no left child - only right
            break;
        case CLASS_MATH_OP:
            myfile << "_" << tree->index << "MO" << tree->data->value << "[shape=box, color=blue,label=\"" << getStringMathOpAndFunc(tree->data->value) << "\"]\n";
            myfile << "_" << tree->index << "MO" << tree->data->value << " -- ";
            writeChild(myfile, tree->leftChild);
            myfile << "_" << tree->index << "MO" << tree->data->value << " -- ";
            writeChild(myfile, tree->rightChild);
            break;
        default:
            printf("Error in dump - undefined op class!");
            exit(EXIT_FAILURE);
    }
    gravizDeepWriting(myfile, tree->leftChild, index);
    gravizDeepWriting(myfile, tree->rightChild, index);
}

void writeChild(std::ofstream &myfile, Node *tree) {
    switch (tree->data->type) {
        case CLASS_VARIABLE:
            myfile << "_" << tree->index << "V" << tree->data->value << ";\n";
            myfile << "_" << tree->index << "V" << tree->data->value << " [label=\"" << tree->data->name << "\"]\n";
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
        default:
            myfile << "_" << tree->index << "_" << tree->data->type << ";\n";
    }


}


void latexDump(char* outPath, Node* tree) {
    assert(outPath);
    assert(tree);
    std::ofstream myfile;
    myfile.open (outPath);
    myfile << "\\documentclass{article}\n";
    myfile << "\\begin{document}\n";
    myfile << "$$";
    latexDeepWriting(myfile, tree);
    myfile << "$$";
    myfile << "\n";
    myfile << "\\end{document}\n";
    myfile.close();
}

void latexDeepWriting(std::ofstream& myfile, Node* root) {
    if(root == nullptr)
        return;
//    if(root->type != NUM) {
//        if (root->type == DIV) {
//            myfile << "\\frac{";
//            latexDeepWriting(myfile, root->leftChild);
//            myfile << "}{";
//            latexDeepWriting(myfile, root->rightChild);
//            myfile << "}";
//            return;
//        }
//        myfile << "(";
//        latexDeepWriting(myfile, root->leftChild);
//        myfile << getNameOfOp(root->type);
//        latexDeepWriting(myfile, root->rightChild);
//        myfile << ")";
//    } else {
//        myfile << " "<< root->value << " ";
//    }
}

