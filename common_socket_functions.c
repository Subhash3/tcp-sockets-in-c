#include "./common_socket_functions.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "helpers.h"

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