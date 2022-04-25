#include <stdio.h>
#include <string.h>
#include "interpreter.h"


void interpret(Value *tree) {
	Frame *parentFrame = talloc(sizeof(Frame));
	parentFrame->bindings = makeNull();
	parentFrame->parent = NULL;
	while(!isNull(tree)) {
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
		if(!strcmp(car(car(binding))->s, tree->s)) {
			return cdr(car(binding));
		}
		binding = cdr(binding);
	}
	return lookUpSymbol(tree, frame->parent);
}

Value *evalIf(Value *args, Frame *frame) {
	if (length(args) != 3) {
		printf("Invalid if statement, must have 3 arguments\n");
		texit(1);
	}

	if (strcmp(eval(car(args), frame)->s, "#f")) {
		return eval(car(cdr(args)), frame);
	}

	return eval(car(cdr(cdr(args))), frame);
}

Value *evalQuote(Value *args) {
	if (length(args) != 1) {
		printf("quote must have one argument\n");
		texit(1);
	}
	return args;
}

Value *evalLet(Value *args, Frame *frame) {
	Frame *newFrame = talloc(sizeof(Frame));
	newFrame->bindings = makeNull();
	newFrame->parent = frame;

	if (length(args) != 2) {
		printf("let must have exactly 2 arguments: a list of bindings and a body\n");
		texit(1);
	}

	Value *binding = car(args);
	while (!isNull(binding)) {
		if (binding->type != CONS_TYPE || length(car(binding)) != 2) {
			printf("Invalid \'let\' bindings\n");
			texit(1);
		}

		Value *result = eval(car(cdr(car(binding))), frame);
		Value *symbol = car(car(binding));
		if (symbol->type != SYMBOL_TYPE) {
			printf("Let must define symbol types only\n");
			texit(1);
		}
		Value *newBinding = cons(symbol, result);
		newFrame->bindings = cons(newBinding, newFrame->bindings);

		binding = cdr(binding);
	}

	return eval(car(cdr(args)), newFrame);
}

Value *evalDefine(Value *args, Frame *frame) {
	Frame *curFrame = frame;
	while(curFrame->parent != NULL) {
		curFrame = curFrame->parent;
	}
	if(length(args) != 2) {
		printf("Invalid define, must have exactly 2 arguments");
	}
	Value *returnValue = makeNull();
	returnValue->type = VOID_TYPE;
	
	Value *bindings = frame->bindings;
	while(!isNull(bindings)) {
		if(!strcmp(car(car(bindings))->s, car(args)->s)) {
			Value *existingBinding = cdr(car(bindings));
			existingBinding = eval(car(cdr(args)), frame);
			return returnValue;
		}
		bindings = cdr(bindings);
	}
	
	Value *newBinding = cons(car(args), eval(car(cdr(args)), frame));
	curFrame->bindings = cons(newBinding, curFrame->bindings);
	
	return returnValue;
}

Value *eval(Value *expr, Frame *frame) {
	switch(expr->type) {
		case INT_TYPE:
		case DOUBLE_TYPE:
		case STR_TYPE:
		case BOOL_TYPE:
			return expr;
		case SYMBOL_TYPE:
			return lookUpSymbol(expr, frame);
		case CONS_TYPE: {
			Value *first = car(expr);
			Value *args = cdr(expr);
		
			if (first->type == CONS_TYPE) {
				first = eval(first, frame);
			} 
			
			if (first->type != SYMBOL_TYPE) {
				printf("Unknown function\n");
				texit(1);
			}
			
			// Evaluate functions
			if (!strcmp(first->s, "if")) {
				return evalIf(args, frame);
			}

			else if (!strcmp(first->s, "quote")) {
				return evalQuote(args);
			}

			else if (!strcmp(first->s, "let")) {
				return evalLet(args, frame);
			}
			else if(!strcmp(first->s, "define")) {
				return evalDefine(args, frame);
			}

			// ... further special forms here ...

			else {
				printf("Function %s not recognized\n", first->s);
				texit(1);
			}

			return NULL;  // to make the compiler happy
		}
		default:
			printf("Invalid token for evaluation");
			texit(1);
			return NULL;  // To make the compiler happy
	}
}
