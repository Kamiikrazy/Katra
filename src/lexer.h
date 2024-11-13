#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char *src[];
int line;

const char *keywords[] = {
			  "function",
			  "for",
			  "if",
			  "else",
			  "while",
			  "return",
			  "int",
			  "str",
			  "char",
			  "float",
			  "bool",
			  };

#define NUM_KEYWORDS (sizeof(keywords)/sizeof(keywords[0])) 

typedef enum {
	KEYWORD,
	IDENTIFIER,
	STRING,
	NUMBER,
	OPERATOR,
	PUNCTUATION,
	OTHER
} TokenType;

typedef struct {
	TokenType type; // The token type
	char *value; // The value of the token to be stored
} KToken;

void setValue(KToken *token, const char *text) {
	token -> value = malloc(strlen(text) + 1);
	if(token->value) {
		strcpy(token->value, text);
	}
}

int isKeyword(const char *str) {
	for(int i=0; i<NUM_KEYWORDS; i++) {
		if(strcmp(str, keywords[i]) == 0) {
			return 1;	
		}
	}
	return 0;
}

void next() {
	const char *c = *src;
	KToken token;
	
	while(*c != '\0') {
		if(isspace(*c)) { // Skip whitespace
			c++;
			continue;
		}

		else if(*c == '\n') {
			++line;
		}
		// Check for keyword/identifiers
		else if(isalpha(*c) || *c == '_') {
			int len = 0;
			while(isalpha(*c) || *c == '_') {
				token.value[len++] = *c++;
			}
			token.value[len] = '\0';
			token.type = isKeyword(token.value) ? KEYWORD : IDENTIFIER;
			printf("Token(%s, %s)\n", token.type == KEYWORD ? "KEYWORD" : "IDENTIFIER", token.value);
		}
		// Check for numbers
		else if(isdigit(*c)) {
			int len = 0;
			while(isdigit(*c)) {
				token.value[len++] = *c++;
			}
			token.value[len] = '\0';
			token.type = NUMBER;
			printf("Token(NUMBER, %s)", token.value);
		}
		// Check for operators
		else if(*c == '+' || *c == '-' || *c == '/' || *c == '*' || *c == '<' || *c == '>' || *c == '!' || *c == '%') {
			int len = 0;
			while(*c == '+' || *c == '-' || *c == '/' || *c == '*' || *c == '<' || *c == '>' || *c == '!' || *c == '%') {
				token.value[len++] = *c++;
			}
			token.value[len] = '\0';
			token.type = OPERATOR;
			printf("Token(OPERATOR, %s)", token.value);
		}
		// Check for punctuation
		else if(ispunct(*c)) {
			token.value[0] = *c++;
			token.value[1] = '\0';
			token.type = PUNCTUATION;
			printf("Token(PUNCTUATION, %s)", token.value);
		}
		// Check for string
		else if(*c == '"' || *c == '\'') {
			int len = 0;
			while(*c != '"' || *c != '\'' && *c != '\0') {
				if(*c == '\\' && *(c+1) == '"') {
					token.value[len++] = '"';
					c += 2;
				} else if (*c == '\\' && *(c+1) == '\\') {
					c += 2;
				} else {
					token.value[len++] = *c++;
				}
			}
			
			if(*c == '"' || *c == '\'') {
				token.value[len] = '\0';
				token.type = STRING;
				printf("Token(STRING, %s)", token.value);
			} else {
				printf("Unterminated string literal");
				break;
			}
		}
		// Handle unknown values
		else {
			token.value[0] = *c++;
			token.value[1] = '\0';
			token.type = OTHER;
			printf("Token(UNKNOWN, %s)", token.value);
		}
	}
}
