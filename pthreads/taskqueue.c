#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Global variables
long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;
pthread_mutex_t m;

struct queue{
    struct queue* next;
    int num;
};
struct queue* q;
struct queue* end;


void processtask(long number);

void processtask(long number)
{
    // simulate burst time
    sleep(number);

    // update global variables
    sum += number;
    if (number % 2 == 1)
    {
        odd++;
    }
    else
    {
        even++;
    }
    if (number < min)
    {
        min = number;
    }
    if (number > max)
    {
        max = number;
    }
}

void* multithread() {
    while(1){
        pthread_mutex_lock(&m);
        if(q==NULL){
            pthread_mutex_unlock(&m);
            if(done) break;
            else continue;
        }
        processtask(q->num);
        q = q->next;
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: sum <infile>\n");
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    int threadNum = atoi(argv[2]);
    int rc =  pthread_mutex_init(&m, NULL);
    pthread_t th[threadNum];
    // q = malloc(sizeof(struct queue));
    // q->next = NULL;
    end = q;
    // Read from file
    FILE *fin = fopen(fn, "r");
    long t;
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    char type;
    long num;

    for (int i = 0; i < threadNum; i++) {
        if (pthread_create(th + i, NULL, &multithread, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        printf("Thread %d has started\n", i);
    }

    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {
        if (type == 'p')
        { // processing task
            // processtask(num);
            // pthread_mutex_lock(&m);
            end = malloc(sizeof(struct queue));
            end->num = num;
            end->next = NULL;
            end = end->next;
            // pthread_mutex_unlock(&m);
            printf("Task completed\n");
        }
        else if (type == 'w')
        { // waiting period
            sleep(num);
            printf("Wait Over\n");
        }
        else
        {
            printf("ERROR: Type Unrecognizable: '%c'\n", type);
            exit(EXIT_FAILURE);
        }
    }
    done = true;
    fclose(fin);

    ////////////////////////////
    for (int i = 0; i < threadNum; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Thread %d has finished\n", i);
    }
    // Print global variables
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    return (EXIT_SUCCESS);
}
