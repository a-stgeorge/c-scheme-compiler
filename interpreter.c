#include <stdio.h>
#include <string.h>
#include "interpreter.h"

// helper function that determines if list of values list contains a particular symbol symb.
static bool contains(Value *list, Value *symb) {
	if (symb->type != SYMBOL_TYPE) {
		printf("Invalid symbol type\n");
		texit(1);
	}
	
	Value *cur = list;
	while (!isNull(cur)) {
		if (cur->type != CONS_TYPE) {
			printf("Invalid list\n");
			texit(1);
		}
		if (!strcmp(car(cur)->s, symb->s)) {
			return true;
		}
		cur = cdr(cur);
	}

	return false;
}

void bind(char *name, Value *(*function)(Value *), Frame *frame) {
	Value *value = makeNull();
	value->type = PRIMITIVE_TYPE;
	value->pf = function;

	Value *symbol = makeNull();
	symbol->type = SYMBOL_TYPE;
	symbol->s = name;
	Value *binding = cons(symbol, value);
	frame->bindings = cons(binding, frame->bindings);
}

// PRIMITIVE FUNCTION DEFINITIONS
Value *primitiveAdd(Value *args) {
	double sum = 0;
	bool hasDoubles = false;
	// error checking on args, a list of inputs
	// compute the result as a single value
	Value *cur = args;
	while (!isNull(cur)) {
		if (car(cur)->type == INT_TYPE) {
			sum += car(cur)->i;
		} else if (car(cur)->type == DOUBLE_TYPE) {
			hasDoubles = true;
			sum += car(cur)->d;
		} else {
			printf("+ can only take in numbers\n");
			texit(1);
		}
		cur = cdr(cur);
	}
	Value *returnValue = makeNull();
	if (hasDoubles) {
		returnValue->type = DOUBLE_TYPE;
		returnValue->d = sum;
	} else {
		returnValue->type = INT_TYPE;
		returnValue->i = (int) sum;
	}
	return returnValue;
}

Value *primitiveIsNull(Value *args) {
	if(length(args) != 1) {
		printf("null? can only have one argument\n");
		texit(1);
	}
	if(car(args)->type == SYMBOL_TYPE) {
		printf("Invalid arguments\n");
		texit(1);
	}
	
	Value *result = makeNull();
	result->type = BOOL_TYPE;
	if(isNull(args)) {
		result->s = "#t";
	}
	else {
		result->s = "#f";
	}
	return result;
}

Value *primitiveCar(Value *args) {
	if(length(args) != 1) {
		printf("car can only have one argument\n");
		texit(1);
	}
	if(args->type != CONS_TYPE) {
		printf("car function expects a list\n");
		texit(1);
	}
	return car(car(car(args)));
}

Value *primitiveCdr(Value *args) {
	if(length(args) != 1) {
		printf("cdr can only have one argument\n");
		texit(1);
	}
	if(args->type != CONS_TYPE) {
		printf("cdr function expects a list\n");
		texit(1);
	}
	return cdr(car(car(args)));
}

Value *primitiveCons(Value *args) {
	if(length(args) != 2) {
		printf("Cons must have exactly two arguments.\n");
        texit(1);
	}
	return cons(car(args), car(cdr(args)));
}


// MAIN INTERPRET FUNCITON
void interpret(Value *tree) {
	Frame *parentFrame = talloc(sizeof(Frame));
	parentFrame->bindings = makeNull();
	parentFrame->parent = NULL;
	bind("+", primitiveAdd, parentFrame);
	bind("null?", primitiveIsNull, parentFrame);
	bind("car", primitiveCar, parentFrame);
	bind("cdr", primitiveCdr, parentFrame);
	bind("cons", primitiveCons, parentFrame);

	while(!isNull(tree)) {
		Value *result = eval(car(tree), parentFrame);
		printTree(result);
		if (result->type != VOID_TYPE) {
			printf("\n");
		}
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

	Value *cond = eval(car(args), frame);
	if (cond->type == BOOL_TYPE && !strcmp(cond->s, "#f")) {
		return eval(car(cdr(cdr(args))), frame);
	}

	return eval(car(cdr(args)), frame);
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
		printf("Invalid define, must have exactly 2 arguments\n");
		texit(1);
	}
	Value *returnValue = makeNull();
	returnValue->type = VOID_TYPE;

	if (car(args)->type != SYMBOL_TYPE) {
		printf("Can only define symbols.\n");
		texit(1);
	}
	
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

Value *evalLambda(Value *args, Frame *frame) {
	if(length(args) != 2) {
		printf("Invalid lambda, must have exactly 2 arguments\n");
		texit(1);
	}
	Value *lambdaValue = makeNull();
	lambdaValue->type = CLOSURE_TYPE;

	Value *param = car(args);
	if(param->type != CONS_TYPE && param->type != NULL_TYPE) {
		printf("Invalid lambda paramaters, needs to be a list of symbols\n");
		texit(1);
	}

	Value *iter;
	while(!isNull(param)) {
		if(car(param)->type != SYMBOL_TYPE) {
			printf("Invalid lambda paramaters, needs to be a list of symbols\n");
        	texit(1);
		}
		if (contains(cdr(param), car(param))) {
			printf("Invalid lambda parameters, cannot have more than one symbol of the same type.\n");
			texit(1);
		}
		param = cdr(param);
	}

	lambdaValue->closure.args = car(args);
	lambdaValue->closure.body = car(cdr(args));
	lambdaValue->closure.frame = frame;
	return lambdaValue;
}

Value *apply(Value *function, Value *args) {
	if (function->type == PRIMITIVE_TYPE) {
		return (function->pf)(args);
	}
	
	if (function->type != CLOSURE_TYPE) {
		printf("Invalid combination, must start with a function\n");
		texit(1);
	}
	Frame *curFrame = talloc(sizeof(Frame));
	curFrame->parent = function->closure.frame;
	curFrame->bindings = makeNull();

	Value *param = function->closure.args;
	while(!isNull(param)) {
		if(isNull(args)) {
			printf("Invalid function call, not enough arguments\n");
			texit(1);
		}
		Value *binding = cons(car(param), car(args));
		curFrame->bindings = cons(binding, curFrame->bindings);

		param = cdr(param);
		args = cdr(args);
	}
	if(!isNull(args)) {
		printf("Invalid function call, too many arguments\n");
		texit(1);
	}

	return eval(function->closure.body, curFrame);
}

Value *eval(Value *expr, Frame *frame) {
	switch(expr->type) {
		case INT_TYPE:
		case DOUBLE_TYPE:
		case STR_TYPE:
		case BOOL_TYPE:
		case CLOSURE_TYPE:
			return expr;
		case SYMBOL_TYPE:
			return lookUpSymbol(expr, frame);
		case CONS_TYPE: {
			Value *first = car(expr);
			Value *args = cdr(expr);
		
			if (first->type == CONS_TYPE) {
				first = eval(first, frame);
			}

			if (first->type == CLOSURE_TYPE) {
                Value *newArgs = makeNull();
                while(!isNull(args)) {
                    Value *arg = eval(car(args), frame);
                    newArgs = cons(arg, newArgs);
                    args = cdr(args);
                }
                newArgs = reverse(newArgs);
				return apply(first, newArgs);
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

			else if(!strcmp(first->s, "lambda")) {
				return evalLambda(args, frame);
			}

			// ... further special forms here ...

			else {
	            args = expr;
				Value *newArgs = makeNull();
    	        while(!isNull(args)) {
                 	Value *arg = eval(car(args), frame);
            	    newArgs = cons(arg, newArgs);
					args = cdr(args);
                }
				newArgs = reverse(newArgs);
                return apply(car(newArgs), cdr(newArgs));

				//printf("Function %s not recognized\n", first->s);
				//texit(1);
			}

			return NULL;  // to make the compiler happy
		}
		default:
			printf("Invalid token for evaluation\n");
			texit(1);
			return NULL;  // To make the compiler happy
	}
}
