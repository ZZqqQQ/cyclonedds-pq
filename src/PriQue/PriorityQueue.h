#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#define MinPQSize 2

struct HelloWorldData_Msg;
struct elementStruct;
struct heapStruct;
typedef struct HelloWorldData_Msg HelloWorldData_Msg;
typedef struct elementStruct elementStruct;
typedef struct heapStruct *priorityQueue;

struct HelloWorldData_Msg
{
  int32_t userID;
  char * message;
};

struct elementStruct{
    int priority;
    time_t startTime;
    HelloWorldData_Msg* msg;
};

struct heapStruct{
    int capacity;
    int size;
    elementStruct* ele;
};

bool isFull(priorityQueue pQ);
bool isEmpty(priorityQueue pQ);
priorityQueue initialize(int maxElements);
void destroy(priorityQueue pQ);
int percolateUp(priorityQueue pQ, int priority, int loc);
void insert(priorityQueue pQ, elementStruct *eS);
void percolateDown(priorityQueue pQ);
HelloWorldData_Msg* deleteMin(priorityQueue pQ);
void scanQueue(priorityQueue pQ);

void testCase1(priorityQueue pQ);

#endif