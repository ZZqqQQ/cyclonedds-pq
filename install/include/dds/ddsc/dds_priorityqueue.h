#ifndef DDS_PRIORITY_QUEUE_H
#define DDS_PRIORITY_QUEUE_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "dds/export.h"
static const int MinPQSize = 2;
static const int thresholdTime = 1;

struct elementStruct;
struct heapStruct;
typedef struct elementStruct elementStruct;
typedef struct heapStruct *priorityQueue;

struct elementStruct{
    int priority;
    time_t startTime;
    void *msg;
};

struct heapStruct{
    int capacity;
    int size;
    elementStruct** ele;
};

bool isFull(priorityQueue pQ);
bool isEmpty(priorityQueue pQ);
DDS_EXPORT priorityQueue initialize(int maxElements);
void destroy(priorityQueue pQ);
int percolateUp(priorityQueue pQ, int priority, int loc);
DDS_EXPORT 
void insert(priorityQueue pQ, elementStruct *eS);
void percolateDown(priorityQueue pQ);
void* deleteMin(priorityQueue pQ);
DDS_EXPORT
void scanQueue(priorityQueue pQ);

#endif