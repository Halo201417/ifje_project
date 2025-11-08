CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -D_POSIX_C_SOURCE=200809L
SRCS = src/main.c src/lexer.c src/parser.c src/ast.c src/printer.c
OBJS = $(SRCS:.c=.o)
TARGET = expr

.PHONY: all clean test

all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) out1.txt

test: all
	@./$(TARGET) < tests/test1.in > out1.txt
	@if ! diff -u tests/test1.out out1.txt > /dev/null ; then \
	  echo "TEST FAILED"; exit 1; \
	else \
	  echo "Tests passed"; \
	fi
