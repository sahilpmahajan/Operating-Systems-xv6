#include "types.h"
#include "stat.h"
#include "user.h"
// #include <stdlib.h>
// #include<sys/wait.h>

int main(int argc, char *argv[]){
    if(argc <= 1){
        exit();
    }
    int pid = fork();
    if(pid==0){
        char **args = (char **)malloc(10 * sizeof(char *));
        for(int i=0;i<argc;i++){
            args[i] = argv[i+1];
        }
        exec(args[0],args);
        // OR do following
        // exec(argv[1],argv+1);
		exit();
    }
    else{
        wait();
    }    
    exit();
}