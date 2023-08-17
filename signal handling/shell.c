#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_CHILD 64

// TODO: YOUR CODE HERE
// INITIALIZE DATA STRUCTURE TO STORE PIDS OF PROCESSES TO KILL LATER
int pid[MAX_CHILD];
/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line) {

    // tokenizer variables
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

    // loop on length of line
	for(i=0; i < strlen(line); i++){

		char readChar = line[i];

        // tokenize on any kind of space
		if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0) {
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
	tokens[tokenNo] = NULL ;
	return tokens;
}

// TODO
// MAKE FUNCTIONS AS REQUIRED TO AVOID REPETITIVE WORK
void execute(char **tokens , int count){
	if(strcmp(tokens[0],"exit")==0){
			for(int i=0;tokens[i]!=NULL;i++) free(tokens[i]);
			free(tokens);
			exit(0);
		}
	else if(strcmp(tokens[0],"cd")==0){
			if(count!=2){
				printf("Invalid number of arguments\n");
				return;
			}
			if(chdir(tokens[1])!=0)
				printf("No such file or directory\n");
		}
	// create child process to execute the command 
	else{
		if (fork() == 0){
			execvp(tokens[0],tokens);			
			exit(0);
		}
		else{
			wait(NULL);
		}
	}
}


int main(int argc, char* argv[]) {

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;           

	// TODO: YOUR CODE HERE
	// INITIALIZE GLOBAL DATA STRUCTURE OF PIDS TO SOME DEFAULT VALUE  
	for(int i=0;i<MAX_CHILD;i++) pid[i]=0;
	// INITIALIZE OTHER VARIABLES AS NEEDED

	while(1) {	

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		// You can maintain boolean variables to check which type of command is given by user and then
        // conditionally execute depending on them

        // TODO: YOUR CODE HERE
		int count=0;
		int cserial=0;
		int cparallel=0;
		while(tokens[count]!=NULL){
			if(strcmp(tokens[count],"&&")==0) cserial++;
			if(strcmp(tokens[count],"&&&")==0) cparallel++;
			count++;
		}
		if(count==0) continue;
		//code if not in parallel
		if(cparallel==0){
			cserial++;
			int c=0 , cnt=0; //c is local counter, cnt is total counter
			while(cserial!=0){ // for every serial find its tokens and count and execute it
				char **tokens_1 = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
				// generate new tokens_1 array which contains args of only 1 instruction
				while(cnt<=count && tokens[cnt]!=NULL && strcmp(tokens[cnt],"&&")!=0){
					tokens_1[c] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
					strcpy(tokens_1[c], tokens[cnt]);
					c++;
					cnt++;
				}
				execute(tokens_1,c);
				for(int i=0;tokens_1[i]!=NULL;i++) free(tokens_1[i]);
				free(tokens_1);
				cserial--;
				c=0;
				cnt++;
			}
		}
		// code if parallel present
		else{}
		

        // TODO: YOUR CODE HERE
    
        // freeing the memory
		for(int i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}

		free(tokens);

	}
	return 0;
}


