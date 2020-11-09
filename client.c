#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "helpers.h"

// All constant definitions
#define MAX_STR_LEN 256

// Global variables
int SERVER_PORT;
char SERVER_IP[MAX_STR_LEN];

// Function Prototypes
int initSocket(int *socketOpt, struct sockaddr_in *address);
int createSocket();
void setSocketOptions(int socketID, int *socketOpt);
void setupSocketAddress(struct sockaddr_in *address, int socketID);
void preProcess(int argc, char **argv);
void connectToServer(int clientSocketFd, struct sockaddr_in *address);
void communicate(int socketFD);
void receiveStuff(int socketFD, char *string);
void sendStuff(int socketFD, char *string);

int main(int argc, char **argv)
{
    preProcess(argc, argv);
    int socketOpt = 1;
    struct sockaddr_in serverAddress;
    int socketFd = initSocket(&socketOpt, &serverAddress);

    connectToServer(socketFd, &serverAddress);
    communicate(socketFd);

    exit(0);
}

int initSocket(int *socketOpt, struct sockaddr_in *address)
{
    int socketID, flag;

    socketID = createSocket();
    setSocketOptions(socketID, socketOpt);
    setupSocketAddress(address, socketID);

    return socketID;
}

int createSocket()
{
    int socketID;

    printf("[*] Creating socket..\n");
    socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0)
    {
        errorHandler("Couldn't create a socket\n");
        exit(errno);
    }
    printf("[+] Server socket created.\n");

    return socketID;
}

void setSocketOptions(int socketID, int *socketOpt)
{
    int flag;

    printf("[*] Setting socket options...\n");
    flag = setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, socketOpt, sizeof(*socketOpt));

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
        printf("[+] Socket options have been set!\n");
        return;
    }

    exit(errno);
}

void setupSocketAddress(struct sockaddr_in *address, int socketID)
{
    int flag;

    printf("[*] Setting socket address...\n");
    bzero(address, sizeof(*address));

    (*address).sin_family = AF_INET;
    (*address).sin_port = htons(SERVER_PORT);

    flag = inet_pton(AF_INET, (const char *)SERVER_IP, &((*address).sin_addr));
    if (flag < 0)
    {
        errorHandler("inet_pton() failed. Please check the provided IP: %s\n", SERVER_IP);
    }
    printf("[+] Setup socket to %s:%d\n", SERVER_IP, SERVER_PORT);

    return;
}

void connectToServer(int clientSocketFd, struct sockaddr_in *address)
{
    int flag;

    printf("[*] Connecting to %s:%d\n", SERVER_IP, SERVER_PORT);
    flag = connect(clientSocketFd, (struct sockaddr *)address, sizeof(*address));
    if (flag < 0)
    {
        errorHandler("Could not connect to server\n");
    }
    printf("[+] Connection Established!\n");

    return;
}

void preProcess(int argc, char **argv)
{
    if (argc != 3)
    {
        errorHandler("Usage: ./%s <SERVER-IP> <SERVER-PORT>\n", argv[0]);
    }

    SERVER_PORT = atoi(argv[2]);
    strcpy(SERVER_IP, argv[1]);

    return;
}

void communicate(int socketFD)
{
    char string[MAX_STR_LEN];

    printf("\nPress CTRL+C to disconnect!\n\n");
    while (true)
    {
        printf("Enter a string: ");
        scanf("%s", string);

        printf("[Client]: %s\n", string);
        sendStuff(socketFD, string);

        receiveStuff(socketFD, string);
        printf("[Server]: %s\n", string);
    }

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

    flag = write(socketFD, string, strlen(string));
    if (flag < 0)
    {
        errorHandler("Could not write to socket\n");
    }

    return;
}