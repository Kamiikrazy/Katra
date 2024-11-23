#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char src[] = "a + b = 2 if a == 3";
int line;

typedef enum {
    Num, Fn, Pub, Loc, If, Else, Orif, Return, Try, While, For, Pass, Break, Do,
    String, Int, Float, Bool, Or, And, Not, Eq, Ne, Eqeq, Lt, Gt, Le, Ge,
    Add, Sub, Mul, Div, Mod, Inc, Dec, Muleq, Diveq, 
    Dot, Comma,
    Huh,
} Token;

const char *keywords[] = {
    "function", "for", "do", "if", "orif", "otherwise", "try", "while", "return", "public", "private",
    "int", "string", "float", "bool", "Null", "any", "True", "False", "module",
};

const char *operators[] = {
    "+", "-", "/", "*", "=", "=/=", "==", "+=", "-=", "/=", "*=", "<", 
    ">", "<=", ">=", "%",
};

#define NUM_KEYWORDS (sizeof(keywords) / sizeof(keywords[0]))
#define NUM_OPERATORS (sizeof(operators) / sizeof(operators[0]))

typedef struct {
    Token type;
    char *value;
} MToken;

Token classifyKeyword(const char *str) {
    if (strcmp(str, "function") == 0) return Fn;
    if (strcmp(str, "public") == 0) return Pub;
    if (strcmp(str, "private") == 0) return Loc;
    if (strcmp(str, "if") == 0) return If;
    if (strcmp(str, "otherwise") == 0) return Else;
    if (strcmp(str, "return") == 0) return Return;
    if (strcmp(str, "orif") == 0) return Orif;
    if (strcmp(str, "try") == 0) return Try;
    if (strcmp(str, "while") == 0) return While;
    if (strcmp(str, "do") == 0) return Do;
    if (strcmp(str, "string") == 0) return String;
    if (strcmp(str, "int") == 0) return Int;
    if (strcmp(str, "float") == 0) return Float;
    if (strcmp(str, "bool") == 0) return Bool;
}

Token classifyOperator(const char *str) {
    if (strcmp(str, "+") == 0) return Add;
    if (strcmp(str, "-") == 0) return Sub;
    if (strcmp(str, "*") == 0) return Mul;
    if (strcmp(str, "/") == 0) return Div;
    if (strcmp(str, "=") == 0) return Eq;
    if (strcmp(str, "==") == 0) return Eqeq;
    if (strcmp(str, "!=") == 0) return Ne;
    if (strcmp(str, "%") == 0) return Mod;
    if (strcmp(str, "+=") == 0) return Inc;
    if (strcmp(str, "-=") == 0) return Dec;
    if (strcmp(str, "*=") == 0) return Muleq;
    if (strcmp(str, "/=") == 0) return Diveq;
    if (strcmp(str, "&") == 0) return And;
    if (strcmp(str, "||") == 0) return Or;
    if (strcmp(str, "!") == 0) return Not;
}

int isKeyword(const char *str) {
    for(int i = 0; i < NUM_KEYWORDS; i++) {
        if(strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isOperator(const char *str, int length) {
    for (int i = 0; i < NUM_OPERATORS; i++) {
        if (strncmp(str, operators[i], length) == 0 && strlen(operators[i]) == length) {
            return 1;
        }
    }
    return 0;
}

void setValue(MToken *token, const char *text) {
    token->value = malloc(strlen(text) + 1);
    if(token->value) {
        strcpy(token->value, text);
    }
}

void freeToken(MToken *token) {
    if(token->value) {
        free(token->value);
        token->value = NULL;
    }
}

void next() {
    const char *c = src;

    while(*c != '\0') {
        MToken token = {0, NULL};

        if(isspace(*c)) {
            c++;
            continue;
        } else if(*c == '\n') {
            line++;
            c++;
            continue;
        }

        if(isalpha(*c) || *c == '_') {
            const char *start = c;
            while(isalpha(*c) || *c == '_') {
                c++;
            }
            int length = c - start;
            char *buffer = malloc(length + 1);
            strncpy(buffer, start, length);
            buffer[length] = '\0';

            setValue(&token, buffer);
            free(buffer);

            if(isKeyword(token.value)) {
                token.type = classifyKeyword(token.value);
                printf("Token(KEYWORD, %s)\n", token.value);
            } else {
                token.type = Loc;
                printf("Token(IDENTIFIER, %s)\n", token.value);
            }
            freeToken(&token);
        } else if(isdigit(*c)) {
            const char *start = c;
            while(isdigit(*c)) {
                c++;
            }
            int length = c - start;
            char *buffer = malloc(length + 1);
            strncpy(buffer, start, length);
            buffer[length] = '\0';

            setValue(&token, buffer);
            free(buffer);

            token.type = Num;
            printf("Token(NUMBER, %s)\n", token.value);
            freeToken(&token);
        } else if (isOperator(c, 1)) {
            const char *start = c;
            while (*c && isOperator(c, c - start + 1)) {
                c++;
            }
            int length = c - start;
            char *buffer = malloc(length + 1);
            strncpy(buffer, start, length);
            buffer[length] = '\0';
            setValue(&token, buffer);
            free(buffer);
            token.type = classifyOperator(token.value);
            printf("Token(OPERATOR, %s)\n", token.value);
            freeToken(&token);
        } else if(*c == '"' || *c == '\'') {
            char quote = *c++;
            const char *start = c;
            while(*c != quote && *c != '\0') {
                c++;
            }
            if(*c == quote) {
                int length = c - start;
                char *buffer = malloc(length + 1);
                strncpy(buffer, start, length);
                buffer[length] = '\0';
                c++;

                setValue(&token, buffer);
                free(buffer);

                token.type = String;
                printf("Token(STRING, %s)\n", token.value);
            } else {
                printf("Unterminated string literal\n");
            }
            freeToken(&token);
        } else {
            char buffer[2] = {*c++, '\0'};
            setValue(&token, buffer);
            token.type = Huh;
            printf("Token(UNKNOWN, %s)\n", token.value);
            freeToken(&token);
        }
    }
}
