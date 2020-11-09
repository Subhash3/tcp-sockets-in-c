CC=gcc
CFLAGS = -Wall

all: clean client server helpers.o

client: helpers.o
	$(CC) $(FLAGS) client.c $^ -o client

server: helpers.o
	$(CC) $(FLAGS) server.c $^ -o server

helpers.o:
	$(CC) $(FLAGS) helpers.c -c

clean:
	rm *.o

cleanAll: clean
	rm client
	rm server