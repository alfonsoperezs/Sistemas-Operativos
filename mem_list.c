/*
 * --- PŔACTICA SISTEMAS OPERATIVOS ---
 * - Alfonso Pérez Sánchez
 */
#include <stdlib.h>
#include <stdbool.h>
#include "mem_list.h"


void createEmptyListM(tListM *L) {
    L->lastPos = -1;
}

bool isEmptyListM(tListM L) {
    return L.lastPos == -1;
}

tPosLM firstM(tListM L) {
    return 0;
}
tPosLM lastM(tListM L) {
    return L.lastPos;
}

tItemLM getItemM(tPosLM pos, tListM L) {
    return L.itemM[pos];
}

void deleteItemM(tPosLM pos, tListM *L) {
    for (int i = 0; i < lastM(*L); i++) {
        if (i >= pos) {
            L->itemM[i] = L->itemM[i + 1];
        }
    }
    L->lastPos--;
}

void deleteListM(tListM *L) {
    for (int i = 0; i <= L->lastPos; i++) {
        deleteItemM(i, L);
        L->lastPos--;
    }
}

bool insertItemM(tItemLM i, tListM *L) {
    bool fin = false;
    if (L->lastPos < MAX_SIZE_LISTM) {
        L->lastPos++;
        L->itemM[L->lastPos] = i;
        fin = true;
    } else {
        fin = false;
    }
    return fin;
}

