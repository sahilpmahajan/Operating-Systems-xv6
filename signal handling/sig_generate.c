#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("Give valid number \n");
        return 0;
    }
    int pid;
    pid = atoi(argv[1]);
    kill(pid,SIGINT);
	printf("SIGINT signal sent to PID: %d \n",getpid());
    kill(pid,SIGTERM);
	printf("SIGTERM signal sent to PID: %d \n",getpid());
    kill(pid,SIGKILL);
	printf("SIGKILL signal sent to PID: %d \n",getpid());
	return 0;
}


