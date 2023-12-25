/*
 * --- PŔACTICA SISTEMAS OPERATIVOS ---
 * - Alfonso Pérez Sánchez
 */
#ifndef LIST
#define LIST

#include <stdbool.h>

#define LNULL NULL
#define MAX_SIZE_LIST 1024

typedef struct tItemL{
    int num;
    char string[MAX_SIZE_LIST];
} tItemL;
typedef struct tNode *tPosL;
typedef tPosL tList;

typedef struct tNode {
    tItemL data;
    tPosL next;
} tNode;

//GENERADORAS
void createEmptyList(tList* L);
bool insertItem(tItemL d, tList* L);

//MODIFICADORAS
bool copyList(tList L, tList* M);
void updateItem(tItemL d , tPosL p, tList* L);

//DESTRUCTORAS
void deleteAtPosition(tPosL p, tList* L);
void deleteList(tList* L);

//OBSERVADORAS
tPosL findItem(tItemL d, tList L);
bool isEmptyList(tList L);
tItemL getItem(tPosL p, tList L);
tPosL first(tList L) ;
tPosL last(tList L);
tPosL previous(tPosL p, tList L);
tPosL next(tPosL p, tList L);
tPosL findPosition(tList L, tItemL d);

#endif //LIST
