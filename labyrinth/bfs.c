#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
#include "queue.h"

// Funkcja zwraca zakodowaną pozycję w labiryncie.
// table - tablica opisująca kodowaną pozycję.
static size_t codePosition(size_t *table, Labyrinth *labyrinth) {
   size_t numberOfDimensions = getNumberOfDimensions(labyrinth);
   size_t result = 0;
   size_t product = 1;
   size_t *dimensions = getDimensions(labyrinth);
   for (size_t i = 0; i < numberOfDimensions ; i++) {
      result += (table[i] - 1) * product;
      product *= dimensions[i];
   }
   return result;
}

// Funkcja odkodowuje pozycję w labiryncie. 
// table - tablica, do której zostaje zapisana odkodowana pozycja.
// number - zakodowana pozycja.
static void decodePosition(size_t *table, Labyrinth *labyrinth, size_t number) {
   size_t numberOfDimensions = getNumberOfDimensions(labyrinth);
   size_t product = getLabyrinthSize(labyrinth);
   size_t *dimensions = getDimensions(labyrinth);

   // Rozpatruję indeks o 1 większy, gdyż wyrażenie i >= 0 byłoby zawsze prawdziwe dla i typu size_t.
   for (size_t i = numberOfDimensions; i > 0; i--) { 
      product /= dimensions[i - 1];
      table[i - 1] = number / product + 1;
      number -= (table[i - 1] - 1) * product;
   }
}

void bfs(Labyrinth *labyrinth) {
   Queue *q = createQueue();
   if (q == NULL)
      freeLabyrinthAndExitWithError(labyrinth, 0);

   size_t numberOfDimensions = getNumberOfDimensions(labyrinth);
   size_t currentPosition[numberOfDimensions];
   size_t *dimensions = getDimensions(labyrinth);
   size_t distance = 0;
   size_t position = getStartingPosition(labyrinth);
   size_t end = getEndingPosition(labyrinth);

   if (position == end) {
      printf("0\n");
      clearQueue(q);
      return;
   }
   
   if (!push(q, position, distance))
      freeLabyrinthAndExitWithError(labyrinth, 0);

   while (!isEmpty(q)) {
      position = getFirstPosition(q);
      distance = getFirstDistance(q);
      pop(q);
      decodePosition(currentPosition, labyrinth, position);
   
      for (size_t i = 0; i < numberOfDimensions; i++) {
         if (currentPosition[i] > 1) {
            currentPosition[i]--;
            position = codePosition(currentPosition, labyrinth);
            if (position == end) {
               printf("%zu\n", distance + 1);
               clearQueue(q);
               return;
            }
            if (!checkWall(labyrinth, position)) {
               if (!push(q, position, distance + 1))
                  freeLabyrinthAndExitWithError(labyrinth, 0);
               setWall(labyrinth, position);
            }
            currentPosition[i]++;
         }
         if (currentPosition[i] < dimensions[i]) {
            currentPosition[i]++;
            position = codePosition(currentPosition, labyrinth);
            if (position == end) {
               printf("%zu\n", distance + 1);
               clearQueue(q);
               return;
            }
            if (!checkWall(labyrinth, position)) {
               if (!push(q, position, distance + 1))
                  freeLabyrinthAndExitWithError(labyrinth, 0);
               setWall(labyrinth, position);
            }
            currentPosition[i]--;
         }
      }
   }

   printf("NO WAY\n");
   clearQueue(q);
}