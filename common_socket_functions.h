#ifndef __COMMON_SOCKET_FUNCTIONS__
#define __COMMON_SOCKET_FUNCTIONS__
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

// All constant definitions
#define PORT 12345
#define MAX_STR_LEN 256

// Global variables
int SERVER_PORT;
char SERVER_IP[MAX_STR_LEN];

// Function Prototypes
int createSocket();
void setSocketOptions(int socketFD, int *socketOpt);
int initSocket(int *socketOpt, struct sockaddr_in *address, bool isServer);
void setupSocketAddress(struct sockaddr_in *address, int socketFD);
void bindSocket(struct sockaddr_in *address, int socketFD);
bool receiveStuff(int socketFD, char *string);
void sendStuff(int socketFD, char *string);

#endif /* __COMMON_SOCKET_FUNCTIONS__ */