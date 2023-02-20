#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Pomocnicza struktura do przechowywania informacji o ścianach.
typedef struct Bitset {
   int32_t *table;
} Bitset;

typedef struct Labyrinth {
   size_t *dimensions;
   size_t startingPosition;
   size_t endingPosition;
   size_t numberOfDimensions;
   size_t labyrinthSize;
   Bitset *bitset;
} Labyrinth;

static Bitset *createBitset(size_t numberOfElements) {
   Bitset *bitset = NULL;
   bitset = malloc(sizeof(Bitset));
   if (bitset == NULL)
      return NULL;

   bitset->table = calloc((numberOfElements - 1) / 32 + 1, sizeof(int32_t));
   if (bitset->table == NULL)
      return NULL;

   return bitset;
}

static bool checkPosition(Bitset *bitset, size_t position) {
   size_t positionInTable = position / 32;
   int32_t bitInNumber = (int32_t)(position & 31);
   return ((bitset->table)[positionInTable] & (int32_t)(1 << bitInNumber));
}

static void setPosition(Bitset *bitset, size_t position) {
   size_t positionInTable = position / 32;
   int32_t bitInNumber = (int32_t)(position & 31);
   (bitset->table)[positionInTable] |= (int32_t)(1 << bitInNumber);
}

static void freeBitset(Bitset *bitset) {
   if (bitset != NULL)
      free(bitset->table);
   free(bitset);
}

Labyrinth *createLabyrinth(size_t *dimensions, size_t startingPosition, 
                           size_t endingPosition, size_t numberOfDimensions,
                           size_t labyrinthSize) {
   Labyrinth *labyrinth;
   labyrinth = malloc(sizeof(Labyrinth));
   if (labyrinth == NULL)
      return NULL;
      
   labyrinth->dimensions = dimensions;
   labyrinth->startingPosition = startingPosition;
   labyrinth->endingPosition = endingPosition;
   labyrinth->numberOfDimensions = numberOfDimensions;
   labyrinth->labyrinthSize = labyrinthSize;
   Bitset *bitset = createBitset(labyrinthSize);
   if (bitset == NULL) {
      free(dimensions);
      return NULL;
   }
   labyrinth->bitset = bitset;
   return labyrinth;
}

size_t *getDimensions(Labyrinth *labyrinth) {
   return labyrinth->dimensions;
}

size_t getStartingPosition(Labyrinth *labyrinth) {
   return labyrinth->startingPosition;
}

size_t getEndingPosition(Labyrinth *labyrinth) {
   return labyrinth->endingPosition;
}

size_t getNumberOfDimensions(Labyrinth *labyrinth) {
   return labyrinth->numberOfDimensions;
}

size_t getLabyrinthSize(Labyrinth *labyrinth) {
   return labyrinth->labyrinthSize;
}

bool checkWall(Labyrinth *labyrinth, size_t position) {
   return checkPosition(labyrinth->bitset, position);
}

void setWall(Labyrinth *labyrinth, size_t position) {
   setPosition(labyrinth->bitset, position);
}

void freeLabyrinth(Labyrinth *labyrinth) {
   if (labyrinth != NULL) {
      free(labyrinth->dimensions);
      freeBitset(labyrinth->bitset);
   }
   free(labyrinth);
}

void freeLabyrinthAndExitWithError(Labyrinth *labyrinth, int errorNumber) {
   fprintf(stderr, "ERROR %d\n", errorNumber);
   freeLabyrinth(labyrinth);
	exit(1);
}