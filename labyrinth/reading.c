#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>
#include "structs.h"

#define STARTING_SIZE 4

// Pomocnicza struktura
typedef struct NumberStruct {
   bool endOfLine, error, readedTheNumber;
   size_t number;
} NumberStruct;

static bool safeRealloc(size_t **table, size_t size) {
   size_t *indicator;
	indicator = realloc(*table, size * sizeof(size_t));
	if (indicator != NULL) {
		*table = indicator;
		return true;
	}
   return false;
}

// Funkcja dodaje liczbę "number" do tablicy "table", aktualizuje liczbę elementów w tablicy.
// Zwraca true, jeżeli operacja się powiodła.
static bool addToTable(size_t number, size_t **table, size_t *numberOfDimensions, size_t *size) {
   if (*numberOfDimensions == *size) {
      (*size) *= 2;
      if (!safeRealloc(table, *size))
         return false;
   }

   (*table)[*numberOfDimensions] = number;
   (*numberOfDimensions)++;

   return true;
}

// Funkcja czyści tablicę "dimensions", zwraca odpowiedni błąd i kończy działanie programu.
static void exitWithError(int errorNumber, size_t *dimensions) {
   free(dimensions);
   fprintf(stderr, "ERROR %d\n", errorNumber);
   exit(1);
}

// Funkcja wczytuje nieujemną liczbę całkowitą i zwraca strukturę NumberStruct.
// - "endOfLine" przyjmuje wartość true, jeżeli napotkamy koniec linii.
// - "error" jest równy true, jeśli wystąpi błąd.
// - "readedTheNumber" jest równy true, jeśli udało się wczytać liczbę.
// - "number" jest równy wczytanej liczbie.
static NumberStruct readNonNegativeNumber() {
   NumberStruct result;
   result.endOfLine = false;
   result.error = false;
   result.readedTheNumber = false;
   result.number = 0;

   int cInt = 0;
   cInt = getchar();
   while (cInt < (int)'0' || cInt > (int)'9') { // pominięcie białych znaków.
      if (cInt < 0 || cInt == 10) {
         result.endOfLine = true;
         return result;
      }

      if (!isspace(cInt)) {
         result.error = true;
         return result;
      }

      cInt = getchar();
   }

   while (cInt >= (int)'0' && cInt <= (int)'9') {
      result.readedTheNumber = true;

		if ((SIZE_MAX - (cInt - (int)'0')) / 10 < result.number) {
         result.error = true;
         return result;
      }
		result.number = result.number * 10 + (cInt - (int)'0');

      cInt = getchar();
   }

   if (cInt < 0 || cInt == 10)
      result.endOfLine = true;
   else if (!isspace(cInt))
      result.error = true;
   
   return result;
}

// Funkcja wczytuje wymiary labiryntu, zapisuje je w tablicy "table",
// w zmiennej "numberOfDimensions" znajduje się ilość wczytanych liczb.
// Funkcja zwraca:
// 1, jeżeli wszystko się udało;
// 0, jeżeli wystąpił problem z pamięcią;
// -1, jeżeli wiersz nie spełniał wymagać.
static int readDimensions(size_t **table, size_t *numberOfDimensions, size_t *size) {
   bool endOfLine = false;
   while (!endOfLine) {
      NumberStruct result = readNonNegativeNumber();
      if (result.error || (result.readedTheNumber && result.number == 0))
         return -1;

      if (result.readedTheNumber) {
         if (!addToTable(result.number, table, numberOfDimensions, size))
            return 0;
      }

      endOfLine = result.endOfLine;
   }

   // Sprawdzenie, czy udało się wczytać chociaż jedną liczbę.
   return (*numberOfDimensions == 0 ? -1 : 1); 
}

// Funkcja wczytuje pozycję w labiryncie i koduje ją na liczbę typu size_t.
// Zwraca zakodowaną pozycję.
static size_t readPosition(size_t *dimensions, size_t numberOfElements, int lineNumber) {
   size_t position = 0;
   size_t product = 1;
   bool endOfLine = false;

   for (size_t i = 0; i < numberOfElements; i++) {
      if (endOfLine)
         exitWithError(lineNumber, dimensions);

      NumberStruct result = readNonNegativeNumber();
      if (result.error || !result.readedTheNumber 
            || result.number == 0 || result.number > dimensions[i])
         exitWithError(lineNumber, dimensions);

      position += (result.number - 1) * product;
      product *= dimensions[i];
      endOfLine = result.endOfLine;
   }

   // Sprawdzenie, czy na końcu linii pozostały tylko białe znaki.
   if (!endOfLine) {
      NumberStruct result = readNonNegativeNumber();
      if (result.error || result.readedTheNumber || !result.endOfLine)
         exitWithError(lineNumber, dimensions);
   }

   return position;
}

// Funkcja oblicza i zwraca rozmiar labiryntu.
static size_t calculateLabyrinthSize(size_t *dimensions, size_t numberOfDimensions) {
   size_t labyrinthSize = 1;

   for (size_t i = 0; i < numberOfDimensions; i++) {
      if (SIZE_MAX / dimensions[i] < labyrinthSize)
         exitWithError(0, dimensions);
		labyrinthSize *= dimensions[i];
   }

   return labyrinthSize;
}

// Funkcja wczytuje stringa w postaci szesnastkowej. Zwraca:
// 1, jeżeli wszystko się udało;
// 0, jeżeli wystąpił problem z pamięcią;
// -1, jeżeli wczytana liczba nie była liczbą szesnastkową.
static int readHexidecimalString(char **hexidecimalNumber, size_t *count) {
   size_t size = STARTING_SIZE;
   *count = 0;
   *hexidecimalNumber = malloc(size * sizeof(char));

   int cInt = getchar();
   while ((cInt >= (int)'0' && cInt <= (int)'9') || (cInt >= (int)'a' && cInt <= 'f') 
          || (cInt >= (int)'A' && cInt <= 'F')) {
      if (*count == size) {
         size *= 2;
         char *indicator;
         indicator = realloc(*hexidecimalNumber, size * sizeof(char));
         if (indicator != NULL)
            *hexidecimalNumber = indicator;
         else {
            free(*hexidecimalNumber);
            return 0;
         }
      }
      (*hexidecimalNumber)[*count] = (char)cInt;
      (*count)++;
      cInt = getchar();
   }
   while (cInt >= 0 && cInt != 10) {
      if (!isspace(cInt)) {
         free(*hexidecimalNumber);
         return -1;
      }
      cInt = getchar();
   }
   return 1;
}

// Funkcja wczytuje opis ścian w postaci z "R". Zwraca:
// 1, jeżeli wszystko się udało;
// 0, jeżeli wystąpił problem z pamięcią;
// -1, jeżeli wiersz nie spełniał wymagać.
static int readHexidecimalNumber(Labyrinth *labyrinth, size_t labyrinthSize) {
   char *hexidecimalNumber;
   size_t count = 0;

   int result = readHexidecimalString(&hexidecimalNumber, &count);
   if (result != 1)
      freeLabyrinthAndExitWithError(labyrinth, (result == 0 ? 0 : 4));

   size_t position = 0;

   // Rozpatruję indeks o 1 większy, gdyż wyrażenie i >= 0 byłoby zawsze prawdziwe dla i typu size_t.
   for (size_t i = count; i > 0; i--) {
      int number = hexidecimalNumber[i - 1] - '0';
      if (hexidecimalNumber[i - 1] >= 'a' && hexidecimalNumber[i - 1] <= 'f')
         number = 10 + hexidecimalNumber[i - 1] - 'a';
      if (hexidecimalNumber[i - 1] >= 'A' && hexidecimalNumber[i - 1] <= 'F')
         number = 10 + hexidecimalNumber[i - 1] - 'A';

      for (int j = 0; j < 4; j++) {
         if ((number & (1 << j)) != 0) {
            if (position + j >= labyrinthSize) {
               free(hexidecimalNumber);
               return -1;
            }
            setWall(labyrinth, position + j);
         }
      }

      position += 4;
   }

   free(hexidecimalNumber);
   return 1;
}

// Funkcja wczytuje opis ścian w postaci z "R". Zwraca:
// 1, jeżeli wszystko się udało;
// -1, jeżeli wiersz nie spełniał wymagać.
static int readWallsWithR(Labyrinth *labyrinth, size_t labyrinthSize) {
   size_t tab[5];
   bool endOfLine = false;

   for (int i = 0; i < 5; i++) {
      if (endOfLine)
         return -1;

      NumberStruct result = readNonNegativeNumber();
      if (result.error || !result.readedTheNumber || result.number > UINT32_MAX)
         return -1;

      tab[i] = result.number;
      endOfLine = result.endOfLine;
   }

   // Sprawdzenie, czy na końcu linii pozostały tylko białe znaki.
   if (!endOfLine) { 
      NumberStruct result = readNonNegativeNumber();
      if (result.error || result.readedTheNumber || !result.endOfLine)
         return -1;
   }

   size_t a = tab[0], b = tab[1], m = tab[2], r = tab[3], s = tab[4];

   if (m == 0)
      return -1;

   for (size_t i = 1; i <= r; i++) {
      s = (a * s + b) % m;
      size_t w = s % labyrinthSize;
      size_t x = ((size_t)1 << 32);
      while (w < labyrinthSize) {
         setWall(labyrinth, w);
         if (SIZE_MAX - x < w)
            break;
         w += x;
      }
   }

   return 1;
}

// Funkcja wczytuje czwarty wiersz i ustawia ściany. Zwraca:
// 1, jeżeli wszystko się udało;
// 0, jeżeli wystąpił problem z pamięcią;
// -1, jeżeli wiersz nie spełniał wymagać.
static int readWalls(Labyrinth *labyrinth, size_t labyrinthSize) {
   int cInt = 0;
   cInt = getchar();
   while (cInt >= 0 && cInt != 10) {
      if (cInt == (int)'R') {
         return readWallsWithR(labyrinth, labyrinthSize);
      }
      else if (cInt == (int)'0') {
         cInt = getchar();
         if (cInt != (int)'x')
            return -1;
         return readHexidecimalNumber(labyrinth, labyrinthSize);
      }
      else if (isspace(cInt)) {
         cInt = getchar();
      }
      else {
         return -1;
      }
   }
   return -1;
}

Labyrinth *readInput() {
   // Utworzenie tablicy.
   size_t numberOfDimensions = 0;
   size_t size = STARTING_SIZE;
   size_t *dimensions;
   dimensions = malloc(size * sizeof(size_t));
   if (dimensions == NULL)
      exitWithError(0, dimensions);

   // Wczytanie pierwszego wiersza.
   int result = readDimensions(&dimensions, &numberOfDimensions, &size);
   if (result != 1)
      exitWithError((result == 0 ? 0 : 1), dimensions);

   size_t labyrinthSize = calculateLabyrinthSize(dimensions, numberOfDimensions);

   // Wczytanie drugiego i trzeciego wiersza.
   size_t startingPosition = readPosition(dimensions, numberOfDimensions, 2);
   size_t endingPosition = readPosition(dimensions, numberOfDimensions, 3);

   // Utworzenie structa przechowującego opis labiryntu.
   Labyrinth *labyrinth;
   labyrinth = createLabyrinth(dimensions, startingPosition, endingPosition, 
                               numberOfDimensions, labyrinthSize);

   if (labyrinth == NULL)
      freeLabyrinthAndExitWithError(labyrinth, 0);

   // Wczytanie czwartego wiersza.
   int resultWall = readWalls(labyrinth, labyrinthSize);
   if (resultWall != 1)
      freeLabyrinthAndExitWithError(labyrinth, (resultWall == 0 ? 0 : 4));

   // Sprawdzenie, czy początkowa pozycja znajduje się w ścianie.
   if (checkWall(labyrinth, startingPosition))
      freeLabyrinthAndExitWithError(labyrinth, 2);

   // Sprawdzenie, czy końcowa pozycja znajduje się w ścianie.
   if (checkWall(labyrinth, endingPosition))
      freeLabyrinthAndExitWithError(labyrinth, 3);

   // Sprawdzenie, czy piąta linia jest pusta.
   int cInt = getchar(); 
   if (cInt >= 0)
      freeLabyrinthAndExitWithError(labyrinth, 5);

   return labyrinth;
}