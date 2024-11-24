#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char src[] = "public function main(a: int, b: int) { return a + b; }";
int line;

typedef enum {
    Num, Fn, Pub, Loc, If, Else, Orif, Return, Try, While, For, Pass, Break, Do,
    String, Int, Float, Bool, Null, True, False, Module, Or, And, Not, Eq, Ne, Eqeq, Lt, Gt, Le, Ge,
    Add, Sub, Mul, Div, Mod, Inc, Dec, Muleq, Diveq, 
    Dot, Comma, Semcol, Col, Dbcol, Bsl, Dbbsl, Dbfsl, Hash, Opb, Clb, Opclb, Clclb,
    Huh,
} Token;

const char *keywords[] = {
    "function", "for", "do", "if", "orif", "otherwise", "try", "while", "return", "public", "private",
    "int", "string", "float", "bool", "Null", "any", "True", "False", "module",
};

const char *operators[] = {
    "+", "-", "/", "*", "=", "=/=", "==", "+=", "-=", "/=", "*=", "<", 
    ">", "<=", ">=", "%", "&", "||", "!"
};

const char *punctuation[] = {
    ".", ",", ";", ":", "::", "\\", "\\\\", "//", "#", "(", ")", "{", "}",
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
    if (strcmp(str, "True") == 0) return True;
    if (strcmp(str, "False") == 0) return False;
    if (strcmp(str, "Null") == 0) return Null;
    if (strcmp(str, "module") == 0) return Module;
    return Huh;
}

Token classifyOperator(const char *str, int length) {
    if (length == 3 && strncmp(str, "=/=", 3) == 0) return Ne;
    if (length == 2) {
        if (strncmp(str, "==", 2) == 0) return Eqeq;
        if (strncmp(str, "<=", 2) == 0) return Le;
        if (strncmp(str, ">=", 2) == 0) return Ge;
        if (strncmp(str, "*=", 2) == 0) return Muleq;
        if (strncmp(str, "+=", 2) == 0) return Inc;
        if (strncmp(str, "-=", 2) == 0) return Dec;
        if (strncmp(str, "/=", 2) == 0) return Diveq;
        if (strncmp(str, "||", 2) == 0) return Or;
    }
    if (length == 1) {
        if (*str == '+') return Add;
        if (*str == '-') return Sub;
        if (*str == '*') return Mul;
        if (*str == '/') return Div;
        if (*str == '=') return Eq;
        if (*str == '<') return Lt;
        if (*str == '>') return Gt;
        if (*str == '!') return Not;
        if (*str == '%') return Mod;
        if (*str == '&') return And;
    }
    return Huh;
}

Token classifyPunctuation(const char *str, int length) {
    if (length == 2) {
        if (strncmp(str, "\\\\", 2) == 0) return Dbbsl;
        if (strncmp(str, "//", 2) == 0) return Dbfsl;
        if (strncmp(str, "::", 2) == 0) return Dbcol;
    }
    if (length == 1) {
        if (*str == ',') return Comma;
        if (*str == '.') return Dot;
        if (*str == ';') return Semcol;
        if (*str == ':') return Col;
        if (*str == '\\') return Bsl;
        if (*str == '#') return Hash;
        if (*str == '(') return Opb;
        if (*str == ')') return Clb;
        if (*str == '{') return Opclb;
        if (*str == '}') return Clclb;
    }
    return Huh;
}

int isKeyword(const char *str) {
    return classifyKeyword(str) != Huh;
}

int isOperator(const char *str, int length) {
    return classifyOperator(str, length) != Huh;
}

int isPunctuation(const char *str, int length) {
    return classifyPunctuation(str, length) != Huh;
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
        } else if (isOperator(c, 3) || isOperator(c, 2) || isOperator(c, 1)) {
            const char *start = c;
            int length = 0;

            if (isOperator(c, 3)) length = 3;
            else if (isOperator(c, 2)) length = 2;
            else if (isOperator(c, 1)) length = 1;

            c += length;

            char *buffer = malloc(length + 1);
            strncpy(buffer, start, length);
            buffer[length] = '\0';

            setValue(&token, buffer);
            free(buffer);

            token.type = classifyOperator(start, length);
            printf("Token(OPERATOR, %s)\n", token.value);
            freeToken(&token);
        } else if(isPunctuation(c, 2) || isPunctuation(c, 1)) {
            const char *start = c;
            int length = isPunctuation(c, 2) ? 2 : 1;
            c += length;

            char *buffer = malloc(length + 1);
            strncpy(buffer, start, length);
            buffer[length] = '\0';

            setValue(&token, buffer);
            free(buffer);

            token.type = classifyPunctuation(start, length);
            printf("Token(PUNCTUATION, %s)\n", token.value);
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
