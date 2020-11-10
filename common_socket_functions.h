#ifndef __COMMON_SOCKET_FUNCTIONS__
#define __COMMON_SOCKET_FUNCTIONS__
#include <stdbool.h>

// Global variables
#define MAX_STR_LEN 256

// Function Prototypes
bool receiveStuff(int socketFD, char *string);
void sendStuff(int socketFD, char *string);

#endif /* __COMMON_SOCKET_FUNCTIONS__ */