#include <iostream>
#include "frontend/lexer.h"
#include "frontend/frontend.h"


int main() {
    int size = 0;
    char* expr = getExpressionFromFile("../gr1.txt", &size);
    std::cout << expr << "\n";

    std::vector<Token*> a = doLexer(expr);
    auto tree = buildTree(&a);

    return 0;
}
