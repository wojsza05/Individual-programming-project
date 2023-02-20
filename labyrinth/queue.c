#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node;

struct Node {
   size_t position;
   size_t distance;
   struct Node *prev;
   struct Node *next;
};

typedef struct Queue {
   struct Node *front;
   struct Node *back;
} Queue;

Queue *createQueue() {
   Queue *q;
   q = malloc(sizeof(Queue));
   if (q != NULL) {
      q->front = NULL;
      q->back = NULL;
   }
   return q;
}

bool isEmpty(Queue *q) {
   return (q->front == NULL);
}

size_t getFirstPosition(Queue *q) {
   return (q->front)->position;
}

size_t getFirstDistance(Queue *q) {
   return (q->front)->distance;
}

bool push(Queue *q, size_t position, size_t distance) {
   struct Node *node = NULL;
   node = malloc(sizeof(struct Node));
   if (node == NULL)
      return false;
      
   node->position = position;
   node->distance = distance;
   node->prev = NULL;
   node->next = NULL;
   if (q->back == NULL) {
      q->front = node;
      q->back = node;
   }
   else {
      (q->back)->next = node;
      node->prev = q->back;
      q->back = node;
   }
   return true;
}

void pop(Queue *q) {
   struct Node *node = q->front;
   if (node->next == NULL) {
      q->front = NULL;
      q->back = NULL;
   }
   else {
      q->front = node->next;
      (node->next)->prev = NULL;
   }
   free(node);
}

void clearQueue(Queue *q) {
   while (!isEmpty(q))
      pop(q);
   free(q);
}