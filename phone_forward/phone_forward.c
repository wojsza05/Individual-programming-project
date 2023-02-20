/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych.
 *
 * @author Wojciech Weremczuk <ww438808@students.mimuw.edu.pl>
 * @copyright Wojciech Weremczuk
 * @date 2022
 */
 
#include <stdlib.h>
#include <string.h>
#include "phone_forward.h"

/** Początkowa wielkość tablicy.
 * Wynikiem funkcji @ref phfwdGet jest struktura @p PhoneNumbers zawierająca co
 * najwyżej jeden element, dlatego ustawiamy początkową wielkość tablicy na 1.
 */
#define INITIAL_SIZE_OF_THE_ARRAY 1

/** Liczba cyfr.
 * Numer składa się z cyfr 0..9, *, #.
 */
#define NUMBER_OF_DIGITS 12

/** @brief Sprawdza, czy podany znak jest cyfrą numeru.
 * @param[in] digit - znak, dla którego sprawdzamy poprawność.
 * @return Wartość @p true, jeśli podany znak jest cyfrą numeru.
 *         Wartość @p false, w przeciwnym przypadku.
 */
static bool isDigit(char const digit) {
   return (digit >= '0' && digit <= '9') || digit == '*' || digit == '#';
}

/** @brief Zwraca identyfikator znaku.
 * Zwraca identyfikator znaku zgodnie z treścią zadania - cyfry 0..9 
 * reprezentują same siebie, * reprezentuje cyfrę 10, # reprezentuje cyfrę 11.
 * @param[in] digit - znak, dla którego wyznaczamy identyfikator.
 * @return Identyfikator znaku.
 */
static int digitID(char const digit) {
   if (digit == '*')
      return 10;
   if (digit == '#')
      return 11;
   return digit - '0';
}

/** @brief Wyznacza długość numeru.
 * @param[in] number - wskaźnik na napis reprezentujący numer.
 * @return Długość napisu reprezentującego numer telefonu
 *         (jeżeli wskaźnik na napis ma wartość NULL lub podany 
 *         napis nie jest poprawnym numerem, to zwraca 0).
 */
static size_t numberLength(char const *number) {
   if (number == NULL)
      return 0;
   
   size_t length = 0;
   while (isDigit(number[length]))
      length++;

   if (number[length] != '\0')
      return 0;
   return length;
}

/** @brief Zmienia długość numeru.
 * Alokuje odpowiednią ilość pamięci, tak żeby @p number przechowywał 
 * wskaźnik na numer długości @p length. 
 * @param[in,out] number - wskaźnik na numer.
 * @param[in] length     - docelowa długość numeru.
 * @return Wartość @p true, jeśli alokacja pamięci się powiodła.
 *         Wartość @p false, w przeciwnym przypadku.
 */
static bool reallocNumber(char **number, size_t const length) {
   if (*number == NULL) {
      *number = malloc((length + 1) * sizeof(char));
      if (*number == NULL)
         return false;
   }
   else {
      char *indicator;
      indicator = realloc(*number, (length + 1) * sizeof(char));
      if (indicator == NULL) 
         return false;
      *number = indicator;
   }
   return true;
}

/** @brief Porównuje dwa numery.
 * @param[in] number1 - wskaźnik na pierwszy numer.
 * @param[in] number2 - wskaźnik na drugi numer.
 * @return Wartość @p -1, jeśli pierwszy numer jest mniejszy leksykograficznie.
 *         Wartość @p 0, jeśli podane numery są równe.
 *         Wartość @p 1, jeśli drugi numer jest mniejszy leksykograficznie.
 */
static int numberComparator(char const **number1, char const **number2) {
   size_t pos = 0;
   while ((*number1)[pos] != '\0' && (*number2)[pos] != '\0') {
      if (digitID((*number1)[pos]) < digitID((*number2)[pos]))
         return -1;
      if (digitID((*number1)[pos]) > digitID((*number2)[pos]))
         return 1;
      pos++;
   }

   if ((*number1)[pos] == '\0' && (*number2)[pos] == '\0')
      return 0;
   return ((*number1)[pos] == '\0' ? -1 : 1);
}

struct PhoneNumbers {
   size_t size;      ///< Rozmiar tablicy.
   size_t count;     ///< Liczba elementów w tablicy.
   char **numbers;   ///< Tablica numerów.
};

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych numerów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
static PhoneNumbers * phnumCreate(void) {
   PhoneNumbers *pnum;
   pnum = malloc(sizeof(PhoneNumbers));
   if (pnum == NULL)
      return NULL;
   
   pnum->size = INITIAL_SIZE_OF_THE_ARRAY;
   pnum->count = 0;
   pnum->numbers = malloc((pnum->size) * sizeof(char*));
   if (pnum->numbers == NULL) {
      free(pnum);
      pnum = NULL;
      return NULL;
   }
   
   return pnum;
}

/** @brief Dodaje numer.
 * Dodaje numer do struktury.
 * @param[in,out] pnum - wskaźnik na strukturę przechowującą numery.
 * @param[in] number   - wskaźnik na napis reprezentujący numer.
 * @param[in] length   - długość numeru.
 * @return Wartość @p true, jeśli numer został dodany.
 *         Wartość @p false, jeśli wystąpił błąd, np. wskaźnik @p pnum
 *         ma wartość NULL lub nie udało się alokować pamięci.
 */
static bool phnumAdd(PhoneNumbers *pnum, 
                     char const *number, size_t const length) {
   if (pnum == NULL || length == 0)
      return false;
   
   if (pnum->size == pnum->count) {
      char **indicator;
      indicator = realloc(pnum->numbers, 2 * (pnum->size) * sizeof(char*));
      if (indicator == NULL)
         return false;
      pnum->size *= 2;
      pnum->numbers = indicator;
   }

   (pnum->numbers)[pnum->count] = NULL;
   if (!reallocNumber(&((pnum->numbers)[pnum->count]), length))
      return false;

   for (size_t i = 0; i <= length; i++)
      (pnum->numbers)[pnum->count][i] = number[i];

   (pnum->count)++;
   return true;
}

/** @brief Sortuje numery i usuwa doplikaty.
 * Sortuje numery znajdujące się strukturze @p PhoneNumbers oraz 
 * usuwa te, które się powtarzają.
 * @param[in,out] pnum - wskaźnik na strukturę przechowującą numery.
 */
static void phnumSortAndDeleteDuplicates(PhoneNumbers *pnum) {
   qsort(pnum->numbers, pnum->count, sizeof(*(pnum->numbers)), 
         (int(*)(void const *, void const *))numberComparator);

   size_t position = 1;
   for (size_t i = 1; i < pnum->count; i++) {
      if (position != i) {
         (pnum->numbers)[position] = (pnum->numbers)[i];
         (pnum->numbers)[i] = NULL;
      }

      if (strcmp((pnum->numbers)[position], 
                 (pnum->numbers)[position - 1]) == 0) {
         free((pnum->numbers)[position]);
         (pnum->numbers)[position] = NULL;
      }
      else {
         position++;
      }
   }
   pnum->count = position;
}

void phnumDelete(PhoneNumbers *pnum) {
   if (pnum == NULL)
      return;

   for (size_t i = 0; i < pnum->count; i++) {
      free((pnum->numbers)[i]);
      (pnum->numbers)[i] = NULL;
   }

   free(pnum->numbers);
   pnum->numbers = NULL;
   free(pnum);
   pnum = NULL;
}

char const * phnumGet(PhoneNumbers const *pnum, size_t const idx) {
   if (pnum == NULL || pnum->count <= idx)
      return NULL;

   return (pnum->numbers)[idx];
}

/** @struct Node
 * To jest struktura zawierająca wierzchołek drzewa 
 * (przekierowania przechowujemy na drzewie).
 */
struct Node;
/** @typedef Node
 * Definicja structury Node.
 */
typedef struct Node {
   char digit;             ///< Cyfra reprezentująca wierzchołek.
   char *forward;          ///< Przekierowanie.
   size_t forwardLength;   ///< Długość napisu reprezentującego przekierowanie.
   struct Node const *parent;    ///< Wskaźnik na rodzica.
   struct Node *subtrees[NUMBER_OF_DIGITS]; ///< Wskaźniki na synów.
} Node;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę, która przechowuje określony znak oraz początkowo
 * nie przechowuje żadnych przekierowań.
 * @param[in] digit  - znak, który będzie przechowywała struktura.
 * @param[in] parent - wskaźnik na ojca.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie powiodła się
 *         alokacja pamięci.
 */
static Node * createNode(char const digit, Node const *parent) {
   Node *node;
   node = malloc(sizeof(Node));
   if (node == NULL)
      return NULL;
   
   node->digit = digit;
   node->forward = NULL;
   node->forwardLength = 0;
   node->parent = parent;
   for (int i = 0; i < NUMBER_OF_DIGITS; i++)
      (node->subtrees)[i] = NULL;
   return node;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p node.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] node - wskaźnik na usuwaną strukturę.
 */
static void deleteNode(Node *node) {
   if (node == NULL)
      return;

   Node const *parentOfDeletedNode = node->parent;
   int startingSubtreeID = 0;

   while (node != parentOfDeletedNode) {
      int subtreeID = startingSubtreeID;
      while (subtreeID < NUMBER_OF_DIGITS && (node->subtrees)[subtreeID] == NULL)
         subtreeID++;

      if (subtreeID == NUMBER_OF_DIGITS) { // Wszystkie poddrzewa są usunięte.
         char const digit = node->digit;
         Node *previousNode;
         previousNode = node;
         node = (Node*)(node->parent);
         free(previousNode->forward);
         previousNode->forward = NULL;
         free(previousNode);
         previousNode = NULL;

         if (node != NULL)
            (node->subtrees)[digitID(digit)] = NULL;

         startingSubtreeID = (digitID(digit)) + 1;
      }
      else { // Usuwamy pierwsze z nieusuniętych poddrzew.
         node = (node->subtrees)[subtreeID];
         startingSubtreeID = 0;
      }
   }
}

/** @brief Wyznacza liczbę niepustych poddrzew wierzchołka @p node.
 * @param[in] node - wskaźnik na wierzchołek drzewa.
 * @return Liczba niepustych poddrzew wierzchołka @p node.
 */
static int nonEmptySubtrees(Node const *node) {
   int result = 0;
   for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
      if ((node->subtrees)[i] != NULL)
         result++;
   }
   return result;
}

struct PhoneForward {
   Node *node; ///< Wskaźnik na pierwszy wierzchołek drzewa trie.
};

PhoneForward * phfwdNew(void) {
   PhoneForward *pf;
   pf = malloc(sizeof(PhoneForward));
   if (pf == NULL)
      return NULL;
   
   pf->node = createNode('0', NULL);
   if (pf->node == NULL) {
      free(pf);
      pf = NULL;
      return NULL;
   }

   return pf;
}

void phfwdDelete(PhoneForward *pf) {
   if (pf == NULL)
      return;
   
   deleteNode(pf->node);
   pf->node = NULL;
   free(pf);
   pf = NULL;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
   size_t const length = numberLength(num2);
   if (pf == NULL || numberLength(num1) == 0 || length == 0
       || strcmp(num1, num2) == 0)
      return false;
   
   Node *node = pf->node;
   Node *newSubtree; // Wskaźnik na wierzchołek nowo dodanego poddrzewa
                     // (NULL, jeśli nie dodaliśmy żadnego nowego wierzchołka).
                     // Potrzebne, żebyśmy wiedzieli jakie wierzchołki usunąć
                     // w razie niepowodzenia alokacji.
   newSubtree = NULL;

   size_t position = 0;
   while (isDigit(num1[position])) {
      char const digit = num1[position];

      if ((node->subtrees)[digitID(digit)] == NULL) {
         (node->subtrees)[digitID(digit)] = createNode(digit, node);
         if ((node->subtrees)[digitID(digit)] == NULL) {
            node = NULL;
            deleteNode(newSubtree);
            return false;
         }

         if (newSubtree == NULL)
            newSubtree = (node->subtrees)[digitID(digit)];
      }

      node = (node->subtrees)[digitID(digit)];
      position++;
   }

   if (!reallocNumber(&(node->forward), length)) {
      node = NULL;
      deleteNode(newSubtree);
      return false;
   }

   for (size_t i = 0; i <= length; i++)
      (node->forward)[i] = num2[i];
   node->forwardLength = length;

   return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
   if (pf == NULL || numberLength(num) == 0)
      return;
   
   Node *node = pf->node;
   Node *previousNode = node;
   size_t position = 0;

   while (isDigit(num[position])) {
      char const digit = num[position];
      if ((node->subtrees)[digitID(digit)] == NULL)
         return;
      previousNode = node;
      node = (node->subtrees)[digitID(digit)];
      position++;
   }

   char const digit = num[position - 1];
   (previousNode->subtrees)[digitID(digit)] = NULL;
   deleteNode(node);

   // Usunięcie martwej ścieżki w drzewie
   node = previousNode;
   while (node->parent != NULL && node->forward == NULL 
          && nonEmptySubtrees(node) == 0) {
      char const digit = node->digit;
      previousNode = (Node*)(node->parent);
      (previousNode->subtrees)[digitID(digit)] = NULL;
      deleteNode(node);
      node = previousNode;
   }
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num) {
   if (pf == NULL)
      return NULL;

   PhoneNumbers *pnum = phnumCreate();
   size_t const numLength = numberLength(num);

   if (pnum == NULL || numLength == 0)
      return pnum;
   
   size_t longest = 0;
   char *prefix = NULL;
   size_t prefixLength = 0;
   Node *node = pf->node;
   size_t position = 0;

   while (node != NULL && isDigit(num[position])) {
      char const digit = num[position];
      node = (node->subtrees)[digitID(digit)];
      position++;

      if (node != NULL && node->forward != NULL) {
         longest = position;
         prefix = node->forward;
         prefixLength = node->forwardLength;
      }
   }

   size_t const length = numLength - longest + prefixLength;
   char *result = NULL;
   if (!reallocNumber(&result, length)) {
      phnumDelete(pnum);
      return NULL;
   }

   for (size_t i = 0; i < prefixLength; i++)
      result[i] = prefix[i];
   for (size_t i = longest; i <= numLength; i++)
      result[i - longest + prefixLength] = num[i];

   bool added = phnumAdd(pnum, result, length);
   free(result);

   if (added)
      return pnum;
   phnumDelete(pnum);
   return NULL;
}

/** @brief Dodaje numery należące do wyniku funkcji @ref phfwdReverse.
 * Sprawdza, czy przekierowanie przechowywane w wierzchołku @p node jest
 * prefiksem numeru @p num, jeżeli tak, to dodaje odpowiedni numer do 
 * struktury @p PhoneNumbers będącej wynikiem funkcji @ref phfwdReverse.
 * @param[in,out] pnum  - wskaźnik na strukturę przechowującą numery.
 * @param[in] node      - wskaźnik na wierzchołek drzewa z przekierowaniami.
 * @param[in] num       - wskaźnik na napis reprezentujący numer.
 * @param[in] numLength - długość numeru.
 * @param[in] depth     - głębokość, na jakiej znajduje się w drzewie 
 *                        wierzchołek @p node.
 * @return Wartość @p true, jeśli działanie funkcji przebiegło pomyślnie
 *         (nie wystąpił żaden błąd).
 *         Wartość @p false, jeśli wystąpił błąd alokacji pamięci.
 */
static bool AddReverseIfItExists(PhoneNumbers *pnum, Node *node, 
               char const *num, size_t const numLength, size_t const depth) {
   size_t const forwardLength = node->forwardLength;
   if (forwardLength == 0 || forwardLength > numLength)
      return true; // Funkcja się powiodła, nie dodano żadnego numeru.
      
   size_t pos = 0;
   while (pos < forwardLength && (node->forward)[pos] == num[pos])
      pos++;

   if (pos < forwardLength)
      return true; // Funkcja się powiodła, nie dodano żadnego numeru.
   
   size_t resultLength = numLength - forwardLength + depth;
   char *result = NULL;
   if (!reallocNumber(&result, resultLength))
      return false; // Wystąpił błąd alokacji.

   Node *auxiliaryNode = node;
   pos = depth - 1;
   while (auxiliaryNode->parent != NULL) {
      result[pos--] = auxiliaryNode->digit;
      auxiliaryNode = (Node*)(auxiliaryNode->parent);
   }
   
   pos = depth;
   for (size_t i = forwardLength; i <= numLength; i++)
      result[pos++] = num[i];

   bool added = phnumAdd(pnum, result, resultLength);
   free(result);

   return added;
}

PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num) {
   if (pf == NULL)
      return NULL;

   PhoneNumbers *pnum = phnumCreate();
   size_t const numLength = numberLength(num);

   if (pnum == NULL || numLength == 0)
      return pnum;

   if (!phnumAdd(pnum, num, numLength)) {
      phnumDelete(pnum);
      return NULL;
   }

   Node *node = pf->node;
   int startingSubtreeID = 0;
   size_t depth = 0;

   while (node != NULL) {
      int subtreeID = startingSubtreeID;
      while (subtreeID < NUMBER_OF_DIGITS && (node->subtrees)[subtreeID] == NULL)
         subtreeID++;

      if (subtreeID == NUMBER_OF_DIGITS) { // Wszystkie poddrzewa przejrzane.
         if (!AddReverseIfItExists(pnum, node, num, numLength, depth)) {
            phnumDelete(pnum);
            return NULL;
         }

         startingSubtreeID = (digitID(node->digit)) + 1;
         node = (Node*)(node->parent);
         depth--;
      }
      else {
         node = (node->subtrees)[subtreeID];
         startingSubtreeID = 0;
         depth++;
      }
   }

   phnumSortAndDeleteDuplicates(pnum);
   return pnum;
}

PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num) {
   if (pf == NULL)
      return NULL;

   PhoneNumbers *pnumReverse = phfwdReverse(pf, num);
   PhoneNumbers *pnumResult = phnumCreate();

   if (pnumReverse == NULL || pnumResult == NULL) {
      phnumDelete(pnumReverse);
      phnumDelete(pnumResult);
      return NULL;
   }

   for (size_t pos = 0; pos < pnumReverse->count; pos++) {
      char const *checkedNumber = phnumGet(pnumReverse, pos);
      PhoneNumbers *pnumGet = phfwdGet(pf, checkedNumber);
      if (pnumGet == NULL) {
         phnumDelete(pnumReverse);
         phnumDelete(pnumResult);
         return NULL;
      }

      char const *resultNumber = phnumGet(pnumGet, 0);
      if (resultNumber != NULL && strcmp(resultNumber, num) == 0) {
         // Numer należy do przeciwobrazu funkcji phfwdGet.
         if (!phnumAdd(pnumResult, checkedNumber, numberLength(checkedNumber))) {
            phnumDelete(pnumGet);
            phnumDelete(pnumReverse);
            phnumDelete(pnumResult);
            return NULL;
         }
      }

      phnumDelete(pnumGet);
   }

   phnumDelete(pnumReverse);
   return pnumResult;
}
