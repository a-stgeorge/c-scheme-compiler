#include "value.h"
#include "tokenizer.h"
#include "talloc.h"
#include "parser.h"
#include "interpreter.h"
#include <stdio.h>


int main() {
	Value *list = tokenize();
	displayTokens(list);
	printf("BTS Token List:\t");
	display(list);
	Value *tree = parse(list);
	printf("\nParse Tree:\t");
	printTree(tree);
	printf("\nBTS Parse Tree:\t");
	display(tree);
}
