#include <iostream>
#include "frontend/lexer.h"

int main() {
    int size = 0;
    char* expr = getExpressionFromFile("../gr1.txt", &size);
    std::cout << expr << "\n";

    std::vector<Token*> a = doLexer(expr);
    return 0;
}
