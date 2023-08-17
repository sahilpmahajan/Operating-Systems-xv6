#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int account_balance = 0;
pthread_mutex_t m;

typedef struct {
    int amt;
} myarg_t;

void* increment(void *arg) {
    myarg_t *args = (myarg_t *) arg;
    for(int j=0; j< args->amt;j++){
        pthread_mutex_lock(&m);
        for (int i = 0; i < 1000000; i++) {
            account_balance++;
        }
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char* argv[]) {
    clock_t t = clock();
    if(argc < 2) exit(EXIT_FAILURE);
    int threadNum;
    threadNum = atoi(argv[1]);
    pthread_t th[threadNum];
    int rc =  pthread_mutex_init(&m, NULL);
    myarg_t args = {2048/threadNum};
    for (int i = 0; i < threadNum; i++) {
        if (pthread_create(th + i, NULL, &increment, &args) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        printf("Transaction %d has started\n", i);
    }
    for (int i = 0; i < threadNum; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Transaction %d has finished\n", i);
    }
    printf("Account Balance is : %d\n", account_balance);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    time_taken *= 1000;  //convert to millisec
    printf("Time spent: %f ms \n", time_taken);
    return 0;
}