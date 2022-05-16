#include "value.h"
#include "tokenizer.h"
#include "talloc.h"
#include "parser.h"
#include "interpreter.h"

#include <stdio.h>

int main(void) {
    Value *list = tokenize(stdin);
    Value *tree = parse(list);
    interpret(tree);
    tfree();
    return 0;
}
