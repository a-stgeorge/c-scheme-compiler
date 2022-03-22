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

// Returns the first Value in the list represented by the Value passed in.
// Assumes that the Value passed in is of CONS_TYPE.
Value *car(Value *list) {
	return list->c.car;
}

// Returns a Value representing the rest of the list passed in after the
// first Value.
// Assumes that the Value passed in is of CONS_TYPE.
Value *cdr(Value *list) {
	return list->c.cdr;
}

bool isNull(Value *value) { // Aidan

}

// Returns the length of the list represented by the Value passed in.
// Assumes that the Value passed in is of CONS_TYPE.
int length(Value *value) {
	int size;
	for(size = 0; !isNull(value); size++)
		value = value->c.cdr;
	return size;
}

Value *reverse(Value *list) { // Aidan

}

void cleanup(Value *list) { // Aidan

}
