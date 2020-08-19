#
# Makefile for the UM Test lab
# 
CC = gcc

IFLAGS  = -I/comp/40/build/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64
LDLIBS  = -l40locality -lcii40 -lm

EXECS   = writetests test_memarray um

all: $(EXECS)

writetests: umlabwrite.o umlab.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test_memarray: test_memarray.o memarray.o memory.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

um: um.o memarray.o memory.o decipher.o operate.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o

