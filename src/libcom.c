#include "libcom.h"

void test(){
  printf("common lib is working !\n");
}

// int main(int argc, char** argv){
//   if (argc < 2){
//     printf(
//         "Usage : %s [d] <input file>\n"
//         "encrypt by default but decrypt if d is added as first argument\n",argv[0]);
//     return 0;
//   }
// 
//   int choice = strcmp(argv[1],"d")==0 ? 1:0; // 0->encrypt 1->decrypt
// 
//   FILE* input = openFile(argv[choice+1],"r"); 
//   char* outfilename = choice == 0 ? addext(argv[choice+1],".st") : remext(argv[choice+1]);
//   FILE* output = openFile(outfilename, "w");
//   free(outfilename);
// 
//   char key[] = "ThisIsMyKey"; 
//   char (*cyph[])(char,char*) = {enc, dec};
//   docrypt(input, output, key,cyph[choice]); 
// 
//   closeFile(input,output);
//   // delete the input file
//   remove(argv[choice+1]);
//   return 0;
// }

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
  char* output = malloc(strlen(input)+1);
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
  char buff = c^key[i%strlen(key)];
  i+=1;
  return buff;
}
char dec(char c, char* key){
  static int i = 0;
  char buff = c^key[i%strlen(key)];
  i+=1;
  return buff;
}
