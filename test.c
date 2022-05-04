#include "value.h"
#include "tokenizer.h"
#include "talloc.h"
#include "parser.h"
#include "interpreter.h"
#include <stdio.h>


int main() {
	Value *list = tokenize();
	displayTokens(list);
	display(list);
	Value *tree = parse(list);
	printTree(tree);
	printf("\n");
	display(tree);
}
