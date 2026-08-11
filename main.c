#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(int argc, char *argv[]) {
    // Check if input file argument was provided
    if (argc < 2) {
        printf("Usage: ./lexical_analyser <.c file>\n");
        return 1;
    }

    initializeLexer(argv[1]);

    Token token;
    while ((token = getNextToken()).type != UNKNOWN) {
        switch (token.type) {
            case KEYWORD:
                printf("Keyword    : %s\n", token.lexeme);
                break;
            case IDENTIFIER:
                printf("Identifier : %s\n", token.lexeme);
                break;
            case OPERATOR:
            case SPECIAL_CHARACTER:
                printf("Operator   : %s\n", token.lexeme);
                break;
            case CONSTANT:
                printf("Literal    : %s\n", token.lexeme);
                break;
            default:
                break;
        }
    }

    printf("\nParsing : %s : Done\n", argv[1]);
    closeLexer();

    return 0;
}