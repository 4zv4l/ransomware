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
  strncpy(output, input, strlen(input));
  output[strlen(output)-3] = '\0';
  return output;
}

int docrypt(FILE* in, FILE* out, char* key, char (*cyph)(char,char*)){
  char c;
  while((c = fgetc(in))!= EOF){
    c = cyph(c,key);
    fputc(c,out);
  }
  return 0;
}

char enc(char c, char* key){
  static int i = 0;
  printf("-> %d\n",i);
  char buff = c^key[i%strlen(key)];
  buff = c+1;
  i+=1;
  return buff;
}
char dec(char c, char* key){
  static int i = 0;
  printf("-> %d\n",i);
  char buff = c^key[i%strlen(key)];
  buff = c-1;
  i+=1;
  return buff;
}

int processFile(char* path){
  // if extension .st then decrypt the file
  int todo = strstr(path, ".st") ? 1:0;
  char* outputPath = todo == 1 ? remext(path):addext(path, ".st");
  
  FILE* input = openFile(path, "r");
  FILE* output = openFile(outputPath, "w");
  free(outputPath);

  char (*cypher[])(char,char*) = {enc, dec};
  char* key = "ThisIsMyKey";
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

int encDir(char* path){
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
        encDir(fullpath);
      }else{
        processFile(fullpath) == 0 ? 0:printf("Error : %s\n",fullpath);
      }
      free(fullpath);
    }
  }
  closedir(dir);
  return 0;
}
