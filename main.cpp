#include <iostream>
#include "frontend/lexer.h"
#include "frontend/frontend.h"
#include "dumps/dumps.h"


int main() {
    int size = 0;
    char* expr = getExpressionFromFile("../gr1.txt", &size);
    std::cout << expr << "\n";

    std::vector<Token*> a = doLexer(expr);
    auto tree = buildTree(&a);
    gravizDump("../gr1.dot", tree);
//    system(" dot -Tpdf ../gr1.dot -o gr1.pdf");
    return 0;
}
