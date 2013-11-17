CC=cc
CFLAGS=-c -Wall -std=c99

LIB=$(wildcard lib/*.c)
LIB_OBJECTS=$(LIB:.c=.o)

SERVER=server.c
SERVER_OBJECTS=$(SERVER:.c=.o)
SERVER_EXECUTABLE=server

CLIENT=client.c
CLIENT_OBJECTS=$(CLIENT:.c=.o)
CLIENT_EXECUTABLE=client

all: clean $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)

$(SERVER_EXECUTABLE): $(LIB_OBJECTS) $(SERVER_OBJECTS)  
	$(CC) $^ -o $@

$(CLIENT_EXECUTABLE): $(LIB_OBJECTS) $(CLIENT_OBJECTS)  
	$(CC) $^ -o $@

clean:
	rm -f server
	rm -f $(LIB_OBJECTS) $(SERVER_OBJECTS) $(CLIENT_OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean
