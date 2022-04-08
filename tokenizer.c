#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"


static bool isTokenEnder(char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f' 
			|| c == '(' || c == ')' || c == ';' || c == EOF;
}

static bool isSymbolCharacter(char c) {
	int charInt = c;
	return c == '+' || c == '-' || c == '.' || c == '*' || c == '/' || c == '<' || c == '=' 
			|| c == '>' || c == '!' || c == '?' || c == ':' || c == '$' || c == '%' || c == '_' 
			|| c == '&' || c == '~' || c == '^' || (charInt <= 57 && charInt >= 48) 
			|| (charInt >= 65 && charInt <= 90) || (charInt >= 97 && charInt <= 122);
}

static bool isDigit(char c) {
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' 
			|| c == '7' || c == '8' || c == '9';
}

static char *charToStr(char c) {
	char *temp = talloc(sizeof(char) * 2);
	temp[0] = c;
	temp[1] = '\0';
	return temp;
}

static char *catStrChar(char *string, char c) {
	char *temp = charToStr(c);
	char *newStr = talloc(strlen(string) + 2*sizeof(char));
	newStr = strcpy(newStr, string);
	newStr = strcat(newStr, temp);
	return newStr;
}

static void constructNumber(Value *val, char nextChar) {
	double tempNum = nextChar - 48;
    val->type = INT_TYPE;

    nextChar = fgetc(stdin);
    while(isDigit(nextChar)) {
        tempNum *= 10;
        tempNum += (nextChar - 48);
        nextChar = fgetc(stdin);
    }

    if(nextChar == '.') {
        val->type = DOUBLE_TYPE;
        nextChar = fgetc(stdin);

    	double dec = 1;
        while(isDigit(nextChar)) {
            dec /= 10;
            tempNum += (nextChar - 48)*dec;
            nextChar = fgetc(stdin);
        }
        val->d = tempNum;
    }
    else{
        val->i = tempNum;
    }
    if (!isTokenEnder(nextChar)) {
        printf("Error, not a number");
        texit(1);
    }
    ungetc(nextChar, stdin);
}

/*
 * Reads in code and creates a linked list of tokens based on their type.
 */
Value *tokenize() {
	
	char charRead;
	Value *list = makeNull();
	charRead = fgetc(stdin);
	while (charRead != EOF) {
		if (charRead == '(') { // Open Type
			Value *val = talloc(sizeof(Value));
			val->type = OPEN_TYPE;
			val->s = "(";
			list = cons(val, list);	
		} else if (charRead == ')') { // Close Type
			Value *val = talloc(sizeof(Value));
			val->type = CLOSE_TYPE;
			val->s = ")";
			list = cons(val, list);	
		} else if (charRead == '#') { // Boolean Type
			Value *val = talloc(sizeof(Value));
			val->type = BOOL_TYPE;

			charRead = fgetc(stdin);
			if (charRead == 't') {
				val->s = "#t";
			} else if (charRead == 'f') {
				val->s = "#f";
			} else {
				printf("Error, #%c not a bool type", charRead);
				texit(1);
			}
			
			charRead = fgetc(stdin);
			if (!isTokenEnder(charRead)) {
				printf("Error, %s%c not a bool type", val->s, charRead);
				texit(1);
			}
			ungetc(charRead, stdin);
			
			list = cons(val, list);	

		} else if (charRead == '+' || charRead == '-') {
			Value *val = talloc(sizeof(Value));
			char sign = charRead;
			charRead = fgetc(stdin);
			if (isTokenEnder(charRead)) {
				val->type = SYMBOL_TYPE;
				val->s = charToStr(sign);
			}
			else {
				constructNumber(val, charRead);
				if(sign == '-' && val->type == INT_TYPE) {
					val->i *= -1;
				} else if (sign == '-') {
					val->d *= -1;
				}
			}
			list = cons(val, list);
		} else if (charRead == '.') {
			ungetc(charRead, stdin);
			charRead = '0';
			Value *val = talloc(sizeof(Value));
			constructNumber(val, charRead);
			list = cons(val, list);
		} else if (isDigit(charRead)) { // Number Types
			Value *val = talloc(sizeof(Value));
			constructNumber(val, charRead);
			list = cons(val, list);

		} else if (isSymbolCharacter(charRead) || charRead == '\'') { // Symbol Type
			Value *val = talloc(sizeof(Value));
			val->type = SYMBOL_TYPE;
			char *charString = charToStr(charRead);
			charRead = fgetc(stdin);
			while(isSymbolCharacter(charRead)) {
				charString = catStrChar(charString, charRead);
				charRead = fgetc(stdin);
			}

			if (!isTokenEnder(charRead)) {
                printf("Error, %c is not a valid symbol character\n", charRead);
                texit(1);
            }
			ungetc(charRead, stdin);
			val->s = charString;
			list = cons(val, list);
		} else if (charRead == '\"') { // String Type
			Value *val = talloc(sizeof(Value));
			val->type = STR_TYPE;
            char *charString = charToStr(charRead);
			charRead = fgetc(stdin);
			while(charRead != '\"') {
				if(charRead == EOF) {
					printf("Error, unterminated string");
					texit(1);
				}
				if(charRead == '\\') {
					charString = catStrChar(charString, charRead);
					charRead = fgetc(stdin);
				}
				charString = catStrChar(charString, charRead);
				charRead = fgetc(stdin);
			}
			charString = catStrChar(charString, charRead);

			val->s = charString;
			list = cons(val, list);
		} else if(charRead == ';') { // Comments
			charRead = fgetc(stdin);
			while(charRead != '\n' && charRead != EOF) {
				charRead = fgetc(stdin);
			}
			ungetc(charRead, stdin);
		}
		else if(isTokenEnder(charRead)) {
			// No action needed, whitespace
		}
		else {
			printf("Error, %c is not a valid character to start a token\n", charRead);
			texit(1);
		}
		charRead = fgetc(stdin);
	}
	return reverse(list);
}

/*
 * Takes in a linked list of tokens as input and displays them with one token
 * per line.
 */
void displayTokens(Value *list) {
	Value *token;
	while(!isNull(list)) {
		token = car(list);
		switch(token->type) {
			case INT_TYPE:
				printf("%d:integer\n", token->i);
				break;
			case DOUBLE_TYPE:
				printf("%lf:float\n", token->d);
				break;
			case STR_TYPE:
				printf("%s:string\n", token->s);
				break;
            case OPEN_TYPE:
                printf("%s:open\n", token->s);
                break;
            case CLOSE_TYPE:
                printf("%s:close\n", token->s);
                break;
            case BOOL_TYPE:
                printf("%s:boolean\n", token->s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", token->s);
                break;
            default:
				printf("Error: invalid token type\n");
				return;
                break;
		}
		list = cdr(list);
	}
}




