#include "./common_socket_functions.h"
#include "helpers.h"
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void receiveStuff(int socketFD, char *string)
{
    int flag, nBytes;

    memset(string, 0, MAX_STR_LEN);
    while (true)
    {
        flag = read(socketFD, string, 1);
        if (flag < 0)
        {
            errorHandler("Error while reading the socket\n");
        }

        nBytes += 1;
        if (flag == 0 || nBytes >= MAX_STR_LEN - 1)
        {
            break;
        }
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