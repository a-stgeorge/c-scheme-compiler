// Parses scheme code into a parse tree from an ordered list of tokens
// Authors: Jon, Isaac, Aidan

#include <stdio.h>
#include <assert.h>
#include "parser.h"

Value *parse(Value *tokens) {
	Value *stack = makeNull();
	int depth = 0;
	
	assert(tokens != NULL && "Error (parse): null pointer");
	while (!isNull(tokens)) {
		Value *token = car(tokens);
		
		if(token->type != CLOSE_TYPE) {
			if(token->type == OPEN_TYPE) {
				depth++;
			}
			stack = cons(token, stack);
		}
		else if (token->type == CLOSE_TYPE) {
			depth--;
			if (depth < 0) {
				printf("Syntax error: too many close parentheses\n");
				texit(1);
			}
			Value *cur = car(stack);
			Value *popped = makeNull();
			stack = cdr(stack);
			while(cur->type != OPEN_TYPE) {
				popped = cons(cur, popped);
				cur = car(stack);
				stack = cdr(stack);
			}
			stack = cons(popped, stack);
		}	

		tokens = cdr(tokens);
	}
	if(depth != 0) {
		printf("Syntax error: not enough close parentheses\n");
		texit(1);
	}
	return reverse(stack);
}

void printTree(Value *tree) {
	if (isNull(tree)) {
	
	}

	else if (tree->type != CONS_TYPE) {
		switch (tree->type) {
		case STR_TYPE:
		case BOOL_TYPE:
		case SYMBOL_TYPE:
			printf("%s", tree->s);
			break;
		case INT_TYPE:
			printf("%i", tree->i);
			break;
		case DOUBLE_TYPE:
			printf("%lf", tree->d);
			break;
		default:
			// Don't handle other cases
			break;
		}
	}
	
	else {
		if (car(tree)->type == CONS_TYPE) {
			printf("(");
			printTree(car(tree));
			printf(")");
		} else if (car(tree)->type == NULL_TYPE) {
			printf("()");
		} else {
			printTree(car(tree));
		}
		
		if (cdr(tree)->type != NULL_TYPE) {
			printf(" ");
		}

		printTree(cdr(tree));
	}

}

