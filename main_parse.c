// Main test file for parser portion of scheme parsing project

#include <stdio.h>
#include "parser.h"

int main(void) {
	Value *list = tokenize();
	Value *tree = parse(list);
	printTree(tree);
	printf("\n");
	tfree();
	return 0;
}
