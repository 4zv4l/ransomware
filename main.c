#ifdef __unix__                         // if linux or others Unix like OS
    #define OS_Windows 0
    #include "src/liblin.h"
    #include <unistd.h>                 // linux kernel syscall
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
#elif defined(_WIN32) || defined(WIN32) // if Windows
    #define OS_Windows 1
    #include "src/libwin.h"
    #include <windows.h>                // Windows API
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
#endif

int main (int argc, char *argv[]){
  printf("Here we go again...\n");
  welcome();
  return 0;
}
