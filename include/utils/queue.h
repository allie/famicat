#ifndef QUEUE_H
#define QUEUE_H

#include "../common.h"

typedef struct Item {
	void* value;
	struct Item* next;
} Item;

typedef struct {
	int size;
	Item* head;
	Item* tail;
} Queue;

Queue* Queue_New();
void Queue_Destroy(Queue* queue);
void Queue_Enqueue(Queue* queue, void* value);
void* Queue_Dequeue(Queue* queue);

#endif
