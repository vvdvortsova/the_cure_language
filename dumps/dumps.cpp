//#include <cassert>
//#include <cmath>
//#include "dumps.h"
//
//
//
//#define  EQUATION_EPS 1e-9
//
///**
//* @brief      Compare a double variable with zero
//* @param[in]  a  double variable
//* @return     0 if a == 0 else 1
//*/
//static bool isZero(double a) {
//    return fabs(a) < EQUATION_EPS;
//}
//
//void gravizDump(char* outPath, Node* tree) {
//    assert(outPath);
//    assert(tree);
//    int index = 0;
//    std::ofstream myfile;
//    myfile.open (outPath);
//    myfile << "graph astdump {\n";
//    myfile << "res [shape=doubleoctagon]" << "[label=\"result = "<< calculate(tree)<<"\"]\n";
//    gravizDeepWriting(myfile, tree, &index);
//    myfile << "}\n";
//
//    myfile.close();
//
//}
//
//void gravizDeepWriting(std::ofstream& myfile, Node* tree, int *index) {
//    if(tree == nullptr || tree->type == NUM || tree->type == VAR)
//        return;
//    if (tree->type == COS || tree->type == SIN) {
//        myfile << "_" << tree->index << "_" << tree->type << "[shape=box, color=red,label=\"" << getNameOfOp(tree->type) << "\"]\n";
//
//        myfile << "_" << tree->index << "_" << tree->type << " -- ";
//        if(tree->rightChild->type == NUM) {
//            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << ";\n";
//            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->value << "\"]\n";
//        }
//        else if(tree->rightChild->type == VAR) {
//            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << ";\n";
//            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->chVar << "\"]\n";
//        } else
//            myfile << "_" << tree->rightChild->index << "_" << tree->rightChild->type << ";\n";
//        gravizDeepWriting(myfile, tree->leftChild, index);
//        gravizDeepWriting(myfile, tree->rightChild, index);
//    } else {
//        myfile << "_" << tree->index << "_" << tree->type << "[shape=box, color=blue,label=\"" << getNameOfOp(tree->type) << "\"]\n";
//        myfile << "_" << tree->index << "_" << tree->type << " -- ";
//        if(tree->leftChild->type == NUM) {
//            myfile << "_" << tree ->leftChild->index << "N" ;
//            char temp[100] ="";
//            if(isZero(tree->leftChild->value)) {
//                sprintf(temp,"_%d0",(*index)++);
//            }
//            myfile << temp << "\n";
//            myfile << "_" << tree->leftChild->index << "N" << temp << " [label=\"" << tree->leftChild->value << "\"]\n";
//
//        } else if (tree->leftChild->type == VAR) {
//            myfile << "_" << tree ->leftChild->index << "V" ;
//            myfile << "_" << tree->leftChild->index << "V" << " [label=\"" << tree->leftChild->chVar << "\"]\n";
//
//        } else {
//            myfile << "_" << tree->leftChild->index << "_" << tree->leftChild->type << ";\n";
//        }
//
//        myfile << "_" << tree->index << "_" << tree->type << " -- ";
//        if(tree->rightChild->type == NUM) {
//            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << ";\n";
//            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->value << "\"]\n";
//        }
//        else if(tree->rightChild->type == VAR) {
//            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << ";\n";
//            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->chVar << "\"]\n";
//        } else
//            myfile << "_" << tree->rightChild->index << "_" << tree->rightChild->type << ";\n";
//
//        gravizDeepWriting(myfile, tree->leftChild, index);
//        gravizDeepWriting(myfile, tree->rightChild, index);
//    }
//}
//
//
//
//
//void latexDump(char* outPath, Node* tree) {
//    assert(outPath);
//    assert(tree);
//    std::ofstream myfile;
//    myfile.open (outPath);
//    myfile << "\\documentclass{article}\n";
//    myfile << "\\begin{document}\n";
//    myfile << "$$";
//    latexDeepWriting(myfile, tree);
//    myfile << "$$";
//    myfile << "\n";
//    myfile << "\\end{document}\n";
//    myfile.close();
//}
//
//void latexDeepWriting(std::ofstream& myfile, Node* root) {
//    if(root == nullptr)
//        return;
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
//}
//
