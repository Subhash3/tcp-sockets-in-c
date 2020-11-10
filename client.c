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
#include "./common_socket_functions.h"

// Function Prototypes
void preProcess(int argc, char **argv);
void connectToServer(int clientSocketFd, struct sockaddr_in *address);
void communicate(int socketFD);

int main(int argc, char **argv)
{
    preProcess(argc, argv);
    int socketOpt = 1;
    struct sockaddr_in serverAddress;
    int socketFd = initSocket(&socketOpt, &serverAddress, false);

    connectToServer(socketFd, &serverAddress);
    communicate(socketFd);

    exit(0);
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
