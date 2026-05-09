#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
  int fd[2];
  char msg[] = "Hello from child!";
  char buffer[50];

  pipe(fd);
  int pid = fork();

  if(pid<0){
    printf("Fork failed");
    exit(1);

  }

  if(pid==0){
    close(fd[0]);
    write(fd[1], msg, sizeof(msg));
    close(fd[1]);
    exit(0);
  }else{
    close(fd[1]);
    read(fd[0], buffer, sizeof(buffer));
    printf("Parent received...\n");
    close(fd[0]);
    wait(0);
  }

  exit(0);

}
