/*
 * --- PŔACTICA SISTEMAS OPERATIVOS ---
 * - Alfonso Pérez Sánchez
 */
#ifndef P3_H
#define P3_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include "list.h"
#include "mem_list.h"
#include "proc_list.h"


#endif  // P3_H

#define MAX_SIZE 256
#define MAX_BUFFER 2048
#define TAMANO 2048
#define MAXVAR 2048


// comandos práctica 0
bool procesarEntrada(char command[MAX_SIZE], tList *historial, int *contador, tList *fileList,tListM *memList,
                     tListP *procList, char *envp[], char *environ[]);
int TrocearCadena(char *cadena, char *argumentos[]);
void actualizarHistorial(tList *historial, char command[MAX_SIZE]);
void inicializarFileList(tList *fileList);
void pwd();

void CMD_authors(char opcion[MAX_SIZE], int trozos);
void CMD_pid(char opcion[MAX_SIZE], int trozos);
void CMD_chdir(char opcion[MAX_SIZE], int trozos);
void CMD_date(int opcion);
void CMD_hist(char opcion[MAX_SIZE], int trozos, tList *historial);
void CMD_comand(char opcion[MAX_SIZE], int trozos, tList *historial, int *contador, tList *fileList, tListM *memList,
                tListP *procList, char *envp[], char *environ[]);
void CMD_open(char file[MAX_SIZE], char opcion[MAX_SIZE], int trozos, tList *fileList);
void CMD_close(char opcion[MAX_SIZE], int trozos, tList *fileList);
void CMD_dup(char opcion[MAX_SIZE], int trozos, tList *fileList);
void listopen(tList fileList);
void CMD_infosys();
void CMD_help(char opcion[MAX_SIZE], int trozos);

// comandos practica 1
void CMD_create(char param1[MAX_SIZE], char param2[MAX_SIZE], int trozos);
void CMD_stat(char *param[], int trozos);
void CMD_list(char *param[], int trozos);
void CMD_delete(char *param[], int trozos);
void CMD_deltree(char *param[], int trozos);

void notEmptyDirDelete(char nameDirectory[MAX_SIZE]);
void longStat(char *directorio, bool link, bool acc);
void shortStat(char *directorio);
char LetraTF (mode_t m);
char *ConvierteModo2 (mode_t m);
void fecha(time_t timeDir);
void longList(char *param, bool lng, bool acc, bool lnk, bool hid, bool reca, bool recb);
void contenidoDirectorio(char* param, bool listal, bool acc, bool link, bool hid, bool reca, bool recb);
void obtenerNombre(char * ruta,char * nombre);

// comandos practica 2
void CMD_malloc(char *param[], int trozos, tListM *M);
void CMD_shared(char *param[], int trozos, tListM *M);
void CMD_mmap(char *arg[],int trozos, tListM *M);
void CMD_read(char *ar[],int trozos);
void CMD_write(char *param[],int trozos);
void CMD_mem(char *param[],int trozos, tListM *M);
void CMD_recurse(char *param[], int trozos);

void mostrarMemList(tListM M, pid_t pid, cmd tElemento);
void SharedCreate(char *tr[], pid_t pid, tListM *M, int trozos);
void sharedFree(char *tr[], pid_t pid, tListM *M, int trozos);
void SharedDelkey (char *args[]);
void *ObtenerMemoriaShmget (key_t clave, size_t tam);
void *MapearFichero (char * fichero, int protection, tListM *M);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void *cadtop(char *string);
ssize_t EscribirFichero(char *f, void *p, size_t cont,int overwrite);
void Do_MemPmap(void);
void mostrarMemListCompleta(tListM M);
void borradoFinal(tListM *M);
void LlenarMemoria (void *p, size_t cont, unsigned char byte);
void imprimir_char (char c);
void imprimir_hex (char c);
void imprimir_mem (char *p, int nbytes);
void Recursiva (int n);

// comandos practica 3
void CMD_uid(char *param[], int trozos);
void CMD_showvar(char *param[], int trozos, char * arg3[]);
void CMD_changevar (char *param[], int trozos, char *arg3[], char *env[]);
void CMD_subsvar(char *param[], int trozos, char *arg3[], char *env[]);
void CMD_showenv(char *param[], int trozos, char *arg3[], char *env[]);
void CMD_fork(tListP *procList);
void CMD_exec(char *param[], int trozos, bool *isFinish, char *env[]);
void CMD_other(char *param[], int trozos, tListP *procList, char *env[]);
void CMD_jobs(tListP *procList);
void CMD_deljobs(char *param[], int trozos, tListP *procList);
void CMD_job(char *param[], int trozos, tListP *procList);


uid_t obtenerUid(char *nombre);
char *user(uid_t uid);
void mostrarCredencial();
int BuscarVariable (char *var, char *e[]);
void imprimirVariable(char *env[], char *name);
int CambiarVariable(char * var, char *nuevoVar, char *valor,  char *e[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
void exPrimerPlano(char *param[], int trozos, int numVariables, char *vars[], int i);
void exSegPlano(char *param[], int trozos, int numVariables, char *vars[], int i, tListP *procList);
void borrarProcesos(status estado, tListP *procList);
void imprimirProceso(tItemLP item);
tItemLP actualizarEstado(tItemLP item, int opcion);
const char * nombreSenal(int sen);
