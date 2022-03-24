#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "linkedlist.h"

int main(void) {
	Value *val1 = malloc(sizeof(Value));
	val1->type = INT_TYPE;
	val1->i = 23;

	Value *val2 = malloc(sizeof(Value));
	val2->type = STR_TYPE;
	val2->s = "tofu"; 

	Value *head = makeNull();
	head = cons(val1, head);
	head = cons(val2, head);

	display(head);
	printf("Second Value: ");
	display(car(cdr(head)));
	printf("Length = %d\n", length(head));
	printf("Empty? %i\n", isNull(head));
	printf("IsNull? %i\n", isNull(cdr(cdr(head))));

	Value *val3 = malloc(sizeof(Value));
	val3->type = STR_TYPE;
	val3->s = "This is a test";

	Value *val4 = malloc(sizeof(Value));
	val4->type = DOUBLE_TYPE;
	val4->d = 3.14159;

	display(val3);
	Value *consVal = cons(val3, val4);
	display(consVal);

	Value *val5 = makeNull();
	display(val5);
	printf("Testing Null: %i\n\n", isNull(val5));

	// Testing reverse()
	val1 = makeNull();
	val1->type = INT_TYPE;
	val1->i = 23;
	val2 = makeNull();
	val2->type = STR_TYPE;
	val2->s = "tofu";
	val3 = malloc(sizeof(Value));
	val3->type = STR_TYPE;
	val3->s = "This is a test";
	val4 = malloc(sizeof(Value));
	val4->type = DOUBLE_TYPE;
	val4->d = 3.14159;
	Value *list = cons(val1, val2);
	list = cons(list, val5);
	list = cons(list, val4);
	list = cons(list, val3);
	display(list);

	Value *revList = reverse(list);
	display(revList);

	Value *revVal = reverse(val4);
	display(revVal);

	// Testing deep copy of reverse
	cdr(car(car(car(list))))->s = "very long string";
	display(list);
	display(revList);

	printf("\n");

	// Test Cleanup - also ran valgrind to double check
	// Need to be careful not to double free, anything that is already in
	// a list cannot be in another list
	cleanup(head);
	cleanup(consVal);
	cleanup(list);
	cleanup(revList);	
	cleanup(revVal);

	return 0;
}
