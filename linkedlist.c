#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"


// Create an empty linked list
// It has no input parameters
// Returns a null list of type NULL_TYPE
Value *makeNull() {
	Value *newVal = malloc(sizeof(Value));
	newVal->type = NULL_TYPE;
	newVal->s = "NULL";
	return newVal;
}

// Creates a list of type CONS_TYPE containing
// the value of car followed by the value of cdr.
// Values of car and cdr are passed in which eventually
// make up the list.
// Returns a list of CONS_TYPE containing values of car
// and cdr.
Value *cons(Value *car, Value *cdr) {
	Value *newVal = malloc(sizeof(Value));
	newVal->type = CONS_TYPE;
	newVal->c.car = car;
	newVal->c.cdr = cdr;
	return newVal;
}

// Helper method for display.
// List to be displayed is passed in
// No values returned, but the values contained
// in the list are printed.
void displayHelp(Value *list) {
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

// Displays the contents of a list
// A list is passed in which is to be printed.
// No values are returned.
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
