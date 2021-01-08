#ifndef THE_CURE_LANGUAGE_DUMPS_H
#define THE_CURE_LANGUAGE_DUMPS_H

#include "../frontend/language.h"

/**
 * Dumps tree into graviz format
 * @param outPath
 * @param tree
 */
void gravizDump(char* outPath, Node* tree);

/**
 * Helping method to walk the tree when gravizDump is doing
 * @param myfile
 * @param tree
 * @param index
 */
void gravizDeepWriting(std::ofstream& myfile, Node* tree, int* index);

#endif //THE_CURE_LANGUAGE_DUMPS_H
