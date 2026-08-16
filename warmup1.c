#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

  pid_t pid = getpid();

  int z = fork();

  if (z==0){
    printf("I am child\n");
    pid = getpid();
    printf("%d\n", pid);
  }

  else{
    printf("I am parent\n");
    printf("%d\n", pid);
    wait(NULL);
  }

  return 0;
}
  
