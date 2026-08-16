#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_FILE_PATH 256

struct Node{
  int pid;
  struct Node *next;
};

int peek(struct Node *head){
  return head->pid;
}

struct Node* add_child(struct Node *head, int pid){
  if (head==NULL){
    head = (struct Node*)malloc(sizeof(struct Node));
    head->pid = pid;
    head->next = NULL;
    return head;
  }
  struct Node *lastnode = head;
  while (lastnode->next!=NULL){
    lastnode = lastnode->next;
  }
  struct Node *new = (struct Node*)malloc(sizeof(struct Node));
  new->pid = pid;
  new->next = NULL;
  lastnode->next = new;
  return head;
}

struct Node* remove_child(struct Node *head, int pid){
  struct Node *currentnode = head;
  struct Node *lastnode;

  if (head==NULL){
    return head;
  }

  if (head->pid==pid){
    head = currentnode->next;
    free(currentnode);
    return head;
  }

  while (currentnode!=NULL){
    if (currentnode->pid==pid){
      lastnode->next = currentnode->next;
      free(currentnode);
      break;
    }
    lastnode = currentnode;
    currentnode = currentnode->next;
  }
  return head;
}

int printlist(struct Node *head){
  struct Node *currentnode = head;

  while (currentnode!=NULL){
    printf("%d ", currentnode->pid);
    currentnode = currentnode->next;
  }
  printf("NULL\n");
  return 0;
}

char **tokenize(char *line, int* tokens_size)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	    tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	    strcpy(tokens[tokenNo++], token);
	    tokenIndex = 0; 
      }
    }
    else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL;
  *tokens_size = tokenNo;
  return tokens;
}


void signal_handler(int sig){
  printf("\n");
}

int main(int argc, char* argv[]) {
    struct Node *child_list = NULL;
    char pwd[MAX_FILE_PATH];
	char line[MAX_INPUT_SIZE];
	char **tokens;
    int tokens_size, pid, ws;
    int is_running = 1;
	int i;

    signal(SIGINT, signal_handler);

	while(is_running) {
        getcwd(pwd, sizeof(pwd));

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("%s $ ", pwd);
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line, &tokens_size);
   
        //do whatever you want with the commands, here we just print them

        if ((tokens_size==2) && (strcmp(tokens[0],"cd")==0)){
          chdir(tokens[1]);
        }
        else if ((tokens_size==1) && (strcmp(tokens[0],"exit")==0)){
          while (child_list!=NULL){
            pid = peek(child_list);
            kill(pid, 9);
            child_list = remove_child(child_list, pid);
          }
          is_running = 0;
        }
        else if (tokens_size>0){
          int is_background = 0;
          if ((tokens_size>1) && (strcmp(tokens[tokens_size-1],"&")==0)){
            is_background = 1;
            tokens[tokens_size-1] = NULL;
          }
          pid = fork();
          // Child code
          if (pid==0){
            if (is_background){
              setpgid(0,0);
            }
            int err = execvp(tokens[0], tokens);
            if (err==-1){
              exit(1);
            }
          }
          // Parent code
          if (is_background){
            child_list = add_child(child_list, pid);
          }
          else{
            waitpid(pid, &ws, 0);
            if (WEXITSTATUS(ws)==1){
              printf("EXITSTATUS: 1\n");
            }
          }
        }

        // Periodic check to reap background child processes
        if (child_list!=NULL){
          pid = waitpid(-1, &ws, WNOHANG);
          if (pid>0){
            child_list = remove_child(child_list, pid);
            printf("Background process finished: %d\n", pid);
          }
        }

		//for(i=0;tokens[i]!=NULL;i++){
		//	printf("found token %s (remove this debug output later)\n", tokens[i]);
		//}
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
