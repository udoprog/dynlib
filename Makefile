.SUFFIXES: .c .o

PREFIX=/usr/local
SRC=dyn/store.c
SRC+=dyn/string.c
SRC+=dyn/array.c

INC=$(PREFIX)/include
LIB=$(PREFIX)/lib

OBJ=$(SRC:.c=.o)

CFLAGS=-std=gnu99 -pedantic -Wall -Werror -O3 -I.

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
