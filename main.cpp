#include <iostream>
#include "frontend/lexer.h"
#include "frontend/frontend.h"
#include "dumps/dumps.h"
#include "backend/backend.h"

int main(int argc, char** argv) {
    if(argc == 3) {
        int size = 0;
        char* expr = getExpressionFromFile(argv[1], &size);
//        std::cout << expr << "\n";

        std::vector<Token*> a = doLexer(expr);
        auto tree = buildTree(&a);

        gravizDump("../gr1.dot", tree);
        system(" dot -Tpdf ../gr1.dot -o gr1.pdf && xdg-open gr1.pdf");
        buildBackend(tree, argv[2]);
        return 0;
    }
    printf("Please!Check your arguments!\n");
    exit(EXIT_FAILURE);
}