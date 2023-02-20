#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Labyrinth Labyrinth;

// Funkcja tworzy structa "Labyrinth" z danymi i zwraca wskaźnik na niego.
Labyrinth *createLabyrinth(size_t *dimensions, size_t startingPosition, 
                           size_t endingPosition, size_t numberOfDimensions, 
                           size_t labyrinthSize);

// Funkcja zwraca tablicę z wymiarami labiryntu.
size_t *getDimensions(Labyrinth *labyrinth);

// Funkcja zwraca zakodowaną pozycję początkową.
size_t getStartingPosition(Labyrinth *labyrinth);

// Funkcja zwraca zakodowaną pozycję końcową.
size_t getEndingPosition(Labyrinth *labyrinth);

// Funkcja zwraca liczbę wymiarów w labiryncie.
size_t getNumberOfDimensions(Labyrinth *labyrinth);

// Funkcja zwraca rozmiar labiryntu.
size_t getLabyrinthSize(Labyrinth *labyrinth);

// Funkcja sprawdza, czy w danej pozycji jest ściana.
bool checkWall(Labyrinth *labyrinth, size_t position);

// Funkcja ustawia ścianę w danej pozycji.
void setWall(Labyrinth *labyrinth, size_t position);

// Funkcja zwalnia pamięć.
void freeLabyrinth(Labyrinth *labyrinth);

// Funkcja zwalnia pamięć i wypisuje komunikat o błędzie.
void freeLabyrinthAndExitWithError(Labyrinth *labyrinth, int errorNumber);

#endif /* STRUCTS_H */