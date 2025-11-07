CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
SRCS = src/main.c src/lexer.c src/parser.c src/ast.c src/printer.c
OBJS = $(SRCS:.c=.o)
TARGET = expr

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)

test: all
	./$(TARGET) < tests/test1.in > out1.txt
	diff -u tests/test1.out out1.txt || (echo "TEST FAILED" && exit 1)
	@echo "Tests passed"
