#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#ifndef PARSE_H
#define PARSE_H

value *parse(Value *tokens);
void printTree(Value *tree);

#endif
