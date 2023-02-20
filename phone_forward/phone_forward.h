/** @file
 * Interfejs klasy przechowującej przekierowania numerów telefonicznych.
 *
 * @authors Marcin Peczarski <marpe@mimuw.edu.pl>, Wojciech Weremczuk <ww438808@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __PHONE_FORWARD_H__
#define __PHONE_FORWARD_H__

#include <stdbool.h>
#include <stddef.h>

/** @struct PhoneForward
 * To jest struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneForward;
/** @typedef PhoneForward
 * Definicja struktury PhoneForward.
 */
typedef struct PhoneForward PhoneForward;

/** @struct PhoneNumbers
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers;
/** @typedef PhoneNumbers
 * Definicja struktury PhoneNumbers.
 */
typedef struct PhoneNumbers PhoneNumbers;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
PhoneForward * phfwdNew(void);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(PhoneForward *pf);

/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * Relacja przekierowania numerów nie jest przechodnia.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num1   – wskaźnik na napis reprezentujący prefiks numerów
 *                     przekierowywanych;
 * @param[in] num2   – wskaźnik na napis reprezentujący prefiks numerów,
 *                     na które jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne, wskaźnik @p pf
 *         ma wartość NULL lub nie udało się alokować pamięci.
 */
bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2);

/** @brief Usuwa przekierowania.
 * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
 * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań,
 * wskaźnik @p pf ma wartość NULL lub napis nie reprezentuje numeru, 
 * nic nie robi.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą przekierowania
 *                     numerów;
 * @param[in] num    – wskaźnik na napis reprezentujący prefiks numerów.
 */
void phfwdRemove(PhoneForward *pf, char const *num);

/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest ciąg zawierający co najwyżej jeden numer. Jeśli dany
 * numer nie został przekierowany, to wynikiem jest ciąg zawierający ten numer.
 * Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty ciąg.
 * Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete. Funkcja phfwdGet przekazuje własność zwracanego 
 * wskaźnika funkcji, która ją wywołała.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci albo wskaźnik @p pf ma wartość NULL.
 */
PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num);

/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza następujący ciąg numerów: jeśli istnieje numer @p x, taki że dla
 * pewnego prefiksu @p x dodano przekierowanie zmieniające @p x w @p num, to 
 * numer @p x należy do wyniku wywołania @ref phfwdReverse z numerem @p num.
 * Dodatkowo ciąg wynikowy zawsze zawiera też numer @p num. Wynikowe numery są
 * posortowane leksykograficznie i nie mogą się powtarzać. Jeśli podany napis
 * nie reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers, która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci albo wskaźnik @p pf ma wartość NULL.
 */
PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete(PhoneNumbers *pnum);

/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera. Funkcja phnumGet nie przekazuje własności zwracanego 
 * wskaźnika funkcji, która ją wywołała.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  – indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const * phnumGet(PhoneNumbers const *pnum, size_t idx);

/** @brief Wyznacza przeciwobraz funkcji @ref phfwdGet.
 * Dla podanej za pomocą wskaźnika @p pf bazy przekierowań i podanego numeru
 * telefonu @p num wyznacza posortowaną leksykograficznie listę wszystkich 
 * takich numerów telefonów i tylko takich numerów telefonów @p x, że wynik
 * wywołania @ref phfwdGet z numerem @p x jest równy @p num. Wynikowe numery
 * nie mogą się powtarzać. Jeśli podany napis @p num nie reprezentuje numeru, 
 * wynikiem jest pusty ciąg. Funkcja ta alokuje strukturę @p PhoneNumbers, 
 * która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów lub NULL, gdy nie
 *         udało się alokować pamięci albo wskaźnik @p pf ma wartość NULL.
 */
PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num);

#endif /* __PHONE_FORWARD_H__ */
