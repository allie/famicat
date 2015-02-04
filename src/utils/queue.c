#include "queue.h"

Queue* Queue_New() {
	Queue* queue = (Queue*)malloc(sizeof(Queue));

	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;

	return queue;
}

void Queue_Destroy(Queue* queue) {
	if (queue == NULL)
		return;

	while (queue->head != NULL) {
		Item* tmp = queue->head;
		queue->head = queue->head->next;
		free(tmp);
	}

	free(queue);
}

void Queue_Enqueue(Queue* queue, void* value) {
	if (queue == NULL)
		return;

	Item* item = (Item*)malloc(sizeof(Item));

	item->value = value;
	item->next = NULL;

	if (queue->size == 0) {
		queue->head = item;
		queue->tail = item;
	} else {
		queue->tail->next = item;
		queue->tail = queue->tail->next;
	}

	queue->size++;
}

void* Queue_Dequeue(Queue* queue) {
	if (queue == NULL || queue->size == 0)
		return NULL;

	Item* tmp = queue->head;
	queue->head = queue->head->next;
	queue->size--;

	if (queue->size == 0)
		queue->tail = NULL;

	Item* item = tmp->value;
	free(tmp);

	return item;
}
