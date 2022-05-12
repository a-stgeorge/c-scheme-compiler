#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

typedef enum {
   PTR_TYPE,
   INT_TYPE,
   DOUBLE_TYPE,
   STR_TYPE,
   CONS_TYPE,
   OPEN_TYPE,
   CLOSE_TYPE,
   BOOL_TYPE,
   SYMBOL_TYPE,
   NULL_TYPE,
   VOID_TYPE,
   CLOSURE_TYPE,
   PRIMITIVE_TYPE
} valueType;

struct Value {
   valueType type;
   union {
      void *p;
      int i;
      double d;
      char *s;
      struct ConsCell {
         struct Value *car;
         struct Value *cdr;
      } c;
      struct Closure {
         struct Value *args;
		 struct Value *body;
		 struct Frame *frame;
         bool variadic;
	  } closure;
	  struct Value *(*pf)(struct Value *);
   };
};

typedef struct Value Value;

#endif
