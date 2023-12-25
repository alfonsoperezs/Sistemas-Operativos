/*
 * --- PŔACTICA SISTEMAS OPERATIVOS ---
 * - Alfonso Pérez Sánchez
 */
#ifndef MEM_LIST_H
#define MEM_LIST_H

#include <stdbool.h>
#include <stdlib.h>

#endif  // MEM_LIST_H

#define LNULLM (-1)
#define MAX_SIZE_LISTM 1024

typedef struct fich {
    int fd;
    char filename[256];
} tFich;

typedef enum cmd {
    MALLOC, SHARED, MMAP
} cmd;

typedef struct tItemLM{
    cmd cmdType;
    void *dir;
    size_t size;
    char date[128];
    union {
        int key;
        tFich fich;
    } Union;
} tItemLM;

typedef int tPosLM;

typedef struct tListM{
    tItemLM itemM [MAX_SIZE_LISTM];
    int lastPos;
}tListM;

void createEmptyListM(tListM* L);
bool insertItemM(tItemLM d, tListM* L);
void deleteItemM(tPosLM pos, tListM *L);
void deleteListM(tListM* L);
bool isEmptyListM(tListM L);
tItemLM getItemM(tPosLM p, tListM L);
tPosLM firstM(tListM L) ;
tPosLM lastM(tListM L);
