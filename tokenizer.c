#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"


static bool isTokenEnder(char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f' || c == '(' || c == ')' || c == ';';
}

static bool isSymbolCharacter(char c) {
	int charInt = c;
	return c == '+' || c == '-' || c == '.' || c == '*' || c == '/' || c == '<' || c == '=' || c == '>' || c == '!'
			|| c == '?' || c == ':' || c == '$' || c == '%' || c == '_' || c == '&' || c == '~' || c == '^'
			|| (charInt <= 57 && charInt >= 48) || (charInt >= 65 && charInt <= 90) || (charInt >= 97 && charInt <= 122);
}

static bool isDigit(char c) {
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9';
}

static char *charToStr(char c) {
	char *temp = talloc(sizeof(char) * 2);
	temp[0] = c;
	temp[1] = '\0';
	return temp;
}

static char *catStrChar(char *string, char c) {
	char *temp = charToStr(c);
	char *newStr = talloc(sizeof(string) + sizeof(char));
	newStr = strcpy(newStr, string);
	newStr = strcat(newStr, temp);
	return newStr;
}

/*
 * Reads in code and creates a linked list of tokens based on their type.
 */
Value *tokenize() {
	
	char charRead;
	Value *list = makeNull();
	charRead = fgetc(stdin);
	while (charRead != EOF) {
		if (charRead == '(') {
			Value *val = talloc(sizeof(Value));
			val->type = OPEN_TYPE;
			val->s = "(";
			list = cons(val, list);	
		} else if (charRead == ')') {
			Value *val = talloc(sizeof(Value));
			val->type = CLOSE_TYPE;
			val->s = ")";
			list = cons(val, list);	
		} else if (charRead == '#') {
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

		}
		else if(isDigit(charRead)) {
			Value *val = talloc(sizeof(Value));
			double tempNum = charRead - 48;
			val->type = INT_TYPE;

			char nextChar = fgetc(stdin);
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
			list = cons(val, list);
		}
		else if(isSymbolCharacter(charRead) || charRead == '\'') { // TODO: symbol
			Value *val = talloc(sizeof(Value));
			val->type = SYMBOL_TYPE;
			char *charString = charToStr(charRead);
			charRead = fgetc(stdin);
			while(isSymbolCharacter(charRead)) {
				charString = catStrChar(charString, charRead);
				charRead = fgetc(stdin);
			}
			val->s = charString;
			list = cons(val, list);
		}
		else if (charRead == '\"') {
			Value *val = talloc(sizeof(Value));
			val->type = STR_TYPE;
			charRead = fgetc(stdin);
			while(charRead != '\"') {
				if(charRead == '\\') {
					fgetc(stdin);
				}
				charRead = fgetc(stdin);
			}
			// TODO: save string
			list = cons(val, list);
		}
		else if(charRead == ';') {
			while(fgetc(stdin) != '\n');
		}
		else if(isTokenEnder(charRead));
		else {
			printf("Error, %c is not a valid character to start a token\n", charRead);
			//TODO: uncomment the next line when ready
			//texit(1);
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




