Value *parse(Value *tokens) {
	Value *stack = makeNull();
	int depth = 0;
	
	while(!isNull(cdr(tokens))) {
		Value *token = car(tokens);
		tokens = cdr(tokens);
		
		if(token->type != CLOSE_TYPE) {
			if(token->type == OPEN_TYPE) {
				depth++;
			}
			stack = cons(token, stack);
		}
		else if (token->type == CLOSE_TYPE) {
			depth--;
			Value *cur = car(stack);
			Value *popped = makeNull();
			stack = cdr(stack);
			while(cur != OPEN_TYPE) { //TODO: check for null, proper number of open/close parens
				popped = cons(cur, popped);
				cur = car(stack);
				stack = cdr(stack);
			}
			stack = cons(popped, stack);
		}
	}
	if(depth != 0) {
		printf("Not enough closing parenthesees.");
		texit(1);
	}
	return stack;
}

void printTree(Value *tree) {
	
}
