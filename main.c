#include "src/lexer.h"


int main() {
    TokenList list = {NULL, 0, 0};

    lexer(&list);

    for (int i = 0; i < list.count; i++) {
        printf("Token(Type: %d, Value: %s)\n", list.tokens[i].type, list.tokens[i].value);
    }
    freeTokenList(&list);

    return 0;
}
