.SUFFIXES: .c .o

PREFIX=/usr/local
SRC=dyn/store.c
SRC+=dyn/string.c
SRC+=dyn/array.c
#SRC+=main.c

INC=$(PREFIX)/include
LIB=$(PREFIX)/lib

OBJ=$(SRC:.c=.o)

CFLAGS=-std=gnu99 -pedantic -Wall -Werror -g -I.

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o test-all

static: $(OBJ)
	ar r libdyn.a $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) $(OBJ)

install: static
	install -m 0755 libdyn.a $(LIB)/libdyn.a
	install -D -m 0755 dyn/store.h $(INC)/dyn/store.h
	install -D -m 0755 dyn/string.h $(INC)/dyn/string.h
