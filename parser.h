#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"

#ifndef PARSE_H
#define PARSE_H

Value *parse(Value *tokens);

void printTree(Value *tree);

#endif
