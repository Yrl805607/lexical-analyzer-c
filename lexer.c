#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";

static FILE *file_ptr = NULL;

void initializeLexer(const char* filename) {
    file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("Open    : %s : Failed\n", filename);
        exit(EXIT_FAILURE);
    }
    printf("Open    : %s : Success\n", filename);
    printf("Parsing : %s : Started\n\n", filename);
}

void closeLexer(void) {
    if (file_ptr != NULL) {
        fclose(file_ptr);
        file_ptr = NULL;
    }
}

int isKeyword(const char* str) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(keywords[i], str) == 0) {
            return 1;
        }
    }
    return 0;
}

int isOperator(const char* str) {
    if (str == NULL || strlen(str) == 0) return 0;
    
    // Multi-character operators
    if (strcmp(str, "==") == 0 || strcmp(str, "!=") == 0 || 
        strcmp(str, "<=") == 0 || strcmp(str, ">=") == 0 || 
        strcmp(str, "&&") == 0 || strcmp(str, "||") == 0 || 
        strcmp(str, "++") == 0 || strcmp(str, "--") == 0 ||
        strcmp(str, "+=") == 0 || strcmp(str, "-=") == 0 ||
        strcmp(str, "*=") == 0 || strcmp(str, "/=") == 0 ||
        strcmp(str, "%=") == 0) {
        return 1;
    }

    // Single-character operator
    if (strlen(str) == 1 && strchr(operators, str[0]) != NULL) {
        return 1;
    }

    return 0;
}

int isSpecialCharacter(char ch) {
    return (strchr(specialCharacters, ch) != NULL);
}

int isConstant(const char* str) {
    if (str == NULL || strlen(str) == 0) return 0;

    // String or character literal
    if (str[0] == '"' || str[0] == '\'') return 1;

    // Reject standalone sign symbols (+ or -) from being categorized as literals
    if (strcmp(str, "+") == 0 || strcmp(str, "-") == 0) return 0;

    int i = 0;
    int dot_count = 0;

    if (str[0] == '-' || str[0] == '+') i++;

    int digits_found = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (isdigit((unsigned char)str[i])) {
            digits_found++;
        } else {
            return 0;
        }
    }

    return (digits_found > 0);
}

int isIdentifier(const char* str) {
    if (str == NULL || strlen(str) == 0) return 0;

    if (!isalpha((unsigned char)str[0]) && str[0] != '_') {
        return 0;
    }

    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum((unsigned char)str[i]) && str[i] != '_') {
            return 0;
        }
    }

    return !isKeyword(str);
}

void categorizeToken(Token* token) {
    if (isKeyword(token->lexeme)) {
        token->type = KEYWORD;
    } else if (isIdentifier(token->lexeme)) {
        token->type = IDENTIFIER;
    } else if (isConstant(token->lexeme)) {
        token->type = CONSTANT;
    } else if (isOperator(token->lexeme)) {
        token->type = OPERATOR;
    } else if (strlen(token->lexeme) == 1 && isSpecialCharacter(token->lexeme[0])) {
        token->type = SPECIAL_CHARACTER;
    } else {
        token->type = UNKNOWN;
    }
}

Token getNextToken(void) {
    Token token;
    token.lexeme[0] = '\0';
    token.type = UNKNOWN;

    if (file_ptr == NULL) {
        return token;
    }

    int ch;

    while ((ch = fgetc(file_ptr)) != EOF) {
        if (isspace(ch)) {
            continue;
        }

        // Handle C Comments
        if (ch == '/') {
            int next_ch = fgetc(file_ptr);
            if (next_ch == '/') {
                while ((ch = fgetc(file_ptr)) != EOF && ch != '\n');
                continue;
            } else if (next_ch == '*') {
                while ((ch = fgetc(file_ptr)) != EOF) {
                    if (ch == '*') {
                        if ((ch = fgetc(file_ptr)) == '/') break;
                        else ungetc(ch, file_ptr);
                    }
                }
                continue;
            } else {
                ungetc(next_ch, file_ptr);
            }
        }

        // Handle String Literals
        if (ch == '"') {
            int idx = 0;
            token.lexeme[idx++] = ch;
            while ((ch = fgetc(file_ptr)) != EOF) {
                if (idx < MAX_TOKEN_SIZE - 1) token.lexeme[idx++] = ch;
                if (ch == '\\') {
                    ch = fgetc(file_ptr);
                    if (ch != EOF && idx < MAX_TOKEN_SIZE - 1) token.lexeme[idx++] = ch;
                } else if (ch == '"') {
                    break;
                }
            }
            token.lexeme[idx] = '\0';
            categorizeToken(&token);
            return token;
        }

        // Handle Character Literals
        if (ch == '\'') {
            int idx = 0;
            token.lexeme[idx++] = ch;
            while ((ch = fgetc(file_ptr)) != EOF) {
                if (idx < MAX_TOKEN_SIZE - 1) token.lexeme[idx++] = ch;
                if (ch == '\\') {
                    ch = fgetc(file_ptr);
                    if (ch != EOF && idx < MAX_TOKEN_SIZE - 1) token.lexeme[idx++] = ch;
                } else if (ch == '\'') {
                    break;
                }
            }
            token.lexeme[idx] = '\0';
            categorizeToken(&token);
            return token;
        }

        // Handle Identifiers & Keywords
        if (isalpha(ch) || ch == '_') {
            int idx = 0;
            token.lexeme[idx++] = ch;
            while ((ch = fgetc(file_ptr)) != EOF && (isalnum(ch) || ch == '_')) {
                if (idx < MAX_TOKEN_SIZE - 1) token.lexeme[idx++] = ch;
            }
            token.lexeme[idx] = '\0';
            if (ch != EOF) ungetc(ch, file_ptr);

            categorizeToken(&token);
            return token;
        }

        // Handle Numbers (starting directly with digits)
        if (isdigit(ch)) {
            int idx = 0;
            token.lexeme[idx++] = ch;
            while ((ch = fgetc(file_ptr)) != EOF && (isdigit(ch) || ch == '.')) {
                if (idx < MAX_TOKEN_SIZE - 1) token.lexeme[idx++] = ch;
            }
            token.lexeme[idx] = '\0';
            if (ch != EOF) ungetc(ch, file_ptr);

            categorizeToken(&token);
            return token;
        }

        // Handle Operators (including compound operators like +=, -=, etc.)
        if (strchr(operators, ch) != NULL) {
            int idx = 0;
            token.lexeme[idx++] = ch;
            int next_ch = fgetc(file_ptr);

            if (next_ch != EOF) {
                char double_op[3] = { (char)ch, (char)next_ch, '\0' };
                if (isOperator(double_op)) {
                    token.lexeme[idx++] = next_ch;
                } else {
                    ungetc(next_ch, file_ptr);
                }
            }
            token.lexeme[idx] = '\0';
            categorizeToken(&token);
            return token;
        }

        // Handle Array Brackets & Delimiters
        if (isSpecialCharacter((char)ch)) {
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            categorizeToken(&token);
            return token;
        }

        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = UNKNOWN;
        return token;
    }

    return token;
}