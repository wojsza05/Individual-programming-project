#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

// Funkcja tworzy nową kolejkę i zwraca wskaźnik na nią.
Queue *createQueue();

// Funkcja sprawdza, czy kolejka jest pusta.
bool isEmpty(Queue *q);

// Funkcja zwraca pozycję w labiryncie wierzchołka z początku kolejki.
size_t getFirstPosition(Queue *q);

// Funkcja zwraca dystans wierzchołka z początku kolejki.
size_t getFirstDistance(Queue *q);

// Funkcja dodaje wierzchołek na koniec kolejki.
// Zwraca "true", jeżeli się to udało i "false" w przeciwnym razie.
bool push(Queue *q, size_t position, size_t distance);

// Funkcja usuwa wierzchołek z początku kolejki.
// Może zostać wykonana tylko na niepustej kolejce.
void pop(Queue *q);

// Funkcja czyszcząca kolejkę.
void clearQueue(Queue *q);

#endif /* QUEUE_H */