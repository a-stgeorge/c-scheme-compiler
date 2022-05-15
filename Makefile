.PHONY: memtest clean

CC = clang
CFLAGS = -g
LDFLAGS = -lm

SRCS = linkedlist.c main.c parser.c talloc.c tokenizer.c interpreter.c
HDRS = linkedlist.h value.h talloc.h tokenizer.h interpreter.h
OBJS = $(SRCS:.c=.o)


interpreter: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

memtest: interpreter
	valgrind --leak-check=full --show-leak-kinds=all ./$<

%.o : %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f interpreter
