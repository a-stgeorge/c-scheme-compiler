#include <stdio.h>

#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"


static bool isTokenEnder(char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f' || c == '(' || c == ')' || c == ';';
}

static bool isDigit(char c) {
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9';
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
			int tempInt = charRead - 48;

			char nextChar = fgetc(stdin);
			while(isDigit(nextChar)) {
				tempInt *= 10;
				tempInt += (nextChar - 48);
			}	
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




