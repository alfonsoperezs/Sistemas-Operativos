/*
 * --- PŔACTICA SISTEMAS OPERATIVOS ---
 * - Alfonso Pérez Sánchez
 */
#include "p3.h"

int int_g = 8;
float float_g = 4;
double double_g = 6.2;
int int_gn;
float float_gn;
double double_gn;
extern char **environ; // accede a la variable global environ


int main(int argc, char *argv[], char *envp[]){
    char command[MAX_SIZE];
    bool terminado = false;
    tList historial, fileList; //almacena los comandos que han sido introducidos por teclado
    int contador; //cuenta cuantas veces se ha ejecutado procesarentrada para evitar bucles infinitos
    tListM memList;
    tListP procList;

    createEmptyList(&historial);
    createEmptyList(&fileList);
    createEmptyListM(&memList);
    createEmptyListP(&procList);
    inicializarFileList(&fileList);
    while (!terminado){
        contador = 0;
        printf("-> ");
        fgets(command, sizeof(command),stdin);
        actualizarHistorial(&historial,command);
        terminado = procesarEntrada(command, &historial, &contador, &fileList, &memList, &procList, envp, environ);
    }
    borradoFinal(&memList);
    deleteList(&historial);
    deleteList(&fileList);
    deleteListM(&memList);
    deleteListP(&procList);
}

// practica 0
bool procesarEntrada(char command[MAX_SIZE], tList *historial, int *contador, tList *fileList,tListM *memList,
                     tListP *procList, char *envp[], char *environ[]){
    /*
     * Trocea la cadena de texto que se ha introducido por pantalla para posteriormente
     * poder ser ejecutado el comando correspondiente.
     */
    char *args[10];
    int trozos = TrocearCadena(command,args);;
    ++*contador;
    bool isFinish = false;
    if (trozos > 0) {
        if (strcmp(args[0], "authors") == 0) {
            CMD_authors(args[1], trozos);
        } else if (strcmp(args[0], "pid") == 0) {
            CMD_pid(args[1], trozos);
        } else if (strcmp(args[0], "chdir") == 0) {
            CMD_chdir(args[1], trozos);
        } else if (strcmp(args[0], "date") == 0) {
            CMD_date(0);
        } else if (strcmp(args[0], "time") == 0) {
            CMD_date(1);
        } else if (strcmp(args[0], "hist") == 0) {
            CMD_hist(args[1], trozos, historial);
        } else if (strcmp(args[0], "comand") == 0) {
            CMD_comand(args[1], trozos, historial, contador, fileList, memList, procList, envp, environ);
        } else if (strcmp(args[0], "open") == 0) {
            CMD_open(args[1], args[2], trozos, fileList);
        } else if (strcmp(args[0], "close") == 0) {
            CMD_close(args[1], trozos, fileList);
        } else if (strcmp(args[0], "dup") == 0) {
            CMD_dup(args[1], trozos, fileList);
        } else if (strcmp(args[0], "listopen") == 0) {
            if (trozos == 1) listopen(*fileList);
        } else if (strcmp(args[0], "infosys") == 0) {
            CMD_infosys();
        } else if (strcmp(args[0], "help") == 0) {
            CMD_help(args[1], trozos);
        } else if ((strcmp(args[0], "quit") == 0) || (strcmp(args[0], "exit") == 0) || (strcmp(args[0], "bye") == 0)) {
            isFinish = true;
        } else if (strcmp(args[0], "create") == 0) {
            CMD_create(args[1], args[2], trozos);
        } else if (strcmp(args[0], "stat") == 0) {
            CMD_stat(args, trozos);
        } else if (strcmp(args[0], "list") == 0) {
            CMD_list(args, trozos);
        } else if (strcmp(args[0], "delete") == 0) {
            CMD_delete(args, trozos);
        } else if (strcmp(args[0], "deltree") == 0) {
            CMD_deltree(args, trozos);
        } else if (strcmp(args[0], "malloc") == 0) {
            CMD_malloc(args, trozos, memList);
        } else if (strcmp(args[0], "shared") == 0) {
            CMD_shared(args, trozos, memList);
        } else if (strcmp(args[0], "mmap") == 0) {
            CMD_mmap(args, trozos, memList);
        } else if (strcmp(args[0], "read") == 0) {
            CMD_read(args, trozos);
        } else if (strcmp(args[0], "write") == 0) {
            CMD_write(args, trozos);
        } else if (strcmp(args[0], "mem") == 0) {
            CMD_mem(args, trozos, memList);
        } else if (strcmp(args[0], "recurse") == 0) {
            CMD_recurse(args, trozos);
        } else if (strcmp(args[0], "uid") == 0) {
            CMD_uid(args, trozos);
        } else if (strcmp(args[0], "showvar") == 0) {
            CMD_showvar(args, trozos, envp);
        } else if (strcmp(args[0], "changevar") == 0) {
            CMD_changevar(args, trozos, envp, environ);
        } else if (strcmp(args[0], "subsvar") == 0) {
            CMD_subsvar(args, trozos, envp, environ);
        } else if (strcmp(args[0], "showenv") == 0) {
            CMD_showenv(args, trozos, envp, environ);
        } else if (strcmp(args[0], "fork") == 0) {
            CMD_fork(procList);
        } else if (strcmp(args[0], "exec") == 0) {
            CMD_exec(args, trozos, &isFinish, environ);
        } else if (strcmp(args[0], "jobs") == 0) {
            CMD_jobs(procList);
        } else if (strcmp(args[0], "deljobs") == 0) {
            CMD_deljobs(args, trozos, procList);
        } else if (strcmp(args[0], "job") == 0) {
            CMD_job(args, trozos, procList);
        } else {
            CMD_other(args, trozos, procList, environ);
        }
    }
    return isFinish;
}
int TrocearCadena(char *cadena, char *argumentos[]){
    /*
     * Trocea el comando en trozos (cadenas de caracteres hasta llegar a un espacio)
     * y devuelve el numero de trozos
     */
    int i=1;
    if ((argumentos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((argumentos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}
void actualizarHistorial(tList *historial, char command[MAX_SIZE]){
    /*
     * Guarda en la lista el comando que acaba de ser introducido por teclado
     */
    tItemL item;

    if(isEmptyList(*historial)){
        item.num = 1;
    } else{
        item.num = getItem(last(*historial),*historial).num + 1;
    }
    strcpy(item.string,command);
    insertItem(item,historial);
}
void inicializarFileList(tList *fileList){
    /*
     * Inicializa la lista de ficheros abiertos con la entrada, salida
     * y error estandar.
     */
    tItemL item;

    item.num = 0;
    strcpy(item.string,"entrada estandar");
    insertItem(item,fileList);

    item.num = 1;
    strcpy(item.string,"salida estandar");
    insertItem(item,fileList);

    item.num = 2;
    strcpy(item.string,"error estandar");
    insertItem(item,fileList);
}
void pwd(){
    /*
     * Muestra el directorio actual
     */
    char directorio[MAX_SIZE];

    if(getcwd(directorio,MAX_SIZE) != NULL)
        printf("%s\n", directorio);
}

void CMD_authors(char opcion[MAX_SIZE], int trozos){
    /*
     * authors: Imprime los nombres y los nombres de usuario de los autores del programa
     * [-l] : Imprime los nombres de usuario
     * [-n] : Imprime los nombres
     */
    if(trozos == 1){
        puts("Alfonso Perez Sanchez");
    } else{
        if(strcmp(opcion,"-l") == 0){
            puts("user");
        } else if (strcmp(opcion,"-n") == 0) {
            puts("Alfonso Perez Sanchez");
        }
    }
}
void CMD_pid(char opcion[MAX_SIZE], int trozos){
    /*
     * pid: Muestra el PID del proceso actual de la shell
     * [-p] : Muestra el PID del proceso padre de la shell
     */
    pid_t pid;

    if(trozos == 1){
        pid = getpid();
        printf("Pid de shell: %d\n",pid);
    } else{
        pid = getppid();
        printf("Pid del padre del shell: %d\n",pid);
    }
}
void CMD_chdir(char opcion[MAX_SIZE], int trozos){
    /*
     * chdir [directorio] : Cambia el actual directorio del shell al indicado en el comando.
     *        Si no tiene ningún argumento, mostrará el directorio actual.
     */

    if(trozos == 1){
        pwd();
    } else{
        if(chdir(opcion) != 0)
            perror("Imposible cambiar directorio");

    }
}
void CMD_date(int opcion){
    /*
     * opcion = 0 (date): Imprime por pantalla la fecha actual en formato DD/MM/YYYY
     * opcion = 1 (time): Imprime por pantalla la hora actual en formato HH:MM:SS
     */
    time_t currentTime;
    struct tm *tm;
    char data[MAX_SIZE];

    time(&currentTime); //obtiene la fecha en segundos
    tm = localtime(&currentTime); //convierte la fecha
    if(opcion == 0)
        strftime(data, MAX_SIZE, "%d/%m/%Y",tm); //formatea la fecha
    if(opcion == 1)
        strftime(data, MAX_SIZE, "%H:%M:%S",tm);
    printf("%s\n",data);
}
void CMD_hist(char opcion[MAX_SIZE], int trozos, tList *historial){
    /*
     * hist: Muestra/borra los comandos que han sido ejecutados.
     *  [no opción] : Muestra por pantalla los comandos que han sido ejecutado por orden
     *  [-c] : Borra el historial de los comandos que han sido ejecutados
     *  [-N] : Muestra por pantalla los primeros N comandos que han sido ejecutado
     */
    int N;
    tItemL item;

    if(trozos == 1){ //[no opción]
        for(tPosL pos = first(*historial); pos != LNULL; pos = next(pos,*historial)){
            item = getItem(pos,*historial);
            printf("%d -> %s",item.num,item.string);
        }
    } else{
        if(strcmp(opcion,"-c") == 0){ //[-c]
            while (!isEmptyList(*historial))
                deleteAtPosition(last(*historial),historial);
        } else if(opcion[0] == '-'){ //[-N]
            N = atoi(opcion + 1);
            for(tPosL pos = first(*historial); N != 0 && pos != LNULL; pos = next(pos,*historial)){
                item = getItem(pos,*historial);
                printf("%d -> %s",item.num,item.string);
                --N;
            }
        }
    }
}
void CMD_comand(char opcion[MAX_SIZE], int trozos, tList *historial, int *contador, tList *fileList, tListM *memList,
                tListP *procList, char *envp[], char *environ[]){
    /*
     * comand N : Ejecuta el comando N del historial.
     *            Si no tiene argumento, mostrará el historial de comandos.
     */
    int N;
    tItemL item;

    item.num = 0; //inicializar item para que no haya errores en la primera iteracción de comparación
    if(trozos == 1){
        CMD_hist(NULL,1,historial);
    } else{
        N = atoi(opcion);
        if (N > 0) {
            for (tPosL pos = first(*historial); item.num != N && pos != LNULL; pos = next(pos, *historial)) {
                item = getItem(pos, *historial);
            }
            if(item.num != N){
                printf("No hay elemento %d en el historico\n",N);
            } else{
                printf("Ejecutando hist (%d): %s",item.num,item.string);
                if(*contador < 6) //impide bucle infinito
                    procesarEntrada(item.string,historial, contador, fileList, memList,procList,envp,environ);
                else
                    puts("Demasiada recursion en hist");
            }
        }
    }
}
void CMD_open(char file[MAX_SIZE], char opcion[MAX_SIZE],int trozos, tList *fileList){
    /*
     * Abre un archivo y lo agrega (junto con el descriptor del archivo y el
     * modo de apertura a la lista de archivos abiertos del shell.
     */
    int i, df, mode = 0;
    tItemL item;
    char ruta[MAX_SIZE] = "";

    if (trozos == 1) {
        listopen(*fileList);
    } else{
        // obtenemos la ruta del directorio actual

        if(file[0] != '/'){
            if (getcwd(ruta, sizeof(ruta)) == NULL) {
                perror("Error al obtener el directorio actual");
            }
            strcat(ruta, "/");
            strcat(ruta, file);
        } else{
            strcat(ruta, file);
        }

        printf("%s\n",ruta);

        if(trozos >= 3){
            for (i = 3; i <= trozos ; ++i) {
                if (!strcmp(opcion, "cr")) mode |= O_CREAT;
                else if (!strcmp(opcion, "ex")) mode |= O_EXCL;
                else if (!strcmp(opcion, "ro")) mode |= O_RDONLY;
                else if (!strcmp(opcion, "wo")) mode |= O_WRONLY;
                else if (!strcmp(opcion, "rw")) mode |= O_RDWR;
                else if (!strcmp(opcion, "ap")) mode |= O_APPEND;
                else if (!strcmp(opcion, "tr")) mode |= O_TRUNC;
            }

        }
        if ((df = open(ruta, mode, 0777)) == -1)
            perror("Imposible abrir fichero");
        else {
            item.num = df;
            strcpy(item.string,ruta);
            insertItem(item,fileList);
            printf("Anadida entrada %d a la tabla ficheros abiertos\n", df);
        }
    }
}
void CMD_close(char opcion[MAX_SIZE], int trozos, tList *fileList){
    /*
     * Cierra el descriptor del archivo df y elimina el elemento correspondiente de
     * la lista
     */
    int df;
    tItemL item;

    if(trozos == 1){
        listopen(*fileList);
    } else{
        df = atoi(opcion);
        if (close(df) == -1)
            perror("Imposible cerrar descriptor");
        else{
            item.num = df;
            deleteAtPosition(findItem(item,*fileList),fileList);
        }
    }
}
void CMD_dup(char opcion[MAX_SIZE], int trozos, tList *fileList){
    /*
     *  dup df : Duplica el descriptor de fichero df y anade una nueva entrada a la lista ficheros abiertos
     */
    int df;
    tItemL item, dupFile;
    char aux[MAX_BUFFER], *p;

    if(trozos == 1){
        listopen(*fileList);
    } else{
        df = dup(atoi(opcion));
        if(df == -1){
            perror("No se pudo duplicar el archivo");
        } else{
            item.num = atoi(opcion);
            dupFile = getItem(findItem(item,*fileList),*fileList);
            p = dupFile.string;
            sprintf (aux,"dup %d (%s)\n",df, p);
            item.num = df;
            strcpy(item.string,aux);
            insertItem(item,fileList);
        }
    }
}
void listopen(tList fileList){
    int mode;
    tItemL item;

    for(tPosL pos = first(fileList); pos != LNULL; pos = next(pos, fileList)){
        item = getItem(pos,fileList);
        mode = fcntl(item.num, F_GETFL);

        printf("descriptor: %d -> %s ", item.num, item.string);
        if (mode & O_CREAT) printf("O_CREAT\n");
        else if (mode & O_EXCL) printf("O_EXCL\n");
        else if (mode & O_WRONLY) printf("O_WRONLY\n");
        else if (mode & O_RDWR) printf("O_RDWR\n");
        else if (mode & O_APPEND) printf("O_APPEND\n");
        else if (mode & O_TRUNC) printf("O_TRUNC\n");
        else printf("O_RDONLY\n");
    }
}
void CMD_infosys(){
    /*
     * infosys: Muestra información de la máquina en la que se ejecuta el shell.
     */
    struct utsname infosys;

    if (uname(&infosys) == -1) {
        perror("ERROR");
    }

    printf("Informacion del sistema:\n");
    printf("System Name: %s\n", infosys.sysname);
    printf("Node Name: %s\n", infosys.nodename);
    printf("Release: %s\n", infosys.release);
    printf("Version: %s\n", infosys.version);
    printf("Machine: %s\n", infosys.machine);
}
void CMD_help(char opcion[MAX_SIZE], int trozos){
    /*
     * help : Muestra una lista de comandos disponibles.
     * [cmd] : Muestra información de como usar el comando "cmd".
     */
    if(trozos == 1){
        puts("'help [cmd]' ayuda sobre comandos");
        puts("\tComandos disponibles:");
        printf("authors pid chdir date time hist comand open\n");
        printf("close dup listopen infosys help quit exit bye\n");
        printf("create delete deltree stat list\n");
        printf("malloc shared mmap memdump memfill mem recurse \n");
        puts("uid showvar changevar subsvar showenv fork exec jobs deljobs job");
    } else{
        if(strcmp(opcion,"authors") == 0) printf("authors [-l|-n]: Prints the names and logins of the program authors.\n");
        if(strcmp(opcion,"pid") == 0) printf("pid [-p]: Prints the pid of the process executing the shell.\n");
        if(strcmp(opcion,"chdir") == 0) printf("chdir [dir]: Changes the current working directory of the shell to dir.\n");
        if(strcmp(opcion,"date") == 0) printf("date: Prints the current date in the format DD/MM/YYYY.\n");
        if(strcmp(opcion,"time") == 0) printf("time: Prints the current time in the format hh:mm:ss.\n");
        if(strcmp(opcion,"hist") == 0){
            puts("hist [-c|-N]: Shows/clears the historic of commands executed by this shell.");
            puts("\t– hist: Prints all the comands that have been input with their order number");
            puts("\t– hist -N: Prints the first N comands");
            puts("\t– hist -c: Clears (empties) the list of historic commands.");
        }
        if(strcmp(opcion,"comand") == 0) printf("comand N: Repeats command number N (from historic list).\n");
        if(strcmp(opcion,"open") == 0) printf("open [file] mode: Opens a file and adds it to the list of shell open files.\n");
        if(strcmp(opcion,"close") == 0) printf("close [df]: Closes the df file descriptor and eliminates the corresponding item from the list.\n");
        if(strcmp(opcion,"dup") == 0) printf("dup [df]: Duplicates the df file descriptor.\n");
        if(strcmp(opcion,"listopen") == 0) printf("listopen: Lists the shell open files.\n");
        if(strcmp(opcion,"infosys") == 0) printf("infosys: Prints information on the machine running the shell.\n");
        if(strcmp(opcion,"help") == 0) printf("help [cmd]: Displays a list of available commands or gives a brief help on the usage of a command.\n");
        if(strcmp(opcion,"quit") == 0 || strcmp(opcion,"bye") == 0 ||strcmp(opcion,"exit") == 0){
            printf("%s: Ends the shell.\n",opcion);
        }
        if(strcmp(opcion,"create") == 0) printf("create [-f] [name]: Crea un directorio o un fichero (-f)\n");
        if(strcmp(opcion,"stat") == 0) printf("stat [-long][-link][-acc] name1 name2 .. :lista ficheros\n");
        if(strcmp(opcion,"list") == 0) printf("list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 .. :lista contenidos de directorios\n");
        if(strcmp(opcion,"delete") == 0) printf("delete [name1 name2 ..]: Borra ficheros o directorios vacios\n");
        if(strcmp(opcion,"deltree") == 0) printf("deltree [name1 name2 ..]: Borra ficheros o directorios no vacios recursivamente\n");
        if(strcmp(opcion,"malloc") == 0) printf("malloc [-free] [tam]    asigna un bloque memoria de tamano tam con malloc\n        -free: desasigna un bloque de memoria de tamano tam asignado con malloc\n");
        if(strcmp(opcion,"shared") == 0) printf("shared [-free|-create|-delkey] cl [tam] asigna memoria compartida con clave cl en el programa\n-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n-free cl: desmapea el bloque de memoria compartida de clave cl\n-delkey clelimina del sistema (sin desmapear) la clave de memoria cl\n");
        if(strcmp(opcion,"mmap") == 0) printf("mmap [-free] fich prm   mapea el fichero fich con permisos prm\n-free fich: desmapea el ficherofich\n");
        if(strcmp(opcion,"memdump") == 0) printf("memdump addr cont       Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");
        if(strcmp(opcion,"memfill") == 0) printf("memfill addr cont byte  Llena la memoria a partir de addr con byte\n");
        if(strcmp(opcion,"mem") == 0) printf("mem [-blocks|-funcs|-vars|-all|-pmap] ..        Muestra muestra detalles de la memoria del proceso\n-blocks: los bloques de memoria asignados\n-funcs: las direcciones de las funciones\n-vars: las direcciones de las variables\n:-all: todo\n-pmap: muestra la salida del comando pmap(o similar)\n");
        if(strcmp(opcion,"recurse") == 0) printf("recurse [n]     Invoca a la funcion recursiva n veces\n");
        if(strcmp(opcion,"uid") == 0) printf("uid [-get|-set] [-l] [id]        Accede a las credenciales del proceso que ejecuta el shell\n -get: muestra las credenciales\n-set id: establece la credencial al valor numerico id\n -set -l id: establece la credencial a login id\n");
        if(strcmp(opcion,"showvar") == 0) puts("showvar var     Muestra el valor y las direcciones de la variable de entorno var");
        if(strcmp(opcion,"changevar") == 0) printf("changevar [-a|-e|-p] var valor  Cambia el valor de una variable de entorno\n -a: accede por el tercer arg de main\n -e: accede mediante environ\n -p: accede mediante putenv\n");
        if(strcmp(opcion,"subsvar") == 0) printf("subsvar [-a|-e] var1 var2 valor Sustituye la variable de entorno var1 con var2=valor\n-a: accede por el tercer arg de main\n -e: accede mediante environ\n");
        if(strcmp(opcion,"showenv") == 0) printf("showenv [-environ|-addr]         Muestra el entorno del proceso\n-environ: accede usando environ (en lugar del tercer arg de main)\n-addr: muestra el valor y donde se almacenan environ y el 3er arg main\n");
        if(strcmp(opcion,"fork") == 0) puts("fork    El shell hace fork y queda en espera a que su hijo termine");
        if(strcmp(opcion,"exec") == 0) puts("exec VAR1 VAR2 ..prog args....[@pri] ");
        if(strcmp(opcion,"jobs") == 0) puts("jobs    Lista los procesos en segundo plano");
        if(strcmp(opcion,"deljobs") == 0) puts("deljobs [-term][-sig]   Elimina los procesos de la lista procesos en sp");
        if(strcmp(opcion,"job") == 0) puts("job [-fg] pid   Muestra informacion del proceso pid.");
    }
}

// practica 1
void notEmptyDirDelete(char nameDirectory[MAX_SIZE]){
    /*
     * Borra recursivamente los directorios/archivos.
     */
    DIR *directorio = opendir(nameDirectory);
    struct dirent *entradaDirectorio;
    char entradaDirectorioPATH[MAX_BUFFER];
    struct stat fileStat;

    if(directorio == NULL){
        perror("Error al borrar el archivo");
    } else{
        while ((entradaDirectorio = readdir(directorio)) != NULL){ // se leen todas las entradas del directorio
            // se omite borrado de /. y /..
            if (entradaDirectorio->d_name[0] != '.') {
                snprintf(entradaDirectorioPATH, sizeof(entradaDirectorioPATH), "%s/%s",
                         nameDirectory, entradaDirectorio->d_name);
                if(stat(entradaDirectorioPATH,&fileStat) == 0){
                    if(S_ISDIR(fileStat.st_mode)){ // si es un directorio
                        notEmptyDirDelete(entradaDirectorioPATH);
                    } else if (S_ISREG(fileStat.st_mode)){ // si es un fichero
                        if(unlink(entradaDirectorioPATH) != 0){
                            perror("Imposible borrar");
                        }
                    }
                } else{
                    perror("Imposible borrar");
                }
            }
        }
        closedir(directorio);
    }
    if(rmdir(nameDirectory)!=0){
        perror("Imposible borrar");
    }
}
void longStat(char *directorio, bool lnk, bool acc){
    /*
     * Imprime stat en formato largo (se ha introducido como mínimo
     * la opción "-long").
     */
    struct stat stats;
    struct group *grp;
    struct passwd *usr;
    char user[MAX_SIZE], group[MAX_SIZE], symblnk[MAX_SIZE] = "", nombreDirectorio[MAX_SIZE];
    ssize_t numBytes, bufferSize;

    obtenerNombre(directorio,nombreDirectorio);
    if(lstat(directorio,&stats) == 0){
        if(acc){ // fecha
            fecha(stats.st_atime);
        } else{
            fecha(stats.st_mtime);
        }
        if((usr = getpwuid(stats.st_uid))!=NULL){ // nombre
            sprintf(user, "%s", usr->pw_name);
        }else{
            sprintf(user, "%d", stats.st_uid);
        }
        if((grp = getgrgid(stats.st_gid))!=NULL){ // grupo
            sprintf(group, "%s", grp->gr_name);
        }else{
            sprintf(group, "%d", stats.st_gid);
        }
        printf("%4lu (%8lu) %8s %8s %s\t%ld%2s", stats.st_nlink, stats.st_ino, user, group,
               ConvierteModo2(stats.st_mode), stats.st_size, nombreDirectorio);
        if(lnk == true && S_ISLNK(stats.st_mode)){
            bufferSize = stats.st_size;
            numBytes = readlink(directorio,symblnk,bufferSize);
            if(numBytes < 0){
                perror("**** error al acceder");
            } else{
                printf(" -> %s", symblnk);
            }
        }
        puts("");
    } else{
        perror("**** error al acceder");
    }
}
void shortStat(char *directorio){
    /*
     * Imprime stat en caso de que no se intruzca ninguna opción
     */
    struct stat stats;
    char nombreDirectorio[MAX_SIZE];

    obtenerNombre(directorio,nombreDirectorio);

    if(lstat(directorio,&stats) == 0){
        printf("%9ld %s\n", stats.st_size, nombreDirectorio);
    } else{
        perror("**** error al acceder");
    }
}
char LetraTF (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}
char *ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}
void fecha(time_t timeDir){
    /*
     * Imprime la fecha de un directorio (acceso o modificación)
     */
    struct tm tm;
    tm = *localtime(&timeDir);
    printf("%04d/%02d/%02d-%02d:%02d ", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,  tm.tm_hour, tm.tm_min);
}
void contenidoDirectorio(char* param, bool lng, bool acc, bool lnk, bool hid, bool reca, bool recb){
    /*
     * Recorre los directorios y sus subdirectorios.
     */
    DIR * directorio = opendir(param);
    struct dirent * entradaDirectorio;
    char rutaDirectorio[MAX_BUFFER];

    if(directorio != NULL) {
        while ((entradaDirectorio = readdir(directorio)) != NULL){
            sprintf(rutaDirectorio, "%s/%s", param, entradaDirectorio->d_name);
            if (strcmp(entradaDirectorio->d_name, ".") != 0 && strcmp(entradaDirectorio->d_name, "..") != 0){
                // se muestra por pantalla el contenido de todos los directorios menos el de los ocultos
                longList(rutaDirectorio,lng,acc,lnk,hid,reca,recb);
            }
        }
        closedir(directorio);
    }
}
void longList(char *param, bool lng, bool acc, bool lnk, bool hid, bool reca, bool recb){
    /*
     * Muestra por pantalla el contenido de los directorios.
     */
    DIR * directorio = opendir(param);
    struct dirent * entradaDirectorio;
    char rutaDirectorio[MAX_BUFFER];

    if(directorio != NULL) {
        if(recb) contenidoDirectorio(param,lng,acc,lnk,hid,false,recb); // si se incluye -recb
        // imprimir contenido
        printf("**********%s\n", param);
        while ((entradaDirectorio = readdir(directorio)) != NULL){
            sprintf(rutaDirectorio, "%s/%s", param, entradaDirectorio->d_name);
            if((entradaDirectorio->d_name[0] != '.') || (hid && entradaDirectorio->d_name[0] == '.')){
                //ignora /. y /.. excepto cuando se incluye -hid
                if(lng){
                    longStat(rutaDirectorio,lnk,acc); //formato largo
                } else{
                    shortStat(rutaDirectorio); //formato corto
                }
            }
        }
        if(reca) contenidoDirectorio(param,lng,acc,lnk,hid,reca,false); // si se incluye -reca
        closedir(directorio);
    }
}
void obtenerNombre(char * ruta,char * nombre){
    /*
     * Dada una ruta, obtiene el nombre del directorio o fichero actual
     */
    const char *lastSlash = strrchr(ruta, '/');

    if (lastSlash != NULL) {
        // Copia el nombre del archivo (después de la última barra) en nombreArchivo
        strcpy(nombre, lastSlash + 1);
    } else {
        // Si no se encontró una barra, copia la ruta completa en nombreArchivo
        strcpy(nombre, ruta);
    }

}

void CMD_create(char param1[MAX_SIZE], char param2[MAX_SIZE], int trozos){
    /*
     * create [-f] [nombre_fichero]: crea un directorio o un fichero
     * si se incluye la opcion -f
     */
    FILE *fichero;
    switch (trozos) {
        case 1:
            pwd();
            break;
        case 2:
            if(strcmp(param1,"-f") == 0){
                pwd();
            } else{
                if(mkdir(param1,0777) != 0){ //crea el directorio
                    perror("Imposible crear");
                }
            }
            break;
        default:
            if(strcmp(param1,"-f") == 0){
                fichero = fopen(param2, "w");
                if (fichero == NULL) { //verifica que se haya podido crear el archivo
                    perror("No se pudo crear el archivo");
                } else{
                    fclose(fichero);
                }
            } else{
                if(mkdir(param1,0777) != 0){
                    perror("Imposible crear");
                }
            }
            break;
    }
}
void CMD_delete(char *param[], int trozos){
    /*
     * delete [files] : borra los ficheros/directorios
     */
    int i;
    char *name;
    struct stat fileStat;

    if(trozos == 1){
        pwd();
    } else{
        for(i = 1; i < trozos; ++i){
            name = param[i];
            if(stat(name,&fileStat) == 0){
                if(S_ISDIR(fileStat.st_mode)){
                    if(rmdir(name) != 0){
                        perror("Imposible borrar");
                    }
                } else if (S_ISREG(fileStat.st_mode)){
                    if(remove(name) != 0){
                        perror("Imposible borrar");
                    }
                }
            } else{
                perror("Imposible borrar");
            }
        }
    }
}
void CMD_deltree(char *param[], int trozos){
    /*
     * deltree [dir1 dir2 ...]: Borra ficheros y directorios no vacíos
     */
    int i;
    char *name;
    struct stat fileStat;

    if(trozos == 1){
        pwd();
    } else{
        for (i = 1; i < trozos; ++i){
            name = param[i];
            if(stat(name,&fileStat) == 0){
                if(S_ISDIR(fileStat.st_mode)){
                    notEmptyDirDelete(param[i]);
                } else if (S_ISREG(fileStat.st_mode)){
                    if(remove(name) != 0){
                        perror("Imposible borrar");
                    }
                }
            } else{
                perror("Imposible borrar");
            }
        }
    }
}
void CMD_list(char *param[], int trozos){
    /*
     * list [-reca] [-recb] [-hid][-long][-link][-acc] [n1 n2 .. ]:
     *  Lista los contenidos de los directorios
     */
    bool reca = false, recb = false, hid = false, lng = false, acc = false, lnk = false;
    int numOptions = 0, i, dirName = 0;
    struct stat stats;
    char * name;

    for (i = 1; i < trozos; ++i){ //recorremos los parametros para obtener las opciones
        if(strcmp(param[i],"-long") == 0){
            lng = true;
            ++numOptions;
        } else if(strcmp(param[i],"-hid") == 0){
            hid = true;
            ++numOptions;
        } else if(strcmp(param[i], "-acc") == 0){
            acc = true;
            ++numOptions;
        } else if(strcmp(param[i],"-recb") == 0){
            recb = true;
            if(reca) reca = false;
            ++numOptions;
        } else if(strcmp(param[i],"-reca") == 0){
            reca = true;
            if(recb) recb = false;
            ++numOptions;
        } else if(strcmp(param[i],"-link") == 0){
            lnk = true;
            ++numOptions;
        } else{
            ++dirName;
        }
    }
    if(trozos == 1 || dirName == 0){
        pwd();
    } else{
        for(i = 1 + numOptions; i < trozos; ++i){
            name = param[i];
            if(stat(name,&stats) == 0){
                if(S_ISDIR(stats.st_mode)){ // se trata de un directorio
                    longList(param[i],lng,acc,lnk,hid,reca,recb);
                } else {
                    if(lng){ // se trata de un fichero
                        // formato largo
                        longStat(param[i],lnk,acc);
                    } else{
                        // formato corto
                        shortStat(param[i]);
                    }
                }
            } else{
                perror("No se ha podido abrir");
            }

        }
    }
}
void CMD_stat(char *param[], int trozos){
    bool lng = false, lnk = false, acc = false;
    int i, dirName = 0, numOptions = 0;

    for (i = 1; i < trozos; ++i){ //recorremos los parametros para obtener las opciones
        if(strcmp(param[i],"-long") == 0){
            lng = true;
            ++numOptions;
        } else if(strcmp(param[i],"-link") == 0){
            lnk = true;
            ++numOptions;
        } else if(strcmp(param[i], "-acc") == 0){
            acc = true;
            ++numOptions;
        } else{
            ++dirName;
        }
    }
    if(trozos == 1 || dirName == 0){
        pwd();
    } else{
        for(i = 1 + numOptions; i < trozos; ++i){
            if(lng){
                // formato largo
                longStat(param[i],lnk,acc);
            } else{
                // formato corto
                shortStat(param[i]);
            }
        }
    }
}

// practica 2
void CMD_malloc(char *param[], int trozos, tListM *M) {
    /*
     * malloc [-free] [tam] : asigna un bloque memoria de tamano
     *                        tam con malloc
     * -free: desasigna un bloque de memoria de tamano tam asignado
     *         con malloc
     */
    pid_t pid = getpid();
    size_t size;
    time_t t = time(0);
    struct tm *tlocal = localtime(&t);
    tItemLM item;
    tPosLM pos;
    bool encontrado = false;

    if (trozos == 1) { // muestra por pantalla memlist
        mostrarMemList(*M, pid, MALLOC);
    } else {
        if (strcmp(param[1], "-free") == 0) { // opcion free
            if (trozos == 2) { // muestra por pantalla memlist
                mostrarMemList(*M, pid, MALLOC);
            } else { // operacion free
                size = atoi(param[2]);
                if (size <= 0) printf("No se borran bloques de 0 bytes\n");
                else {
                    if (!isEmptyListM(*M)) {
                        for (pos = firstM(*M); pos <= lastM(*M) || !encontrado; ++pos) {
                            // busca el primer elemento con la cantidad de bytes introducida
                            item = getItemM(pos, *M);
                            if (item.cmdType == MALLOC) {
                                if (item.size == atoi(param[2])) {
                                    free(item.dir);
                                    encontrado = true;
                                    deleteItemM(pos, M);
                                }
                            }
                        }
                    }
                }
            }
        } else { // operacion malloc
            size = atoi(param[1]); // almacena el tamaño del malloc
            if (size <= 0) printf("No se asignan bloques de 0 bytes\n");
            else {
                item.cmdType = MALLOC;
                item.size = size;
                item.dir = malloc(item.size); // realiza la funcion malloc
                strftime(item.date, 128, "%b %d %H:%M", tlocal);
                if (item.dir == NULL) { // caso ha fallado malloc()
                    puts("Error: No se pudo reservar memoria");
                } else {
                    if (insertItemM(item, M)) { // se pudo insertar en la lista
                        printf("Asignados %zu bytes en %p\n", item.size, item.dir);
                    } else { // no se pudo insertar en la lista
                        free(item.dir); // liberamos memoria al producirse este error
                        puts("Error: No se pudo reservar memoria");
                    }
                }
            }
        }
    }
}
void CMD_shared(char *param[], int trozos, tListM *M) {
    /*
     * shared [-free|-create|-delkey] cl [tam] asigna memoria compartida con clave cl en el programa
     *  -create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam
     *  -free cl: desmapea el bloque de memoria compartida de clave cl
     *  -delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl
     */
    pid_t pid = getpid();
    if (trozos < 2) {
        mostrarMemList(*M, pid, SHARED);
    } else {
        if (strcmp(param[1], "-create") == 0) {
            SharedCreate(param, pid, M, trozos);
        } else if (strcmp(param[1], "-free") == 0) {
            sharedFree(param, pid, M, trozos);
        } else if (strcmp(param[1], "-delkey") == 0) {
            SharedDelkey(param);
        }
    }

}
void CMD_mmap(char *arg[], int trozos, tListM *M) {
    char *perm;
    void *p;
    int protection = 0;
    pid_t pid = getpid();
    tItemLM item;
    tPosLM pos;
    bool encontrado = false;

    if (trozos < 2) {
        mostrarMemList(*M, pid, MMAP);
    } else {
        if (strcmp(arg[1], "-free") == 0) { // free
            for (pos = firstM(*M); pos <= lastM(*M); ++pos) { // se busca el fichero en la lista
                item = getItemM(pos, *M);
                if (item.cmdType == MMAP) {
                    if (strcmp(item.Union.fich.filename, arg[2]) == 0) {
                        if (munmap(item.dir, item.size) == -1) { // desmapear fichero
                            perror("No se puede eliminar");
                        } else {
                            close(item.Union.fich.fd);
                            deleteItemM(pos, M);
                            encontrado = true;
                        }
                    }
                }
            }
            if (encontrado == false) {
                printf("Error: El fichero no estaba mapeado\n");
            }
        } else { // mapea ficheros en memoria
            if ((perm = arg[2]) != NULL && strlen(perm) < 4) {
                if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
                if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
                if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
            }
            if ((p = MapearFichero(arg[1], protection, M)) == NULL)
                perror("Imposible mapear fichero");
            else
                printf("fichero %s mapeado en %p\n", arg[1], p);
        }
    }

}
void CMD_read(char *ar[], int trozos){
    /*
     * read fiche addr cont : Lee cont bytes desde fich a la direccion addr
     */
    void *p;
    size_t cont = -1;  // -1 -> leer el fichero completo
    ssize_t n;
    if (trozos < 3) {
        printf("faltan parametros\n");
        return;
    }
    p = cadtop(ar[2]);  /*convertimos de cadena a puntero*/
    if (ar[3] != NULL)
        cont = (size_t) atoll(ar[3]);

    if ((n = LeerFichero(ar[1], p, cont)) == -1)
        perror("Imposible leer fichero");
    else
        printf("leidos %lld bytes de %s en %p\n", (long long) n, ar[1], p);
}
void CMD_write(char *param[],int trozos){
    /*
     * write [-o] fiche addr cont : Escribe cont bytes desde la direccion addr a fich (-o sobreescribe)
     */
    void *p;
    size_t cont;
    ssize_t n;

    if(trozos < 4){
        puts("Faltan parametros");
    } else{
        if (strcmp(param[1],"-o") == 0){ // opcion -o (sobreescritura)
            if(trozos == 5){
                p = cadtop(param[3]); // transforma en puntero
                cont = (size_t) atoll(param[4]); // convierte cadena en equivalente numerico
                if ((n = EscribirFichero(param[2], p, cont,1)) == -1){ // intenta escribir en fichero
                    perror("Imposible escribir fichero");
                } else{
                    printf("escritos %lld bytes de %s desde %p\n", (long long) n, param[2], p);
                }
            } else{
                puts("Faltan parametros");
            }
        } else{ // opcion escribir (sin -o)
            if(trozos == 4){
                p = cadtop(param[2]);
                cont = (size_t) atoll(param[3]);
                if ((n = EscribirFichero(param[1], p, cont,0)) == -1){
                    perror("Imposible escribir fichero");
                } else{
                    printf("escritos %lld bytes de %s desde %p\n", (long long) n, param[1], p);
                }
            } else{
                puts("Faltan parametros");
            }
        }
    }
}
void CMD_mem(char *param[],int trozos,tListM *M){
    /*
     * mem [-blocks|-funcs|-vars|-all|-pmap] ..        Muestra muestra detalles de la memoria del proceso
     *          -blocks: los bloques de memoria asignados
     *          -funcs: las direcciones de las funciones
     *          -vars: las direcciones de las variables
     *          :-all: all
     *          -pmap: muestra la salida del comando pmap(o similar)
     */
    int int_l = 2;
    float float_l = 5;
    double double_l = 5.8;
    // locales
    static int int_i = 7;
    static float float_i = 8;
    static double double_i = 2.0;
    // estaticas
    static int int_s;
    static float float_s;
    static double double_s;
    // opciones
    bool blocks = false, pmap = false, funcs = false, vars = false;
    if(trozos < 2){ // caso sin parametros (haria como -all)
        blocks = true;
        funcs = true;
        vars = true;
    } else{
        if(strcmp(param[1],"-blocks") == 0){
            blocks = true;
        } else if(strcmp(param[1],"-funcs") == 0){
            funcs = true;
        } else if(strcmp(param[1],"-vars") == 0){
            vars = true;
        } else if(strcmp(param[1],"-all") == 0){
            blocks = true;
            funcs = true;
            vars = true;
        } else if(strcmp(param[1],"-pmap") == 0){
            pmap = true;
        }
    }
    if(vars){
        printf("Variables locales\t%p,\t%p,\t%p\n", &int_l, &float_l, &double_l);
        printf("Variables globales\t%p,\t%p,\t%p\n", &int_g, &float_g, &double_g);
        printf("Var (N.I) globales\t%p,\t%p,\t%p\n", &int_gn, &float_gn, &double_gn);
        printf("Variables staticas\t%p,\t%p,\t%p\n", &int_i, &float_i, &double_i);
        printf("Var (N.I) staticas\t%p,\t%p,\t%p\n", &int_s, &float_s, &double_s);
    }
    if(funcs){
        printf("Funciones programa\t%p,\t%p,\t%p\n", &CMD_pid, &CMD_deltree, &CMD_open);
        printf("Funciones libreria\t%p,\t%p,\t%p\n", &printf, &malloc, &perror);
    }
    if(blocks){
        mostrarMemListCompleta(*M);
    }
    if(pmap){
        Do_MemPmap();
    }
}
void CMD_recurse(char *param[], int trozos){
    if(trozos > 1)
        Recursiva(atoi(param[1]));
}

void mostrarMemList(tListM M, pid_t pid, cmd tElemento) {
    /*
     * Muestra por pantalla memList
     */
    tPosLM pos;
    tItemLM item;

    printf("******Lista de bloques asignados malloc para el proceso %d\n", pid);
    if (!isEmptyListM(M)) { // recorre la lista y la imprime
        for (pos = firstM(M); pos <= lastM(M); ++pos) {
            item = getItemM(pos, M);
            switch (tElemento) {
                case MALLOC:
                    if (item.cmdType == tElemento) { // filtra en la lista los elementos por tipo (MALLOC, SHARED, MMAP)
                        printf("\t%p\t\t%zu %s malloc\n", item.dir, item.size, item.date);
                    }
                    break;
                case MMAP:
                    if (item.cmdType == tElemento) { // filtra en la lista los elementos por tipo (MALLOC, SHARED, MMAP)
                        printf("\t%p\t\t%zu %s %s (descriptor %d) \n", item.dir, item.size, item.date,
                               item.Union.fich.filename, item.Union.fich.fd);
                    }
                    break;
                case SHARED:
                    if (item.cmdType == tElemento) { // filtra en la lista los elementos por tipo (MALLOC, SHARED, MMAP)
                        printf("\t%p\t\t%zu %s shared (key %d)\n", item.dir, item.size, item.date, item.Union.key);
                    }
                    break;
                default:
                    break;

            }

        }
    }
}
void SharedCreate(char *tr[], pid_t pid, tListM *M, int trozos) { // funcion de ayudap2
    key_t cl;
    size_t tam;
    time_t t = time(0);
    struct tm *tlocal = localtime(&t);
    void *p;
    tItemLM item;


    if (trozos < 4) { // si no hay suficientes paramentros
        mostrarMemList(*M, pid, SHARED);
    } else {
        cl = (key_t) strtoul(tr[2], NULL, 10);
        tam = (size_t) strtoul(tr[3], NULL, 10);
        if (tam == 0) {
            printf("No se asignan bloques de 0 bytes\n");
            return;
        }
        if ((p = ObtenerMemoriaShmget(cl, tam)) != NULL) { // asignar memoria
            item.dir = p;
            item.size = tam;
            item.Union.key = cl;
            item.cmdType = SHARED;
            strftime(item.date, 128, "%b %d %H:%M", tlocal);
            if (insertItemM(item, M)) {
                printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
            } else {
                puts("Error: No se pudo asignar memoria");
            }
        } else {
            printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
        }
    }
}
void *ObtenerMemoriaShmget(key_t clave, size_t tam) { //funcion de ayudap2
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags = flags | IPC_CREAT | IPC_EXCL;
    if (clave == IPC_PRIVATE) { /*no nos vale*/
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);
    if ((p = shmat(id, NULL, 0)) == (void *) -1) {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s);
    return p;
}
void sharedFree(char *tr[], pid_t pid, tListM *M, int trozos) {
    /*
     * Libera la memoria compartida con clave cl
     */
    tItemLM item;
    tPosLM pos;
    key_t key;

    if (trozos < 3) {
        mostrarMemList(*M, pid, SHARED);
    } else {
        key = atoi(tr[2]);
        for (pos = firstM(*M); pos <= lastM(*M); ++pos) {
            item = getItemM(pos, *M);
            if (item.Union.key == key) {
                if ((shmget(item.Union.key, 0, 0)) !=
                    -1) {// intenta obtener información sobre el segmento de memoria compartida
                    if (shmdt(item.dir) == -1) { // desconecta el segmento de memoria compartida
                        printf("No hay bloque de esa clave mapeado en el proceso\n");
                        return;
                    }
                }
                deleteItemM(pos, M);
            }
        }
    }
}
void SharedDelkey(char *args[]) { // funcion de ayudap2
    key_t clave;
    int id;
    char *key = args[2];

    if (key == NULL || (clave = (key_t) strtoul(key, NULL, 10)) == IPC_PRIVATE) {
        printf("      delkey necesita clave_valida\n");
        return;
    }
    if ((id = shmget(clave, 0, 0666)) == -1) {
        perror("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id, IPC_RMID, NULL) == -1)
        perror("shmctl: imposible eliminar id de memoria compartida\n");
}
void *MapearFichero(char *fichero, int protection, tListM *M) { //funcion de ayuda p2
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;
    tItemLM item;
    time_t t = time(0);
    struct tm *tlocal = localtime(&t);

    if (protection & PROT_WRITE)
        modo = O_RDWR;
    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;
    item.dir = p;
    item.size = s.st_size;
    item.cmdType = MMAP;
    item.Union.fich.fd = df;
    strftime(item.date, 128, "%b %d %H:%M", tlocal);
    strcpy(item.Union.fich.filename, fichero);
    insertItemM(item, M);
    return p;
}
ssize_t LeerFichero(char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;
    if (cont == -1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}
void *cadtop(char *string){
    /*
     * Convierte un string en puntero.
     */
    void *p;
    sscanf(string, "%p", &p);
    return p;
}
ssize_t EscribirFichero (char *f, void *p, size_t cont,int overwrite){ //funcion de ayudap2
    ssize_t  n;
    int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags=O_CREAT | O_WRONLY | O_TRUNC;
    if ((df = open(f, flags, 0777)) == -1)
        return -1;
    if ((n = write(df, p, cont)) == -1){
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close (df);
    return n;
}
void Do_MemPmap(void){ // funcion ayudap2
    /*
     * hace el pmap (o equivalente) del proceso actual
     */
    pid_t pid;
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){ /*proceso hijo*/
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid (pid,NULL,0);
}
void mostrarMemListCompleta(tListM M){
    /*
     * Muestra la lista completa
     */
    tPosLM pos;
    tItemLM item;
    pid_t pid = getpid();

    printf("******Lista de bloques asignados malloc para el proceso %d\n", pid);
    if(!isEmptyListM(M)){ // recorre la lista sin ningun filtro
        for (pos = firstM(M); pos <= lastM(M); ++pos) {
            item = getItemM(pos, M);
            if (item.cmdType == MALLOC) {
                printf("\t%p\t\t%zu %s malloc\n", item.dir, item.size, item.date);
            }
            if (item.cmdType == MMAP) {
                printf("\t%p\t\t%zu %s %s (descriptor %d) \n", item.dir, item.size, item.date,
                       item.Union.fich.filename, item.Union.fich.fd);
            }
            if (item.cmdType == SHARED) {
                printf("\t%p\t\t%zu %s shared (key %d)\n", item.dir, item.size, item.date, item.Union.key);
            }
        }
    }
}
void borradoFinal(tListM *M){
    /*
     * Borra las direcciones de memoria reservadas (malloc, shared, mmap) justo antes de que
     * finalice la ejecucion de la shell.
     */
    tPosLM pos;
    tItemLM item;
    if(!isEmptyListM(*M)){
        for (pos = firstM(*M); pos <= lastM(*M); ++pos){
            item = getItemM(pos,*M);
            if(item.cmdType == MALLOC){
                free(item.dir);
            } else if(item.cmdType == SHARED){
                if ((shmget(item.Union.key, 0, 0)) !=
                    -1) {// intenta obtener información sobre el segmento de memoria compartida
                    if (shmdt(item.dir) == -1) { // desconecta el segmento de memoria compartida
                        return;
                    }
                }
            } else if(item.cmdType == MMAP){
                munmap(item.dir, item.size);
            }
        }
    }
}
void LlenarMemoria (void *p, size_t cont, unsigned char byte){
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    printf("Llenando %ld bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",cont, byte, byte, p);

    for (i=0; i<cont;i++)
        arr[i]=byte;
}
void imprimir_char (char c){// Función auxiliar para mostrar caracteres
    bool imprimir = false;
    if (c > 32 && c < 126)
        imprimir = true;
    else
        imprimir = false;

    if(imprimir)
        printf("%4c", c);
    else
        printf("%4c", ' ');

    imprimir = false;
}
void imprimir_hex (char c){ //Función auxiliar para mostrar los caracteres en código hexadecimal
    bool imprimir = false;
    if (c >= 32 && c <= 126)
        imprimir = true;
    else
        imprimir = false;

    if(imprimir)
        printf("%4x", c);
    else
        printf("%3c%c", '0','0');
    imprimir = false;
}
void imprimir_mem (char *p, int nbytes){ //Función auxiliar para poder hacer el comando Mendump
    int i, j;
    int pos_char;
    int veces = 0, lineas = 0;

    for (i = 0; i < nbytes; i++){
        imprimir_char(p[i]);
        veces++;

        if((veces == nbytes - 25 * lineas) || (veces == 25)){
            printf("\n");
            pos_char = i + 1 - veces;

            for (j = pos_char; j < pos_char + veces; ++j){
                imprimir_hex(p[j]);
            }
            printf("\n");
            lineas++;
            veces = 0;
        }
    }
}
void Recursiva (int n){ //Función creada por los profesores para hacer el comando recurse
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

// practica 3
void CMD_uid(char *param[], int trozos){
    /*
     * uid [-get|-set] [-l] [id]: Accede a las credenciales del proceso que ejecuta el shell
     *  -get: muestra las credenciales
     *  -set id: establece la credencial al valor numerico id
     *  -set -l id: establece la credencial a login id
     */
    uid_t uid;

    if(trozos == 1){
        mostrarCredencial();
    } else {
        if(strcmp(param[1],"-set") == 0){ // -set
            if(trozos > 2){
                if(strcmp(param[2],"-l") == 0){ // -set -l
                    if(trozos > 3){
                        if ((uid = obtenerUid(param[3])) == (uid_t) (-1)) {
                            // busca el uid correspondiente al usuario
                            printf("Usuario no existente %s\n", param[3]);
                            return;
                        }
                        if (setuid(uid) == -1){ // cambia uid del proceso
                            perror("Imposible cambiar credencial");
                        }
                    } else mostrarCredencial();
                } else{
                    uid = strtol(param[2],NULL,10); // convierte string en long
                    if(setuid(uid) == -1){ // cambia uid del proceso
                        perror("Imposible cambiar credencial");
                    }
                }
            } else{
                mostrarCredencial();
            }
        } else if(strcmp(param[1],"-get") == 0){ // -get
            mostrarCredencial();
        } else{
            mostrarCredencial();
        }
    }
}
void CMD_showvar(char *param[], int trozos, char * arg3[]){
    /*
     * showvar var:  Muestra el valor y las direcciones de la variable de entorno var
     */
    int i,j;
    char * valor;

    if(trozos == 1){
        imprimirVariable(arg3, "main arg3");
    } else if(trozos == 2){
        valor = getenv(param[1]); // obtiene valor variable de entorno
        if(valor != NULL){
            if ((i = BuscarVariable(param[1], arg3)) == -1) {
                return;
            }
            if ((j = BuscarVariable(param[1], environ)) == -1) {
                return;
            } else {
                printf("Con arg3 main:\t%s (%p) %p\n", arg3[i], arg3[i], &arg3[i]);
                printf("Con environ:\t%s (%p) %p\n", environ[j], environ[j], &environ[j]);
                printf("Con getenv:\t%s (%p)\n", valor, &valor);
            }
        }
    }
}
void CMD_changevar (char *param[], int trozos, char *arg3[], char *env[]){
    /*
     * changevar [-a|-e|-p] var valor  Cambia el valor de una variable de entorno
     *  -a: accede por el tercer arg de main
     *  -e: accede mediante environ
     *  -p: accede mediante putenv
     */
    int i;
    char * var;
    if(trozos < 4){
        puts("Uso: changevar [-a|-e|-p] var valor");
    } else{
        var = malloc(strlen(param[2]) + strlen(param[3]) + 4);
        strcpy(var, param[2]);
        strcat(var, "=");
        strcat(var, param[3]);
        if(strcmp(param[1],"-a") == 0){
            if ((i = BuscarVariable(param[2], arg3)) == -1) {
                perror("Imposible cambiar variable");
                free(var);
                return;
            }
            arg3[i] = var;
        } else if(strcmp(param[1],"-e") == 0){
            if ((i = BuscarVariable(param[2], env)) == -1) {
                perror("Imposible cambiar variable");
                free(var);
                return;
            }
            env[i] = var;
        } else if(strcmp(param[1],"-p") == 0){
            if (putenv(var) != 0) {
                free(var);
                return;
            }
        }
    }
}
void CMD_subsvar(char *param[], int trozos, char *arg3[], char *env[]){
    /*
     * subsvar [-a|-e] var1 var2 valor Sustituye la variable de entorno var1
     *  con var2=valor
     *  -a: accede por el tercer arg de main
     *  -e: accede mediante environ
     */

    if(trozos < 5){
        puts("Uso: subsvar [-a|-e] var valor");
    } else{
        if(strcmp(param[1],"-a") == 0){
            if(CambiarVariable(param[2],param[3],param[4],arg3) == (-1)){
                perror("Imposible cambiar variable");
                return;
            }
        } else if(strcmp(param[1],"-e") == 0){
            if(CambiarVariable(param[2],param[3],param[4],env) == (-1)){
                perror("Imposible cambiar variable");
                return;
            }
        }
    }
}
void CMD_showenv(char *param[], int trozos, char *arg3[], char *env[]){
    /*
     * showenv [-environ|-addr]: Muestra el entorno del proceso
     *  -environ: accede usando environ (en lugar del tercer arg de main)
     *  -addr: muestra el valor y donde se almacenan environ y el 3er arg main
     */

    if (trozos < 2){
        imprimirVariable(arg3, "main arg3");
    } else{
        if (strcmp(param[1],"-environ") == 0){
            imprimirVariable(environ, "environ");
        } else if (strcmp(param[1],"-addr") == 0){
            printf("environ: %p (almacenado en %p)\n",&env[0], &env[0]); //ns si esta bien
            printf("main arg3: %p (almacenado en %p)\n",&arg3[0], &arg3[0]);
        }
    }
}
void CMD_fork(tListP *procList){
    /*
     * fork : El shell hace fork y queda en espera a que su hijo termine
     */
    pid_t pid;

    if ((pid = fork()) == 0) {
        while(!isEmptyListP(*procList)){ // elimina el contenido de la lista
            deleteItemP(firstP(*procList),procList);
        }
        printf ("ejecutando proceso %d\n", getpid());
    } else if (pid != -1){
        waitpid (pid,NULL,0);
    } else{
        perror("Error con fork");
    }
}
void CMD_exec(char *param[], int trozos,bool *isFinish, char *env[]){
    /*
     * exec VAR1 VAR2 ..prog args....[@pri]    Ejecuta, sin crear proceso,prog con argumentos
     *   en un entorno que contiene solo las variables VAR1, VAR2...
     */
    int i, contadorVariables = 0, prioridad = 0;
    char *vars[MAX_SIZE];
    pid_t pid = getpid();

    if(trozos < 2){
        puts("exec VAR1 VAR2 prog args [@pri]");
    } else{
        // buscamos todas las variables de entorno que incluye el usuario
        for(i = 1; i < trozos; ++i){
            if(BuscarVariable(param[i],env) != (-1)){
                vars[i - 1] = param[i];
                ++contadorVariables;
            }
        }
        // obtener valor de prioridad
        if(strstr(param[trozos - 1], "@") != NULL){ // busca que exista en la cadena @
            prioridad = atoi(param[trozos - 1] + 1);
        }
        if(contadorVariables > 0){ // ejecutar con variables determinadas
            if(setpriority(PRIO_PROCESS,pid,prioridad) == (-1)){ // establece prioridad proceso
                return;
            }
            if(execvpe(param[contadorVariables + 1], param + contadorVariables + 1, (i == 0) ? environ : vars) == (-1)){
                puts("No se pudo ejecutar");
            }
            *isFinish = true;
        } else{
            if(setpriority(PRIO_PROCESS,pid,prioridad) == (-1)){ // establece prioridad proceso
                return;
            }
            if(execvp(param[contadorVariables + 1], &param[contadorVariables + 1]) == (-1)){
                puts("No se pudo ejecutar");
            }
            *isFinish = true;
        }
    }
}
void CMD_jobs(tListP *procList){
    /*
     * jobs: Lista los procesos en segundo plano
     */
    int pos;
    tItemLP item;

    if(!isEmptyListP(*procList)){
        for(pos = firstP(*procList); pos <= lastP(*procList); ++pos){
            item = getItemP(pos,*procList);
            item = actualizarEstado(item,0);
            imprimirProceso(item);
        }
    }
}
void CMD_deljobs(char *param[], int trozos, tListP *procList){
    /*
     * deljobs [-term][-sig]   Elimina los procesos de la lista procesos en sp
     *  -term: los terminados
     *  -sig: los terminados por senal
     */
    int i;

    if(trozos >= 2){
        for(i = 1; i < trozos; ++i){
            if(strcmp(param[i],"-term") == 0){ // borrar procesos terminados
                borrarProcesos(FINISHED,procList);
            } else if(strcmp(param[i],"-sig") == 0){ // borrar procesos terminados por señal
                borrarProcesos(SIGNALED,procList);
            }
        }

    } else{
        CMD_jobs(procList);
    }
}
void CMD_job(char *param[], int trozos, tListP *procList){
    /*
     * job [-fg] pid   Muestra informacion del proceso pid.
     *          -fg: lo pasa a primer plano
     */
    int pos = firstP(*procList), pidBuscado;
    bool encontrado = false;
    tItemLP item;
    item.pid = (-1);

    if(trozos == 1){
        CMD_jobs(procList);
    } else if (trozos == 2){
        pidBuscado = atoi(param[1]);
        while(item.pid != pidBuscado && pos <= lastP(*procList)){
            item = getItemP(pos,*procList);
            if (item.pid == pidBuscado) encontrado = true;
        }
        if(encontrado){
            item = actualizarEstado(item,1);
            imprimirProceso(item);
        } else{
            puts("Proceso no encontrado");
        }
    } else if (trozos == 3 && strcmp(param[1],"-fg") == 0){
        pidBuscado = atoi(param[2]);
        while(item.pid != pidBuscado && pos <= lastP(*procList)){
            item = getItemP(pos,*procList);
            if (item.pid == pidBuscado) encontrado = true;
        }
        if(encontrado){
            deleteItemP(pos,procList);
            waitpid(item.pid,NULL,0);
            printf("Proceso %d terminado normalmente\n",item.pid);
        } else{
            puts("Proceso no encontrado");
        }
    }
}
void CMD_other(char *param[], int trozos, tListP *procList, char *env[]){
    bool primerPlano;
    char *vars[MAX_SIZE];
    int i, numVariables = 0;

    if (trozos != 0) {
        // comprobar si se tiene que ejecutar en primer o segundo plano
        if (strstr(param[trozos - 1], "&") == NULL) primerPlano = true;
        else {
            primerPlano = false;
            param[trozos - 1] = NULL;
        }
        // comprobar si hay variables
        for (i = 0; BuscarVariable(param[i], env) != (-1); ++i) {
            ++numVariables;
            vars[i] = param[i];
        }
        // ejecutar programa externo
        if (primerPlano) { // ejecutamos en primer plano
            exPrimerPlano(param, trozos, numVariables, vars, i);
        } else { // ejecutamos en segundo plano
            exSegPlano(param, trozos, numVariables, vars, i, procList);
        }
    } else{
        puts("No ejecutado");
    }

}

uid_t obtenerUid(char *nombre){
    /*
     * Obtiene el uid correspondiente al nombre de usuario
     */
    struct passwd *passwd;

    if ((passwd = getpwnam(nombre)) == NULL) // obtiene informacion del usuario
        return (uid_t) -1;
    return passwd->pw_uid;
}
char * user(uid_t uid){
    /*
     * Obtiene el nombre de usuario correspondiente al uid
     */
    struct passwd *passwd;

    if ((passwd = getpwuid(uid)) == NULL) // obtiene los datos del usuario (si existe)
        return ("UKNOWN");
    return passwd->pw_name;
}
void mostrarCredencial(){
    uid_t real = getuid(), efec = geteuid();
    printf ("Credencial real: %d, (%s)\n", real, user(real));
    printf ("Credencial efectiva: %d, (%s)\n", efec, user(efec));
}
int BuscarVariable (char * var, char *e[]){ // funcion creada por los profesores
    /*
     * Busca una variable en el entorno que se le pasa como parámetro
     */
    int pos = 0;
    char aux[MAXVAR];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL)
        if (!strncmp(e[pos], aux, strlen(aux)))
            return (pos);
        else
            pos++;
    errno = ENOENT;
    return(-1);
}
void imprimirVariable(char *env[], char *name){
    /*
     * Imprime información sobre una variable de entorno
     */
    int i;

    for(i = 0; env[i] != NULL; ++i){
        printf("%p->%s[%d]=(%p) %s\n", &env[i], name, i, env[i], env[i]);
    }
}
int CambiarVariable(char * var, char *nuevoVar, char *valor,  char *e[]){ // funcion creada por los profesores
    /*
     * cambia una variable en el entorno que se le pasa como parámetro
     * lo hace directamente, no usa putenv
     */
    int pos;
    char *aux;
    size_t longitud;

    if ((pos=BuscarVariable(var,e))==-1) // busca la variable
        return(-1);
    longitud = strlen(nuevoVar) + 1 + strlen(valor) + 1;
    if ((aux = (char *) malloc(longitud)) == NULL) // reserva memoria
        return -1;
    snprintf(aux, longitud, "%s=%s", nuevoVar, valor); // combina nuevo valor
    e[pos]=aux;

    return (pos);
}
void exPrimerPlano(char *param[], int trozos, int numVariables, char *vars[], int i){
    /*
     * Ejecuta el programa en primer plano
     */
    pid_t pid;
    int prioridad = 0;

    // obtenemos prioridad
    if(strstr(param[trozos - 1], "@") != NULL){ // busca que exista @
        prioridad = atoi(param[trozos - 1] + 1);
    }
    // creamos proceso hijo y ejecutamos
    if ((pid = fork()) == 0) {
        if(setpriority(PRIO_PROCESS,pid,prioridad) == (-1)){ // establece prioridad proceso
            return;
        }
        if(numVariables > 0){ //ejecutar con variables de entorno
            if(execvpe(param[numVariables], param + numVariables + 1, (i == 0) ? environ : vars) == (-1)){
                puts("No se pudo ejecutar");
                exit(1);
            }
        } else{ // ejecutar sin variables de entorno
            if(execvp(param[numVariables], param) == (-1)){
                puts("No se pudo ejecutar");
                exit(1);
            }
        }
    } else if (pid != -1){
        waitpid (pid,NULL,0);
    } else{
        perror("No se pudo ejecutar");
    }
}
void exSegPlano(char *param[], int trozos, int numVariables, char *vars[], int i, tListP *procList){
    /*
     * Ejecuta el programa en segundo plano
     */
    pid_t pid;
    int prioridad = 0, j, w, posString = 0;
    uid_t uid = geteuid();
    time_t tm;
    char date[MAX_SIZE], cmd[MAX_SIZE], temp[MAX_SIZE];
    tItemLP item;
    struct passwd *passwd;

    // ejecutamos proceso ( se ejecutan al mismo tiempo padre e hijo)
    if ((pid = fork()) == 0) {
        if(setpriority(PRIO_PROCESS,pid,prioridad) == (-1)){ // establece prioridad proceso
            return;
        }
        if(numVariables > 0){ //ejecutar con variables de entorno
            if(execvpe(param[numVariables], param + numVariables + 1, (i == 0) ? environ : vars) == (-1)){
                puts("No se pudo ejecutar");
                exit(1);
            }
        } else{ // ejecutar sin variables de entorno
            if(execvp(param[numVariables], param) == (-1)){
                puts("No se pudo ejecutar");
                exit(1);
            }
        }
    }
    // añadimos a la lista la información del proceso
    passwd = getpwuid(uid);
    tm = time(0);
    strftime(date, 256, "%d/%m/%y %H:%M:%S", localtime(&tm));
    item.pid = pid;
    item.estado = ACTIVE;
    strcpy(item.user,passwd->pw_name);
    for(j = 0; j < trozos - 1; ++j){
        strcpy(temp,param[j]);
        for(w = 0; temp[w] != '\0'; ++w){
            cmd[posString] = temp[w];
            ++posString;
        }
        cmd[posString] = ' ';
        ++posString;
    }
    cmd[posString] = '\0';
    strcpy(item.cmd,cmd);
    strcpy(item.date,date);
    item.priority = prioridad;
    insertItemP(item,procList);
}
void borrarProcesos(status estado, tListP *procList){
    /*
     * Borra los procesos de la lista con el estado x
     */
    tItemLP item;
    int i;

    for (i = firstP(*procList); i <= lastP(*procList) && !isEmptyListP(*procList); ++i){
        item = getItemP(i,*procList);
        if(item.estado == estado){
            deleteItemP(i,procList);
            --i;
        }
    }

}
void imprimirProceso(tItemLP item){
    char *estado = malloc(20);

    switch (item.estado) {
        case FINISHED:
            strcpy(estado,"FINISHED");
            break;
        case STOPPED:
            strcpy(estado,"STOPPED");
            break;
        case ACTIVE:
            strcpy(estado,"ACTIVE");
            break;
        case SIGNALED:
            strcpy(estado,"SIGNALED");
            break;
    }
    printf("%d\t%s p=%d %s %s (%03d) %s\n", item.pid, item.user,
           getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.signal, item.cmd);
}
tItemLP actualizarEstado(tItemLP item, int opcion){
    int estado;

    opcion = WNOHANG | WUNTRACED | WCONTINUED;

    if(waitpid(item.pid, &estado, opcion) == item.pid){
        if(WIFEXITED(estado)){
            item.estado = FINISHED;
            item.signal = WEXITSTATUS(estado);
        } else if (WIFCONTINUED(estado)){
            item.estado = ACTIVE;
        } else if (WIFSTOPPED(estado)){
            item.estado = STOPPED;
            item.signal = WTERMSIG(estado);
        } else if (WIFSIGNALED(estado)){
            item.estado = SIGNALED;
            item.signal = WTERMSIG(estado);
        }
    }
    return item;
}
