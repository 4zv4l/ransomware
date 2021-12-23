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

void welcome();
char* net_get(int* ID);
void send_ID(SOCKET sock, int* ID);
char* get_data(SOCKET sock);
SOCKET set_socket();
SOCKADDR_IN set_addr();
