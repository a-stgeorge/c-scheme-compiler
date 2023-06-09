#include "parser.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

struct Frame {
   Value *bindings;
   struct Frame *parent;
};
typedef struct Frame Frame;

void *interpret(Value *tree);

void *interpretUseFrame(Value *tree, Frame *frame);

Value *eval(Value *expr, Frame *frame);

#endif
