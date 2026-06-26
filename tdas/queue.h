#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

typedef List Queue;

Queue *queue_create(Queue *queue) { return list_create(); }

void queue_insert(Queue *queue, void *data) { list_pushBack(queue, data); }

void *queue_remove(Queue *queue) { return list_popFront(queue); }

void *queue_front(Queue *queue) { return list_first(queue); }

//Operacion especial para iterar sobre la cola
void *queue_next(Queue *queue) { return list_next(queue); }

//Operacion especial para borrar un elemento específico de la cola
void *queue_removeCurrent(Queue * queue) { return list_popCurrent(queue); }

void queue_clean(Queue *queue) { list_clean(queue); }

#endif /* QUEUE_H */