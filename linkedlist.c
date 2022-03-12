#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

Value *makeNull() { // Isaac
	Value *newVal = malloc(sizeof(Value));
	newVal->type = NULL_TYPE;
	newVal->s = "NULL";
	return newVal;
}

Value *cons(Value *car, Value *cdr) { // Isaac
	Value *newVal = malloc(sizeof(Value));
	newVal->type = CONS_TYPE;
	newVal->c.car = car;
	newVal->c.cdr = cdr;
	return newVal;
}

void displayHelp(Value *list) { // Isaac
	switch(list->type) {
		case INT_TYPE:
			printf("%d", list->i);
			break;
		case DOUBLE_TYPE:
			printf("%lf", list->d);
			break;
		case STR_TYPE:
			printf("%s", list->s);
			break;
		case CONS_TYPE:
			displayHelp(list->c.car);
			printf(" ");
			displayHelp(list->c.cdr);
			break;
		case NULL_TYPE:
			printf("%s", list->s);
			break;
	}
}

void display(Value *list) {
	displayHelp(list);
	printf("\n");
}

Value *car(Value *list) { // Jon

}

Value *cdr(Value *list) { // Jon

}

bool isNull(Value *value) { // Aidan

}

int length(Value *value) { // Jon

}

Value *reverse(Value *list) { // Aidan

}

void cleanup(Value *list) { // Aidan

}
