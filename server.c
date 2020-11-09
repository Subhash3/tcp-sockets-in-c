#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "helpers.h"

// All constant definitions
#define PORT 12345
#define MAX_CONNECTIONS 5
#define WELCOME_MESSAAGE "Hello from server!\n"
#define MAX_STR_LEN 256

// Function Prototypes
int initSocket(int *socketOpt, struct sockaddr_in *address);
int createSocket();
void setSocketOptions(int socketFD, int *socketOpt);
void bindSocket(struct sockaddr_in *address, int socketFD);
void listenForConnections(int serverSocketFd);
void handleConnection(int clientSocketFd);
void receiveStuff(int socketFD, char *string);
void sendStuff(int socketFD, char *string);
void reverseString(char *str);
void swap(char *a, char *b);

int main()
{
    int socketOpt = 1;
    struct sockaddr_in serverAddress;
    int serverSocketFd = initSocket(&socketOpt, &serverAddress);

    listenForConnections(serverSocketFd);

    exit(0);
}

int initSocket(int *socketOpt, struct sockaddr_in *address)
{
    int socketFD, flag;

    socketFD = createSocket();
    setSocketOptions(socketFD, socketOpt);
    bindSocket(address, socketFD);

    return socketFD;
}

int createSocket()
{
    int socketFD;

    printf("Creating socket..\n");
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0)
    {
        errorHandler("Couldn't create a socket\n");
        exit(errno);
    }
    printf("Server socket created.\n");

    return socketFD;
}

void setSocketOptions(int socketFD, int *socketOpt)
{
    int flag;

    printf("Setting socket options...\n");
    flag = setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, socketOpt, sizeof(*socketOpt));

    switch (flag)
    {
    case EBADF:
        errorHandler("Invalid File (Socket) Descriptor.\n");
        break;
    case EFAULT:
        errorHandler("Invalid Socket Options.\n");
    case EINVAL:
        break;
    case ENOPROTOOPT:
        errorHandler("Unknows socket option at the indicated level.\n");
        break;
    case ENOTSOCK:
        errorHandler("Given File descriptor is not a valid socket.\n");
        break;
    default:
        printf("Socket options have been set!\n");
        return;
    }

    exit(errno);
}

void bindSocket(struct sockaddr_in *address, int socketFD)
{
    int flag;

    printf("Binding socket...\n");
    bzero(address, sizeof(*address));

    (*address).sin_family = AF_INET;
    (*address).sin_addr.s_addr = htonl(INADDR_ANY);
    (*address).sin_port = htons(PORT);

    flag = bind(socketFD, (struct sockaddr *)address, sizeof(*address));
    if (flag < 0)
    {
        errorHandler("Coudn't bind socket\n");
        exit(errno);
    }
    printf("Socket Bound successfully.\n");

    return;
}

void listenForConnections(int serverSocketFd)
{
    int flag, clientSocketFd;

    printf("listening for connections...\n");

    flag = listen(serverSocketFd, MAX_CONNECTIONS);
    if (flag < 0)
    {
        errorHandler("Listen failed.\n");
        exit(errno);
    }

    while (true)
    {
        struct sockaddr_in addr;
        socklen_t addressLength;
        char clientAddress[MAX_STR_LEN];

        clientSocketFd = accept(serverSocketFd, (struct sockaddr *)&addr, &addressLength);
        inet_ntop(AF_INET, &addr, clientAddress, MAX_STR_LEN);
        printf("Connection Established. Client Address: %s, socket FD: %d\n", clientAddress, clientSocketFd);

        handleConnection(clientSocketFd);
    }
}

void handleConnection(int clientSocketFd)
{
    char response[MAX_STR_LEN];

    printf("\n");

    while (true)
    {
        receiveStuff(clientSocketFd, response);
        printf("[Client]: %s\n", response);

        reverseString(response);

        printf("[Server]: %s\n", response);
        sendStuff(clientSocketFd, response);
    }
    close(clientSocketFd);

    return;
}

void receiveStuff(int socketFD, char *string)
{
    int n;

    memset(string, 0, MAX_STR_LEN);
    n = read(socketFD, string, MAX_STR_LEN - 1);
    if (n < 0)
    {
        errorHandler("Error while reading the socket\n");
    }

    return;
}

void sendStuff(int socketFD, char *string)
{
    int flag;

    // memset(string, 0, MAX_STR_LEN - 1);
    flag = write(socketFD, string, strlen(string));
    if (flag < 0)
    {
        errorHandler("Could not write to socket\n");
    }

    return;
}

void reverseString(char *str)
{
    int length = strlen(str);

    for (int i = 0; i < length / 2; i++)
    {
        swap(&str[i], &str[length - i - 1]);
    }

    return;
}

void swap(char *a, char *b)
{
    char temp;

    temp = *a;
    *a = *b;
    *b = temp;

    return;
}