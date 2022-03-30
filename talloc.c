/*
 * talloc.c file, a memory allocation program.
 * Authors: Jon, Isaac, Aidan
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "talloc.h"

/*
 * Linked list code without using talloc()
 */

// Create an empty linked list
// It has no input parameters
// Returns a null list of type NULL_TYPE
static Value *makeNull() {
	Value *newVal = malloc(sizeof(Value));
	if (newVal == NULL) {
		printf("Out of memory!\n");
		exit(1);
	}

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
static Value *cons(Value *car, Value *cdr) {
	Value *newVal = malloc(sizeof(Value));
	if (newVal == NULL) {
		printf("Out of memory!\n");
		exit(1);
	}
	newVal->type = CONS_TYPE;
	newVal->c.car = car;
	newVal->c.cdr = cdr;
	return newVal;
}

// Returns the first Value in the list represented by the Value passed in.
// Assumes that the Value passed in is of CONS_TYPE.
static Value *car(Value *list) {
	assert(list->type == CONS_TYPE);
	return list->c.car;
}

// Returns a Value representing the rest of the list passed in after the
// first Value.
// Assumes that the Value passed in is of CONS_TYPE.
static Value *cdr(Value *list) {
	assert(list->type == CONS_TYPE);
	return list->c.cdr;
}

// Tests if value is null or not by checking the type of value
static bool isNull(Value *value) {
	return value->type == NULL_TYPE;
}



/*
 * Talloc specific code
 *
 */

static Value *ptrList;

/*
 * A malloc-like function that allocates memory, tracking all allocated
 * pointers in the "active list."  (You can choose your implementation of the
 * active list, but whatever it is, your talloc code should NOT call functions
 * in linkedlist.h; instead, implement any list-like behavior directly here.
 * Otherwise you'll end up with circular dependencies, since you're going to
 * modify the linked list to use talloc instead of malloc.)
 */
void *talloc(size_t size) {
	if (ptrList == NULL) {
		ptrList = makeNull();
	}

	Value *ptr = malloc(sizeof(Value));
	if (ptr == NULL) {
		printf("Out of memory!\n");
		texit(1);
	}
	ptr->type = PTR_TYPE;
	ptr->p = malloc(size);
	
	ptrList = cons(ptr, ptrList);

	return ptr->p;
}

/*
 * Free all pointers allocated by talloc, as well as whatever memory you
 * malloc'ed to create/update the active list.
 */
void tfree() {
	
	while (!isNull(ptrList)) {
		free(car(ptrList)->p); // if not null, ptrList is a cons type of ptrs
		free(car(ptrList));

		Value *temp = cdr(ptrList);
		free(ptrList);
		ptrList = temp;
	}
	
	free(ptrList);
	ptrList = NULL;

}

/*
 * A simple two-line function to stand in for the C function "exit", which
 * calls tfree() and then exit().  (You'll use this later to allow a clean exit
 * from your interpreter when you encounter an error: so memory can be
 * automatically cleaned up when exiting.)
 */
void texit(int status) {
	tfree();
	exit(status);
}



