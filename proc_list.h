/*
 * --- PŔACTICA SISTEMAS OPERATIVOS ---
 * - Alfonso Pérez Sánchez
 */
#ifndef PRACTICA_PROC_LIST_H
#define PRACTICA_PROC_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#endif //PRACTICA_PROC_LIST_H

#define LNULLP (-1)
#define MAX_SIZE_LISTP 1024

typedef enum status {
    FINISHED, STOPPED, SIGNALED, ACTIVE
} status;

typedef struct tItemLP{
    pid_t pid;
    status estado;
    char user[MAX_SIZE_LISTP];
    char cmd[MAX_SIZE_LISTP];
    char date[MAX_SIZE_LISTP];
    int signal;
    int priority;
} tItemLP;

typedef int tPosLP;

typedef struct tListP{
    tItemLP itemP [MAX_SIZE_LISTP];
    int lastPos;
}tListP;

void createEmptyListP(tListP* L);
bool insertItemP(tItemLP d, tListP* L);
void deleteItemP(tPosLP pos, tListP *L);
void deleteListP(tListP* L);
bool isEmptyListP(tListP L);
tItemLP getItemP(tPosLP p, tListP L);
tPosLP firstP(tListP L);
tPosLP lastP(tListP L);
