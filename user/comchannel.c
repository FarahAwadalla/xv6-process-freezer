#include "kernel/types.h"
#include "user/user.h"

int main(){
  int fds[2];
  char buf[100];
  int n;

  pipe(fds);

  int pid;
  pid = fork();
  printf("fork() returned %d\n", pid);
  if(pid == 0){
    printf("child\n");
    n = read(fds[0], buf, sizeof(buf));
    write(1, buf, n);

  } else {
    printf("parent\n");
    write(fds[1],"write to parent\n", 16 );
  
  }

  close(pid);

  exit(0);
}
