#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"
#include "reading.h"
#include "bfs.h"

int main() {
   
   // Wczytanie danych.
   Labyrinth *labyrinth = readInput();

   // Przejście labiryntu i wypisanie wyniku.
   bfs(labyrinth);
   
   // Zwolnienie pamięci.
   freeLabyrinth(labyrinth); 
   
   return 0;
}
