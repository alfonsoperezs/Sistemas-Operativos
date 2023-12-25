all: list.o proc_list.o mem_list.o p3.c
	gcc $(CFLAGS) -o p3 p3.c list.o mem_list.o proc_list.o

p3.o: p3.c p3.h
	gcc $(CFLAGS) -c p3.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

proc_list.o: proc_list.c proc_list.h
	gcc $(CFLAGS) -c proc_list.c

mem_list.o: mem_list.c mem_list.h
	gcc $(CFLAGS) -c mem_list.c

valgrind: all
	valgrind -v --tool=memcheck --leak-check=full ./p3
