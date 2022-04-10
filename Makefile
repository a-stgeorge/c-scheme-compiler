.PHONY: memtest clean

CC = clang
CFLAGS = -g

SRCS = linkedlist.c main_parse.c parser.c talloc.c tokenizer.c
HDRS = linkedlist.h value.h talloc.h tokenizer.h
OBJS = $(SRCS:.c=.o)

parser: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

memtest: parser
	valgrind --leak-check=full --show-leak-kinds=all ./$<

%.o : %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f parser
