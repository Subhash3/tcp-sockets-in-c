CC=gcc
CFLAGS = -Wall

all: helpers.o
	$(CC) $(FLAGS) client.c $^ -o client
	$(CC) $(FLAGS) server.c $^ -o server

helpers.o:
	$(CC) $(FLAGS) helpers.c -c

clean:
	rm *.o

cleanAll: clean
	rm client
	rm server