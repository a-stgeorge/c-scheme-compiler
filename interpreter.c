#include <stdio.h>
#include <string.h>
#include "interpreter.h"
#include <math.h>

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


// ========== PRIMITIVE FUNCTION DEFINITIONS ==========

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

Value *primitiveMultiply(Value *args) {
    double product = 1;
    bool hasDoubles = false;
    // error checking on args, a list of inputs
    // compute the result as a single value
	Value *cur = args;
    while (!isNull(cur)) {
        if (car(cur)->type == INT_TYPE) {
            product *= car(cur)->i;
        } else if (car(cur)->type == DOUBLE_TYPE) {
            hasDoubles = true;
            product *= car(cur)->d;
        } else {
            printf("* can only take in numbers\n");
            texit(1);
        }
        cur = cdr(cur);
    }
    Value *returnValue = makeNull();
    if (hasDoubles) {
        returnValue->type = DOUBLE_TYPE;
        returnValue->d = product;
    } else {
        returnValue->type = INT_TYPE;
        returnValue->i = (int) product;
    }
    return returnValue;
}

Value *primitiveSubtract(Value *args) {
    double remainder = 0;
    bool hasDoubles = false;
    // error checking on args, a list of inputs
    // compute the result as a single value
    Value *cur = args;
	if (length(cur) > 1) {
		if (car(cur)->type == INT_TYPE) {
	            remainder = car(cur)->i;
	        } else if (car(cur)->type == DOUBLE_TYPE) {
	            hasDoubles = true;
	            remainder = car(cur)->d;
	        } else {
	            printf("- can only take in numbers\n");
	            texit(1);
	        }
	        cur = cdr(cur);
	}
    while (!isNull(cur)) {
        if (car(cur)->type == INT_TYPE) {
            remainder -= car(cur)->i;
        } else if (car(cur)->type == DOUBLE_TYPE) {
            hasDoubles = true;
            remainder -= car(cur)->d;
        } else {
            printf("- can only take in numbers\n");
            texit(1);
        }
        cur = cdr(cur);
    }
    Value *returnValue = makeNull();
    if (hasDoubles) {
        returnValue->type = DOUBLE_TYPE;
        returnValue->d = remainder;
    } else {
        returnValue->type = INT_TYPE;
        returnValue->i = (int) remainder;
    }
    return returnValue;
}

Value *primitiveDivide(Value *args) {
    double quotient = 1;
    // error checking on args, a list of inputs
    //compute the result as a single value
    Value *cur = args;
    if (length(cur) > 1) {
        if (car(cur)->type == INT_TYPE) {
                quotient = car(cur)->i;
            } else if (car(cur)->type == DOUBLE_TYPE) {
                quotient = car(cur)->d;
            } else {
                printf("/ can only take in numbers\n");
                texit(1);
            }
            cur = cdr(cur);
    }
    while (!isNull(cur)) {
        if (car(cur)->type == INT_TYPE) {
            quotient /= car(cur)->i;
        } else if (car(cur)->type == DOUBLE_TYPE) {
            quotient /= car(cur)->d;
        } else {
            printf("/ can only take in numbers\n");
            texit(1);
        }
        cur = cdr(cur);
    }
    Value *returnValue = makeNull();
    returnValue->type = DOUBLE_TYPE;
    returnValue->d = quotient;
    return returnValue;
}

Value *libraryMod(Value *args) {
    if (length(args) != 2) {
		printf("modulo must be given 2 arguments\n");
		texit(1);
	}
	if(car(args)->type == DOUBLE_TYPE) {
		if(floor(car(args)->i) == car(args)->i) {
			car(args)->type = INT_TYPE;
			car(args)->i = car(args)->d;
		}
		else {
			printf("expected integer values and double values were received\n");
			texit(1);
		}
	}
	if(car(cdr(args))->type == DOUBLE_TYPE) {
		if(floor(car(cdr(args))->d) == car(cdr(args))->d) {
			car(cdr(args))->type = INT_TYPE;
			car(cdr(args))->i = car(cdr(args))->d;
		}
		else {
			printf("expected integer values and double values were received\n");
			texit(1);
		}
	}
	if (car(args)->type != INT_TYPE || car(cdr(args))->type != INT_TYPE) {
		printf("modulo can only take in integers\n");
		texit(1);
	}
	if(car(cdr(args))->i == 0) {
		printf("Cannot divide by 0\n");
		texit(1);
	}
	
    Value *returnValue = makeNull();
    returnValue->type = INT_TYPE;
    returnValue->i = car(args)->i % car(cdr(args))->i;
    return returnValue;
}

Value *primitiveIsNull(Value *args) {
	if(length(args) != 1) {
		printf("null? can only have one argument\n");
		texit(1);
	}
	
	Value *result = makeNull();
	result->type = BOOL_TYPE;
	if(isNull(car(args))) {
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
	if(car(args)->type != CONS_TYPE) {
		printf("car function expects a list\n");
		texit(1);
	}
	return car(car(args));
}

Value *primitiveCdr(Value *args) {
	if(length(args) != 1) {
		printf("cdr can only have one argument\n");
		texit(1);
	}
	if(car(args)->type != CONS_TYPE) {
		printf("cdr function expects a list\n");
		texit(1);
	}
	return cdr(car(args));
}

Value *primitiveCons(Value *args) {
	if(length(args) != 2) {
		printf("Cons must have exactly two arguments.\n");
        texit(1);
	}

	return cons(car(args), car(cdr(args)));
}

Value *primitiveLessOrEq(Value *args) {
	if(length(args) != 2) {
		printf("<= must have exactly 2 arguments\n");
		texit(1);
	}
	if((car(args)->type != INT_TYPE && car(args)->type != DOUBLE_TYPE) || (car(cdr(args))->type != INT_TYPE && car(cdr(args))->type != DOUBLE_TYPE)) {
		printf("Invalid arguments, <= handles numeric comparisons only\n");
		texit(1);
	}
	
	double arg1 = 0;
	double arg2 = 0;
	if(car(args)->type == DOUBLE_TYPE) {
		arg1 = car(args)->d;
	}
	else {
		arg1 = car(args)->i;
	}
	if(car(cdr(args))->type == DOUBLE_TYPE) {
		arg2 = car(cdr(args))->d;
	}
	else {
		arg2 = car(cdr(args))->i;
	}
	
	Value* returnValue = makeNull();
	returnValue->type = BOOL_TYPE;
	if(car(args) <= car(cdr(args))) {
		returnValue->s = "#t";
	}
	else {
		returnValue->s = "#f";
	}
	return returnValue;
}

Value *primitiveIsEq(Value *args) {
	bool isEq = true;
	if(length(args) < 2) {
        printf("Eq? must have at least 2 arguments.\n");
        texit(1);
    }
	Value *first = car(args);
	Value *cur = cdr(args);
	while(!isNull(cur)) {
		if(car(cur) != first) {
			isEq = false;
		}
	}
	
	Value *returnValue = makeNull();
    returnValue->type = BOOL_TYPE;
    if(isEq) {
		returnValue->s = "#t";
	}
	else {
		returnValue->s = "#f";
	}
    return returnValue;
}

Value *primitiveIsPair(Value *args) {
	if(length(args) != 1) {
		printf("pair? must have exactly one argument.\n");
		texit(1);
	}
	Value *returnValue = makeNull();
	returnValue->type = BOOL_TYPE;
	if(length(car(args)) >= 2) {
		returnValue->s = "#t";
	}
	else {
		returnValue->s = "#f";
	}
	return returnValue;
}

Value *apply(Value *function, Value *args);

Value *primitiveApply(Value *args) {
	if(length(args) != 2) {
        printf("apply must have exactly two arguments.\n");
        texit(1);
    }
	if(car(cdr(args))->type != CONS_TYPE) {
		printf("The last argument of apply must be a list.\n");
		texit(1);
	}
	return apply(car(args), car(cdr(args)));
}

Value *libraryNEqual (Value* args) {
	if(length(args) != 2) {
		printf("= must have exactly 2 arguments\n");
		texit(1);
	}
	if((car(args)->type != INT_TYPE && car(args)->type != DOUBLE_TYPE) || (car(cdr(args))->type != INT_TYPE && car(cdr(args))->type != DOUBLE_TYPE)) {
		printf("Invalid arguments, = handles numeric comparisons only\n");
		texit(1);
	}
	
	double arg1 = 0;
	double arg2 = 0;
	if(car(args)->type == DOUBLE_TYPE) {
		arg1 = car(args)->d;
	}
	else {
		arg1 = car(args)->i;
	}
	if(car(cdr(args))->type == DOUBLE_TYPE) {
		arg2 = car(cdr(args))->d;
	}
	else {
		arg2 = car(cdr(args))->i;
	}
	
	Value *returnValue = makeNull();
	returnValue->type = BOOL_TYPE;
	if(arg1 == arg2) {
		returnValue->s = "#t";
	}
	else {
		returnValue->s = "#f";
	}
	return returnValue;
}

Value *libraryEqual(Value *args) {
	if(length(args) != 2) {
		printf("equal? must have exactly 2 arguments\n");
		texit(1);
	}
	
	Value *arg1 = car(args);
	Value *arg2 = car(cdr(args));
	
	Value *returnValue = makeNull();
	returnValue->type = BOOL_TYPE;
	returnValue->s = "#t";
	
	if(arg1->type == CONS_TYPE && arg2->type == CONS_TYPE) {
		if(length(arg1) != length(arg2)) {
			returnValue->s = "#f";
		}
		Value *cur1 = arg1;
		Value *cur2 = arg2;
		while(cur1) {
			Value *result = libraryEqual(cons(cur1, cur2));
			if(!strcmp("#f", result->s)) {
				returnValue->s = "#f";
				return returnValue;
			}
			cur1 = cdr(cur1);
			cur2 = cdr(cur2);
		}
		returnValue->s = "#t";
		return returnValue;
	} else if(arg1->type == CONS_TYPE || arg2->type == CONS_TYPE) {
		returnValue->s = "#f";
	}
	else {
		/*if(equal(arg1, arg2)) {
			returnValue->s = "#t";
		}*/
	}
	return returnValue;
}


// ========== MAIN INTERPRET FUNCITON ==========

void interpret(Value *tree) {
	Frame *parentFrame = talloc(sizeof(Frame));
	parentFrame->bindings = makeNull();
	parentFrame->parent = NULL;
	bind("+", primitiveAdd, parentFrame);
    bind("*", primitiveMultiply, parentFrame);
    bind("-", primitiveSubtract, parentFrame);
    bind("/", primitiveDivide, parentFrame);
	bind("<=", primitiveLessOrEq, parentFrame);
	bind("modulo", libraryMod, parentFrame);
	bind("null?", primitiveIsNull, parentFrame);
	bind("car", primitiveCar, parentFrame);
	bind("cdr", primitiveCdr, parentFrame);
	bind("cons", primitiveCons, parentFrame);
	bind("eq?", primitiveIsEq, parentFrame);
	bind("pair?", primitiveIsPair, parentFrame);
	bind("apply", primitiveApply, parentFrame);
	bind("=", libraryNEqual, parentFrame);
	bind("equal?", libraryEqual, parentFrame);

	while(!isNull(tree)) {
		Value *result = eval(car(tree), parentFrame);
		if (result->type != VOID_TYPE) {
			printTree(result);
		}
		tree = cdr(tree);
	}
}


// ========== SPECIAL FORMS HELPER FUNCTIONS ==========

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
	if (length(args) != 3 && length(args) != 2) {
		printf("Invalid if statement, must have 2 or 3 arguments\n");
		texit(1);
	}

	Value *cond = eval(car(args), frame);
	if(length(args) == 3) {
		if (cond->type == BOOL_TYPE && !strcmp(cond->s, "#f")) {
			return eval(car(cdr(cdr(args))), frame);
		}

		return eval(car(cdr(args)), frame);
	}
	else {
		if(cond->type == BOOL_TYPE && !strcmp(cond->s, "#f")) {
			Value *returnValue = makeNull();
			returnValue->type = VOID_TYPE;
			return returnValue;
		}
		return eval(car(cdr(args)), frame);
	}
}

Value *evalQuote(Value *args) {
	if (length(args) != 1) {
		printf("quote must have one argument\n");
		texit(1);
	}
	return car(args);
}

Value *evalLet(Value *args, Frame *frame, int type) { // type == 0 let, type == 1 letrec type == 2 let*
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

		Value *result;
		if(type == 0 || type == 2) {
			result = eval(car(cdr(car(binding))), frame);
		} else {
			result = eval(car(cdr(car(binding))), newFrame);
		}
		Value *symbol = car(car(binding));
		if (symbol->type != SYMBOL_TYPE) {
			printf("Let must define symbol types only\n");
			texit(1);
		}
		Value *newBinding = cons(symbol, result);
		newFrame->bindings = cons(newBinding, newFrame->bindings);

		binding = cdr(binding);
		if(type == 2) {
			frame = newFrame;
			newFrame = talloc(sizeof(Frame));
			newFrame->bindings = makeNull();
			newFrame->parent = frame;
		}
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
	
	if(param->type != CONS_TYPE && param->type != SYMBOL_TYPE && param->type != NULL_TYPE) {
		printf("Invalid lambda paramaters, needs to be a list of symbols\n");
		texit(1);
	} 
	else if(param->type == SYMBOL_TYPE) {
		lambdaValue->closure.variadic = true;
	}
	else {
		lambdaValue->closure.variadic = false;
	
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
	}
	lambdaValue->closure.args = car(args);
	lambdaValue->closure.body = car(cdr(args));
	lambdaValue->closure.frame = frame;
	return lambdaValue;
}

Value *evalBegin(Value *args, Frame *frame) {
	Value *cur = args;
	Value *result = makeNull();
	result-> type = VOID_TYPE;
	while (cur->type == CONS_TYPE) {
		result = eval(car(cur), frame);
		cur = cdr(cur);
	}
	if (!isNull(cur)) {
		printf("Invalid arguments for for begin\n");
		texit(1);
	}

	return result;
}

Value *evalAnd(Value *args, Frame *frame) {
	Value *cur = args;
	Value *result = makeNull();
	result->type = BOOL_TYPE;

	if(cur->type != CONS_TYPE && cur->type != NULL_TYPE) {
		printf("Invalid input parameters\n");
		texit(1);
	}
	
	if(cur->type == NULL_TYPE) {
		result->s = "#t";
		return result;
	}
	
	while(cur->type == CONS_TYPE) {
		Value *cond = eval(car(cur), frame);
		if(cond->type == BOOL_TYPE && !strcmp(cond->s, "#f")) {
			result->s = "#f";
			return result;
		}
		cur = cdr(cur);
	}
	result->s = "#t";
	return result;
}

Value *evalOr(Value *args, Frame *frame) {
	Value *cur = args;
	Value *result = makeNull();
	result->type = BOOL_TYPE;

	if(cur->type != CONS_TYPE && cur->type != NULL_TYPE) {
		printf("Invalid input parameters\n");
		texit(1);
	}
	
	if(cur->type == NULL_TYPE) {
		result->s = "#t";
		return result;
	}
	
	while(cur->type == CONS_TYPE) {
		Value *cond = eval(car(cur), frame);
		if(cond->type == BOOL_TYPE && !strcmp(cond->s, "#t")) {
			result->s = "#t";
			return result;
		}
		cur = cdr(cur);
	}
	result->s = "#f";
	return result;
}



// ========== APPLY FUNCTION ==========

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
	if(function->closure.variadic) {
		Value *binding = cons(param, args);
		curFrame->bindings = cons(binding, curFrame->bindings);
	}
	else {
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
	}
	return eval(function->closure.body, curFrame);
}


// ========== MAIN EVALUATION FUNCTION ==========

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
				return evalLet(args, frame, 0);
			}

			else if (!strcmp(first->s, "letrec")) {
				return evalLet(args, frame, 1);
			}

			else if (!strcmp(first->s, "let*")) {
				return evalLet(args, frame, 2);
			}

			else if (!strcmp(first->s, "define")) {
				return evalDefine(args, frame);
			}

			else if (!strcmp(first->s, "lambda")) {
				return evalLambda(args, frame);
			}

			else if (!strcmp(first->s, "begin")) {
				return evalBegin(args, frame);
			}
			
			else if(!strcmp(first->s, "and")) {
				return evalAnd(args, frame);
			}
			
			else if(!strcmp(first->s, "or")) {
				return evalOr(args, frame);
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

			}

			return NULL;  // to make the compiler happy
		}
		default:
			printf("Invalid token (type %d) for evaluation\n", expr->type);
			texit(1);
			return NULL;  // To make the compiler happy
	}
}
