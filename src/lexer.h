#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char src[] = "function 24 + 13";
int line;

typedef enum {
    Num, Fn, Pub, Loc, If, Else, Orif, Return, Try, While, For, Pass, Break, Do,
    String, Int, Float, Bool, Or, And, Not, Eq, Ne, Eqeq, Lt, Gt, Le, Ge,
    Add, Sub, Mul, Div, Mod, Inc, Dec, Muleq, Diveq, Huh,
} Token;

const char *keywords[] = {
    "function", "for", "do", "if", "orif", "otherwise", "try", "while", "->",
    "Int", "String", "Float", "Bool", "Null", "Any"
};

const char *operators[] = {
    "+", "-", "/", "*", "=", "=/=", "==", "+=", "-=", "/=", "*=", "<", 
    ">", "<=", ">=", "%", "and", "or", "not"
};

#define NUM_KEYWORDS (sizeof(keywords) / sizeof(keywords[0]))
#define NUM_OPERATORS (sizeof(operators) / sizeof(operators[0]))

Token classifyKeyword(const char *str) {
	if(str == "function") {
		return Fn;
	} else if(str == "public") {
		return Pub;
	} else if(str == "private") {
		return Loc;
	} else if(str == "if") {
		return If;
	} else if(str == "otherwise") {
		return Else;
	} else if(str == "->") {
		return Return;
	} else if(str == "orif") {
		return Orif;
	} else if(str == "try") {
		return Try;
	} else if(str == "while") {
		return While;
	} else if(str == "nothing") {
		return Pass;
	} else if(str == "break") {
		return Break;
	} else if(str == "do") {
		return Do;
	} else if(str == "string") {
		return String;
	} else if(str == "int") {
		return Int;
	} else if(str == "float") {
		return Float;
	} else if(str == "bool") {
		return Bool;
	}
}

typedef struct {
    Token type;
    char value[128];
} KToken;

int isKeyword(const char *str) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void setValue(KToken *token, const char *text) {
    strncpy(token->value, text, sizeof(token->value) - 1);
    token->value[sizeof(token->value) - 1] = '\0';
}

void next() {
    const char *c = src;
    KToken token;

    while (*c != '\0') {
        if (isspace(*c)) {
            c++;
            continue;
        } else if (*c == '\n') {
            line++;
            c++;
            continue;
        }

        if (isalpha(*c) || *c == '_') {
            char buffer[128];
            int len = 0;
            while (isalpha(*c) || *c == '_') {
                buffer[len++] = *c++;
            }
            buffer[len] = '\0';
            setValue(&token, buffer);

            if (isKeyword(token.value)) {
                token.type = Fn;
                printf("Token(KEYWORD, %s)\n", token.value);
            } else {
                token.type = Loc;
                printf("Token(IDENTIFIER, %s)\n", token.value);
            }
        } else if (isdigit(*c)) {
            char buffer[128];
            int len = 0;
            while (isdigit(*c)) {
                buffer[len++] = *c++;
            }
            buffer[len] = '\0';
            setValue(&token, buffer);
            token.type = Num;
            printf("Token(NUMBER, %s)\n", token.value);
        } else if (strchr("+-/*<>!%", *c)) {
            char buffer[128];
            int len = 0;
            while (strchr("+-/*<>!%", *c)) {
                buffer[len++] = *c++;
            }
            buffer[len] = '\0';
            setValue(&token, buffer);
            token.type = Eq;
            printf("Token(OPERATOR, %s)\n", token.value);
        } else if (*c == '"' || *c == '\'') {
            char quote = *c++;
            char buffer[128];
            int len = 0;
            while (*c != quote && *c != '\0') {
                if (*c == '\\' && *(c + 1) == quote) {
                    buffer[len++] = *c++;
                }
                buffer[len++] = *c++;
            }
            if (*c == quote) {
                c++;
                buffer[len] = '\0';
                setValue(&token, buffer);
                token.type = String;
                printf("Token(STRING, %s)\n", token.value);
            } else {
                printf("Unterminated string literal\n");
                break;
            }
        } else {
            char buffer[2] = {*c++, '\0'};
            setValue(&token, buffer);
            token.type = Huh;
            printf("Token(UNKNOWN, %s)\n", token.value);
        }
    }
}
