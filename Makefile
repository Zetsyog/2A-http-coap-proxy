CC?=gcc
CFLAGS?=-Werror -Wextra -Wall -g
LDLIBS?=-lpthread -lmicrohttpd -lcoap-2

EXEC=proxy
BINDIR=bin
OBJ=util.o http-server.o resource.o coap.o

all: main.c $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) -o bin/$(EXEC) $^ $(CFLAGS) $(LDLIBS)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o
	rm -f $(BINDIR)/$(EXEC)