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
#include <signal.h>
#include "helpers.h"
#include "./common_socket_functions.h"

// typedef struct
// {
//     int socketFD;
//     int pid;
// } User;

// All constant definitions
#define PORT 12345
#define MAX_CONNECTIONS 5
#define WELCOME_MESSAAGE "Hello from server!\n"

// Function Prototypes
int initSocket(int *socketOpt, struct sockaddr_in *address);
int createSocket();
void setSocketOptions(int socketFD, int *socketOpt);
void bindSocket(struct sockaddr_in *address, int socketFD);
void listenForConnections(int serverSocketFd);
void handleConnection(int clientSocketFd);
void reverseString(char *str);
void swap(char *a, char *b);
void sigChildHandler(int sig);

int main()
{
    int socketOpt = 1;
    struct sockaddr_in serverAddress;
    int serverSocketFd = initSocket(&socketOpt, &serverAddress);

    signal(SIGCHLD, sigChildHandler);
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
    printf("Server socket created. FD: %d\n", socketFD);

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
    int pid;

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

        // Create a child process for that client
        pid = fork();
        if (pid < 0)
        {
            errorHandler("Failed to create child process\n");
        }
        else if (pid == 0)
        {
            // Child process
            printf("Child process: FD: %d\n", clientSocketFd);
            handleConnection(clientSocketFd);
        }
    }
}

void handleConnection(int clientSocketFd)
{
    char response[MAX_STR_LEN];
    bool isResponseValid;

    printf("\n");

    while (true)
    {
        isResponseValid = receiveStuff(clientSocketFd, response);
        if (!isResponseValid)
        {
            // printf("Socket %d is empty\n", clientSocketFd);
            break;
        }
        printf("[Client(%d)]: %s\n", clientSocketFd, response);

        reverseString(response);

        printf("[Server(%d)]: %s\n", clientSocketFd, response);
        sendStuff(clientSocketFd, response);
    }
    close(clientSocketFd);
    printf("Socket %d connection closed\n", clientSocketFd);

    exit(0);
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

void sigChildHandler(int sig)
{
    printf("Caught SIGCHILD (%d)\n", sig);

    return;
}