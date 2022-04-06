#include "talloc.h"
#include "linkedlist.h"
#include "tokenizer.h"

int main(void) {
	Value *list = tokenize();
	displayTokens(list);
	tfree();
	return 0;
}
