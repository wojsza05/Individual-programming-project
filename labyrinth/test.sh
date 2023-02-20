#!/bin/bash

if (($# != 2))
then
  echo "Niewłaściwa ilość parametrów!"
  exit 1
fi

PROG=$1
DIR=$2

if [ ! -e $PROG ]
then
   echo "Podany program nie istnieje!"
   exit 1
fi

if [ ! -d $DIR ]
then
   echo "Podana ścieżka nie istnieje!"
   exit 1
fi

ERRORS=0
VALGRIND_ERRORS=0
TESTS=0

for f in "$DIR/"*.in 
do 
   ((TESTS++))
   echo "Test $f: "

   valgrind \
      --error-exitcode=123 \
      --leak-check=full \
      --show-leak-kinds=all \
      --errors-for-leak-kinds=all \
      --log-fd=9 9>temp.out \
      ./$PROG <$f 1>test.out 2>test.err

   # Sprawdzenie valgrinda.
   if [ $? -eq 123 ]
   then
      cat temp.out
      ((VALGRIND_ERRORS++))
   else
      echo "Valgrind nie znalazł problemów."
   fi

   # Sprawdzenie poprawności odpowiedzi.
   if diff "${f%in}out" "test.out" >/dev/null 2>&1 \
      && diff "${f%in}err" "test.err" >/dev/null 2>&1
   then 
      echo $'OK.\n'
   else
      echo $'Błędna odpowiedź.\n'
      ((ERRORS++))
   fi

   # Usunięcie powstałych plików.
   rm test.out test.err temp.out
done

echo "Liczba testów: $TESTS."
echo "Liczba błędnych odpowiedzi: $ERRORS."
echo "Liczba błędów valgrinda: $VALGRIND_ERRORS."