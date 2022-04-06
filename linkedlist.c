#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "linkedlist.h"
#include "talloc.h"


// Create an empty linked list
// It has no input parameters
// Returns a null list of type NULL_TYPE
Value *makeNull() {
	Value *newVal = talloc(sizeof(Value));
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
	Value *newVal = talloc(sizeof(Value));
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
		case PTR_TYPE:
			printf("%p", list->p);
			break;
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
			printf("(");
			displayHelp(list->c.car);
			printf(" ");
			displayHelp(list->c.cdr);
			printf(")");
			break;
		case NULL_TYPE:
			printf("%s", "NULL");
			break;
		default:
			printf("%s", list->s);
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
	assert(list->type == CONS_TYPE);
	return list->c.car;
}

// Returns a Value representing the rest of the list passed in after the
// first Value.
// Assumes that the Value passed in is of CONS_TYPE.
Value *cdr(Value *list) {
	assert(list->type == CONS_TYPE);
	return list->c.cdr;
}

// Tests if value is null or not by checking the type of value
bool isNull(Value *value) {
	return value->type == NULL_TYPE;
}

// Returns the length of the list represented by the Value passed in.
// Assumes that the Value passed in is of CONS_TYPE.
int length(Value *value) {
	int size;
	if (value->type != CONS_TYPE) {
		return 1;
	}
	for(size = 0; !isNull(value); size++) {
		assert(value->type == CONS_TYPE);
		value = value->c.cdr;
	}
	return size;
}

// Function that takes in list, a linked list, and returns the list in reverse
// order as a linked list. The list returned is a full copy (no remaining pointers)
// of the original input list.
Value *reverse(Value *list) {
	if (list->type != CONS_TYPE) {
		return list;
	}

	Value *cur = list;
	Value *newList = makeNull();
	while (!isNull(cur)) {
		assert(cur->type == CONS_TYPE); // assert valid list
		newList = cons(car(cur), newList);
		cur = cdr(cur);
	}

	return newList;
}

// Takes in a Value and recursively frees the memory for all the Values
// stored in the list.
/*void cleanup(Value *list) {
	if (list-> type != CONS_TYPE) {
		free(list);
		list = NULL;
	} else {
		cleanup(car(list));
		cleanup(cdr(list));
		free(list);
		list = NULL;
	}
}*/
