#include <stdio.h>

#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"


static bool isWhiteSpace(char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f';
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
			val->type = OPEN_TYPE;
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
			if (!isWhiteSpace(charRead)) {
				printf("Error, %s%c not a bool type", val->s, charRead);
				texit(1);
			}	
			
			list = cons(val, list);	

		}
		charRead = fgetc(stdin);
	}
	return reverse(list);
}

/*
 * Takes in a linked list of tokens as input and displays them with one token
 * per line.
 */
void displayTokens(Value *list);




