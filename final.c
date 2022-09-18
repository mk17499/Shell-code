#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_CLIENT_COUNT 1000

pid_t table[64];

int next;
pid_t to_kill;
pid_t to_protect;

void print_table()
{
	for(int i = 0;i<3;i++)
		printf("%u ",table[i]);
	printf("\n");
}


void my_sigint_handler_fg()
{
	//printf("process group to be killed = %d\n",to_kill);
	printf("\n");
	kill(to_kill,SIGKILL);
}

int numberoftokens=0;
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
	  numberoftokens++;
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

int main(int argc, char* argv[]) {

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              

    //signal(SIGINT,my_sigint_handler_bg);

	int i;
    int stat;
	
	signal(SIGINT,my_sigint_handler_fg);

	while(1) {			
		/* BEGIN: TAKING INPUT */

        int rv = waitpid(-1,&stat,WNOHANG);
        if(rv > 0)
        {
            printf("Shell : Background process reaped\n"); // check if the return value is greater thaan 0, to see if the process has returned the correct value
        }

		//print_table();

		numberoftokens = 0;
		int status;
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		printf("Command entered: %s\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		int fl=0;
       //do whatever you want with the commands, here we just print them
		
	   	if(tokens[0] == NULL)
		{
			continue;
		}
		if(strcmp(tokens[numberoftokens-1],"&")==0)
            fl = 1;

		if(strcmp(tokens[0], "cd" ) == 0)
		{
			if( tokens[1] == NULL) // to check if there is no token 1
				continue;
			else
			{
				int rv = chdir(tokens[1]);
				if(rv == -1)
					printf("Incorrect command\n");
				continue;
			}
		}
		if(strcmp(tokens[0],"exit") == 0)
		{
			for(int i = 0;i<64;i++) // Killing all the processes running in background using kill system call
			{
				kill(table[i],SIGKILL);
			}
			kill(getpid(),SIGKILL);
		}

		pid_t fc = fork();

		if(fc < 0)
		{
			fprintf(stderr,"%s\n","Unable to create a child process");
		}
		else if(fc == 0) // child's code
		{
            setpgid(0,0);
			// my child is not able to return back to home
			if(fl == 1 && strcmp(tokens[0],"&")!=0)
			{
				fl = 0;
				tokens[numberoftokens-1] = '\0';
			}
			execvp(tokens[0],tokens);
			printf("Command execution failed\n");
            continue;
			_exit(1);
		}
		else // parent's code
		{	
			table[next] = fc;
			next = next + 1;
			if(fl == 1)
			{
                waitpid(fc,&status,WNOHANG);
			}
            else
			{
				to_kill = fc;
                waitpid(fc,&status,0);
			}
		}
		// Freeing the allocated memory	

		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}