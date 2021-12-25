#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <windows.h>

#define Directory "C:"
#define PORT 8080
#define IP "127.0.0.1"
#define LENGTH 32
//typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

// run send_ID and get_data
char* net_get(int* ID);
// send the ID to the server
void send_ID(SOCKET sock, int* ID);
// get the key from the server
char* get_data(SOCKET sock);
// set the socket and the address
SOCKET set_socket();
SOCKADDR_IN set_addr();
