#include "PriorityQueue.h"

int main(int argc, char** argv){
    priorityQueue q = initialize(5);
    pthread_t th1, th2;
    pthread_create(&th1, NULL, (void *)testCase1, (void *)q);
    pthread_create(&th2, NULL, (void *)scanQueue, (void *)q);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    destroy(q);

    return 0;
}