#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
  int pid = fork();
 
  if(pid<0){
    printf("Fork failed\n");
    exit(1);

  }

  if(pid==0){
    printf("Child processing...");
    char *args[] = {"ls", 0};
    exec("ls", args);

   printf("Execution failed!\n");
   exit(1);

  }

  else{
    wait(0);
    printf("Child finished.\n");

  }

  exit(0);

}
