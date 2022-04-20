#include <stdio.h>
#include "interpreter.h"

void interpret(Value *tree) {
	while(!isNull(tree)) {
		Frame *parentFrame = talloc(sizeof(Frame));
		parentFrame->bindings = makeNull();
		parentFrame->parent = NULL;
		printTree(eval(car(tree), parentFrame));
		printf("\n");
		tree = cdr(tree);
	}
}

Value *lookUpSymbol(Value *tree, Frame *frame) {
	if(frame == NULL) {
		printf("Variable %s not found\n", tree->s);
		texit(1);
	}
	Value *binding = frame->bindings;
	while(!isNull(binding)) {
		if(car(car(binding))->s == tree->s) {
			return car(cdr(binding));
		}
		binding = cdr(binding);
	}
	lookUpSymbol(tree, frame->parent);
}

Value *eval(Value *expr, Frame *frame) {
	switch(expr->type) {
		case INT_TYPE:
		case DOUBLE_TYPE:
		case STR_TYPE:
		case BOOL_TYPE:
			return expr;
			break;
		case SYMBOL_TYPE:
			return lookUpSymbol(expr, frame);
			break;
		case CONS_TYPE:
			break;
		default:
			break;
	}
}
