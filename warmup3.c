#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT 100

int main(int argc, char *argv[]){

  /* me stupid
  char input[MAX_INPUT];
  fgets(input, MAX_INPUT, stdin);
  strtok(input, "\n");      //removing \n from the end of string
  
  char *command, *argument, *extra;
  command = strtok(input, " ");
  argument = strtok(NULL, " ");
  extra = strtok(NULL, " ");
  */

  if (argc!=3){
    printf("Incorrect amount of arguments!\n");
    return -1;
  }
  
  int pid = fork();
  if (pid==0){
    // Child will run this code
    char *args[] = {argv[1], argv[2], NULL};
    execvp(argv[1], args);
  }

  else{
    wait(NULL);
    printf("Successfully executed the '%s' command with pid %d\n", argv[1], pid);
  }
  

  return 0;
}
