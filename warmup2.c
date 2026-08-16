#include <stdio.h>
#include <unistd.h>

int main(){
  //printf("Before exec\n");

  //execl("/bin/ls", "ls", "-1", (char *)NULL);

  //execlp("ls", "ls", "-1", (char *)NULL);

  char *args[] = {"ls", "-1", NULL};
  execvp("ls", args);

  return 0;
}


