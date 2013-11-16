CC=cc
CFLAGS=-c -Wall -std=c99

all: clean server

clean:
	rm -f server

.c.o:
	$(CC) $(CFLAGS) $< -o $@
