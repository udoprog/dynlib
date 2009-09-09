.SUFFIXES: .c .o

PREFIX=/usr/local
SRC=dyn/store.c
SRC+=dyn/string.c
SRC+=dyn/array.c
SRC+=dyn/var.c
SRC+=dyn/btree.c

INC=$(PREFIX)/include
LIB=$(PREFIX)/lib

OBJ=$(SRC:.c=.o)

CFLAGS=-std=gnu99 -pedantic -Wall -Werror -O3 -I. -g

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) main.c -o test-all

static: $(OBJ)
	ar r libdyn.a $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) $(OBJ)
	$(RM) libdyn.a

install: static
	install -m 0755 libdyn.a $(LIB)/libdyn.a
	install -D -m 0755 dyn/var.h 		$(INC)/dyn/var.h
	install -D -m 0755 dyn/store.h 	$(INC)/dyn/store.h
	install -D -m 0755 dyn/string.h $(INC)/dyn/string.h

ex1:
	$(CC) $(CFLAGS) examples/example1.c -o example1 -ldyn

ex2:
	$(CC) $(CFLAGS) examples/example2.c -o example2 -ldyn

ex3:
	$(CC) $(CFLAGS) examples/example3.c -o example3 -ldyn

ex4:
	$(CC) $(CFLAGS) examples/example4.c -o example4 -ldyn

