#include "libwin.h"

SOCKADDR_IN set_addr(){
	SOCKADDR_IN sin = {0};
	sin.sin_addr.s_addr = inet_addr(IP); // set the ip
	sin.sin_port = htons(PORT); // set port
	sin.sin_family = AF_INET; // ip protocol
	return sin;
}

SOCKET set_socket(){
	// create client socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("error : socket()\n");
		printf("Could not create socket : %d" , WSAGetLastError());
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
	if(send(sock,(const char*) id, sizeof(int), 0) == -1){
		//printf("[-] Failed to send the ID...\n");
		return;
	}
	//printf("[+] ID sent successfully !\n");
}

char* net_get(int* ID){
	// init WSA
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("error : SWAStartup()\n");
		return 0;

	}
	SOCKET sock = set_socket();
	SOCKADDR_IN sin = set_addr();
	// connect to the server
	if(connect(sock, (SOCKADDR *)&sin, sizeof(sin)) == -1){
		return 0;
	}
	// check ID/ create ID
	*ID == 0 ? *ID = time(NULL) : 0;
	// send ID
	send_ID(sock, ID);
	// get data
	char* key = get_data(sock); 
	// close the socket
	closesocket(sock);
	WSACleanup();
	return key;
}
