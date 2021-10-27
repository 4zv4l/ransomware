#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>
#define PORT 8080
#define FILE_NAME "caught.key"
#define LENGTH 1024
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

void save(int ID, char* key){
  FILE* f = fopen(FILE_NAME, "a+");
  if(f == NULL){
    perror("Fopen()");
  }
  fprintf(f,
      "--- Beginning : %d ---\n"
      "%s\n"
      "--- Ending ---\n"
      ,ID,key);
  fclose(f);
}

char* gen_key(int ID){
  srand(ID);
  char element[] = "abcdef,?gh:;ijklmno=+~pqrstuvwxyz^^[]$*{})-_@";
  char* key = calloc(1,LENGTH);
  int i;
  for(i = 0; i < LENGTH; i++){
    key[i] = element[rand()%strlen(element)];
  }
  save(ID, key);
  return key;
}

int checkID(int ID){
  FILE* f = fopen(FILE_NAME, "r+");
  if(f == NULL){
    // perror("Read()");
    return 1;
  }
  char char_ID[11];
  sprintf(char_ID, "%d", ID);
  char* line = malloc(27);
  while(fgets(line, 27, f)){
    line = line+16;
    line[strlen(line)] = '\0';
    if(!strcmp(line,char_ID)){
      return 0;
    }
    // skip the lines with no ID
    while(getc(f)!='\n');
    while(getc(f)!='\n');
    while(getc(f)!='\n');
    line = line-16;
  }
  free(line);
  fclose(f);
  return 1;
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
        if(checkID(ID) == 0){
          printf("ID already in...\n");
          char bad_ID[LENGTH];
          sprintf(bad_ID, "%s", "Segmentation fault (core dumped)");
          send(csock, bad_ID, LENGTH, 0);
          return;
        }
        //printf("[+] Got the ID -> %d\n", ID);
// gen key
        char* key = gen_key(ID);
// send data
	//printf("[+]Gotcha Client!\n");
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

int main(){
// create server socket + bind
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // ip tcp 
	if (sock == -1) {
		perror("socket()");
		return 3;
	}
	SOCKADDR_IN sin = {0};
	sin.sin_addr.s_addr = htonl(INADDR_ANY); // on accept sur toutes les addr
	sin.sin_family = AF_INET; // ip addr
	sin.sin_port = htons(PORT); // port
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
	//printf("[+]Listening....\n");
	while(1) handleClients(sock);
        //printf("[-]Bye-bye Client!\n");
	close(sock);
	//printf("[-]Server closed with success!\n");
}
