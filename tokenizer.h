#include <stdio.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"

#ifndef TOKENIZER_H
#define TOKENIZER_H

/*
 * Reads in code and creates a linked list of tokens based on their type.
 */
Value *tokenize(FILE *stream);

/*
 * Takes in a linked list of tokens as input and displays them with one token
 * per line.
 */
void displayTokens(Value *list);

#endif
