#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "linkedlist.h"
#include "talloc.h"

int main(void) {
	Value *val1 = talloc(sizeof(Value));
	val1->type = INT_TYPE;
	val1->i = 23;

	Value *val2 = talloc(sizeof(Value));
	val2->type = STR_TYPE;
	val2->s = talloc(10 * sizeof(char));
	strcpy(val2->s, "tofu");

	Value *head = makeNull();
	head = cons(val1, head);
	head = cons(val2, head);

	display(head);
	printf("Second Value: ");
	display(car(cdr(head)));
	printf("Length = %d\n", length(head));
	printf("Empty? %i\n", isNull(head));
	printf("IsNull? %i\n", isNull(cdr(cdr(head))));

	Value *val3 = talloc(sizeof(Value));
	val3->type = STR_TYPE;
	val3->s = "This is a test";

	Value *val4 = talloc(sizeof(Value));
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
	val3 = talloc(sizeof(Value));
	val3->type = STR_TYPE;
	val3->s = "This is a test";
	val4 = talloc(sizeof(Value));
	val4->type = DOUBLE_TYPE;
	val4->d = 3.14159;
	Value *list = cons(val1, makeNull());
	list = cons(val2, list);
	list = cons(val5, list);
	list = cons(val4, list);
	list = cons(val3, list);
	display(list);

	Value *revList = reverse(list);
	display(revList);

	Value *revVal = reverse(val4);
	display(revVal);
	printf("\n");

	texit(0);
	return 0;
}
