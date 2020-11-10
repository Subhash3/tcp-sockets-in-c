CC=gcc
CFLAGS = -Wall

all: helpers.o common_socket_functions.o
	$(CC) $(FLAGS) client.c $^ -o client
	$(CC) $(FLAGS) server.c $^ -o server

helpers.o:
	$(CC) $(FLAGS) helpers.c -c

common_socket_functions.o:
	$(CC) $(FLAGS) common_socket_functions.c -c

clean:
	rm *.o

cleanAll: clean
	rm client
	rm server