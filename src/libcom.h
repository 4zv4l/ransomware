#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test();

// open the file in the right mode and check for errors
FILE* openFile(const char* path, const char* mode);
// close the input and output files
int closeFile(FILE*, FILE*);
// docrypt will read each char from input
// and use the function given in argument to encrypt each char
// will write de encrypted char in the output file
int docrypt(FILE* input, FILE* output, char* key,char (*)(char,char*));
// encrypt the char with the given key
char enc(char,char*);
// decrypt the char with the given key
char dec(char,char*);
// add the extension to the encrypted file
char* addext(const char* s1, const char* s2);
// remove the extension from the file
char* remext(const char* input);


