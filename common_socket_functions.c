#include "./common_socket_functions.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include "helpers.h"

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

// Willl be used by client only
void setupSocketAddress(struct sockaddr_in *address, int socketFD)
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

// Willl be used by Server only
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

int initSocket(int *socketOpt, struct sockaddr_in *address, bool isServer)
{
    int socketFD, flag;

    socketFD = createSocket();
    setSocketOptions(socketFD, socketOpt);
    if (isServer)
    {
        bindSocket(address, socketFD);
    }
    else
    {
        setupSocketAddress(address, socketFD);
    }

    return socketFD;
}

bool receiveStuff(int socketFD, char *string)
{
    int flag;

    memset(string, 0, MAX_STR_LEN);
    flag = read(socketFD, string, MAX_STR_LEN - 1);
    // printf("Flag: %d, String: %s\n", flag, string);

    if (flag < 0)
    {
        errorHandler("Error while reading the socket\n");
    }
    if (flag == 0)
    {
        return false;
    }

    return true;
}

void sendStuff(int socketFD, char *string)
{
    int flag;

    string[strlen(string)] = '\0';
    flag = write(socketFD, string, strlen(string));
    if (flag < 0)
    {
        errorHandler("Could not write to socket\n");
    }

    return;
}