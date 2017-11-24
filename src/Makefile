############################################################
# Makefile                                       					 #
# automatizovaný překlad všech zdrojových soborů 					 #
# make        - překlad programu          								 #
# make clean  - odstraní spustitelné a binární soubory     #
############################################################

CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror

ifj17: ifj17.o code_gen.o DLlist.o scanner.o stack_operations.o symtable.o syntax_analyzer.o
	$(CC) $(CFLAGS) ifj17.o code_gen.o DLlist.o scanner.o stack_operations.o symtable.o syntax_analyzer.o -o $@


ifj17.o: main.c code_gen.h syntax_analyzer.h
	$(CC) $(CFLAGS) -o $@ -c main.c

code_gen.o: code_gen.c code_gen.h syntax_analyzer.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c code_gen.c

DLlist.o: DLlist.c DLlist.h syntax_analyzer.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c DLlist.c

scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -o $@ -c scanner.c

stack_operations.o: stack_operations.c stack_operations.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c stack_operations.c

symtable.o: symtable.c symtable.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c symtable.c

syntax_analyzer.o: syntax_analyzer.c syntax_analyzer.h scanner.h symtable.h DLlist.h stack_operations.h code_gen.h
	$(CC) $(CFLAGS) -o $@ -c syntax_analyzer.c

clean:
	rm *.o
	rm ifj17
