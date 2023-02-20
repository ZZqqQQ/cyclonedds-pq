#include "dds/ddsc/dds_priorityqueue.h"

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t isEmptyCond = PTHREAD_COND_INITIALIZER;

bool isFull(priorityQueue pQ){
    return pQ->size >= pQ->capacity;
}

bool isEmpty(priorityQueue pQ){
    return pQ->size == 0;
}

priorityQueue initialize(int maxElements){
    priorityQueue pQ;

    if(maxElements < MinPQSize){
        printf("Priority queue size is too small!\n");
        exit(1);
    }

    pQ = malloc(sizeof(struct heapStruct));
    if(NULL == pQ){
        printf("Out of space!\n");
        exit(1);
    }
    
    pQ->ele = malloc((maxElements + 1) * sizeof(struct elementStruct*));
    if(NULL == pQ->ele){
        printf("Out of space!!\n");
        exit(1);
    }
    pQ->capacity = maxElements;
    pQ->size = 0;
    pQ->ele[0] = malloc(sizeof(struct elementStruct));
    pQ->ele[0]->priority = 0;
    pQ->ele[0]->msg = NULL;

    return pQ;
}


void destroy(priorityQueue pQ){
    if(NULL == pQ){
        printf("The pointer is nullptr!\n");
        return;
    }

    if(NULL == pQ->ele){
        printf("No space has allocated to this Queue!\n");
    }else{
        free(pQ->ele);
    }
    free(pQ);

    if(pthread_rwlock_destroy(&rwlock)){
        printf("Read/write lock destroy failed!");
        exit(1);
    }
}

int percolateUp(priorityQueue pQ, int priority, int loc){
    //pthread_rwlock_wrlock(&rwlock);
    int i;
    for(i = loc;pQ->ele[i / 2]->priority > priority;i/=2){
        pQ->ele[i] = pQ->ele[i / 2];
    }
    //pthread_rwlock_unlock(&rwlock);
    return i;
}

void insert(priorityQueue pQ, elementStruct *eS){
    if(isFull(pQ)){
        printf("Priority queue is full!\n");
        return;
    }
    pthread_mutex_lock(&mutex);
    pQ->size++;
    int i = percolateUp(pQ, eS->priority, pQ->size);
    pQ->ele[i] = eS;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&isEmptyCond);
}

void percolateDown(priorityQueue pQ){
    int i, child;

    // pthread_rwlock_wrlock(&rwlock);
    elementStruct *lastElement = pQ->ele[pQ->size];

    for(i = 1;i * 2 <= pQ->size;i = child){
        child = i * 2;
        if(child != pQ->size && pQ->ele[child + 1]->priority < pQ->ele[child]->priority){
            child++;
        }

        if(lastElement->priority > pQ->ele[child]->priority){
            pQ->ele[i] = pQ->ele[child];
        }
        else{
            break;
        }
    }
    pQ->ele[i] = lastElement;
    // pthread_rwlock_unlock(&rwlock);
}

void* deleteMin(priorityQueue pQ){
    elementStruct *minElement;
    if(isEmpty(pQ)){
        printf("Priority queue is empty\n");
        return pQ->ele[0]->msg;
    }
    minElement = pQ->ele[1];
    percolateDown(pQ);
    pQ->size--;
    return minElement;
}

// Scan the bottom of binary heap(floor(size / 2) + 1 ~ size)
void scanQueue(priorityQueue pQ){
    int index;
    while(true){
        // if(isEmpty(pQ)){
        //     printf("Waiting...\n");
        //     sleep(1);
        //     continue;
        // }
        index = pQ->size / 2 + 1;
        while(index <= pQ->size){
            if(time(NULL) - pQ->ele[index]->startTime >= thresholdTime){
                pQ->ele[index]->startTime = time(NULL);
                if(pQ->ele[index]->priority - 1 >= 0){
                    pthread_mutex_lock(&mutex);
                    pQ->ele[index]->priority -= 2;
                    printf("Priority has been raised to %d\n", pQ->ele[index]->priority);
                    elementStruct *tmp = pQ->ele[index];
                    int i = percolateUp(pQ, pQ->ele[index]->priority, index);
                    pQ->ele[i] = tmp;
                    pthread_mutex_unlock(&mutex);
                }
            }
            if((index + 1) > pQ->size){
                index = pQ->size / 2 + 1;
                usleep(500000);
            }else{
                index = index + 1;
            }
        }

    }
}

// void testCase1(priorityQueue pQ){
//     HelloWorldData_Msg msg2;
//     msg2.userID = 2;
//     msg2.message = "Gensin2";
//     elementStruct e1;
//     e1.priority = 2;
//     e1.msg = &msg2;
//     insert(pQ, &e1);
//     HelloWorldData_Msg msg3;
//     msg3.userID = 3;
//     msg3.message = "Gensin3";
//     elementStruct e2;
//     e2.priority = 3;
//     e2.msg = &msg3;
//     insert(pQ, &e2);
//     HelloWorldData_Msg msg1;
//     msg1.userID = 1;
//     msg1.message = "Gensin1";
//     elementStruct e;
//     e.priority = 1;
//     e.msg = &msg1;
//     insert(pQ, &e);
//     HelloWorldData_Msg msg4;
//     msg4.userID = 1;
//     msg4.message = "Gensin4";
//     elementStruct e3;
//     e3.priority = 4;
//     e3.msg = &msg4;
//     insert(pQ, &e3);
//     HelloWorldData_Msg* ret;
//     sleep(2);
//     ret = deleteMin(pQ);
//     printf("%s\n", ret->message);
//     ret  = deleteMin(pQ);
//     printf("%s\n", ret->message);
//     ret = deleteMin(pQ);
//     printf("%s\n", ret->message);
//     sleep(1);
//     ret = deleteMin(pQ);
//     printf("%s\n", ret->message);
// }