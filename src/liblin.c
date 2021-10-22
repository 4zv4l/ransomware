#include "liblin.h"
#include <stdlib.h>

void welcome(){
  printf("Linux detected, let's go !\n\n");
}

char* net_get(){
  // create client socket
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // ip tcp 
  if (sock == -1) {
    perror("socket()");
    return 0;
  }
  SOCKADDR_IN sin = {0};
  sin.sin_addr.s_addr = inet_addr(IP); // set the ip
  sin.sin_port = htons(PORT); // set port
  sin.sin_family = AF_INET; // ip protocol
  int flag = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  // connect to the server
  if(connect(sock, (SOCKADDR *)&sin, sizeof(sin)) == -1){
    perror("[-] connect()");
  }else{
    printf("[+] Connected !\n");
  }
  // receive data
  int len = 25;
  char* key = calloc(1,len);
  if(recv(sock, key, len,0) == -1){
    printf("[-]Failed receive the key from the server!\n");
  } else {
    printf("[+] Receied key from the server with success!\n");
  }
  printf("-> %s", key);
  // close the socket
  close(sock);
  printf("[-]Client closed with success!\n");
  return key;
}
