############################################################
# Makefile                                                 #
# automatizovaný překlad všech zdrojových soborů           #
# make        - překlad programu                           #
# make test   - spustí automatizované testy                #
# make clean  - odstraní spustitelné a binární soubory     #
#               .diff, .output, .ifj17 soubory po testech  #
#                                                          #
# Martin Stodůlka(xstodu08)                                #
# Ondřej Olšák(xolsak00)                                   #
# Michael Schneider(xschne07)                              #
# Marek Kuchynka(xkuchy00)                                 #
############################################################

CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror

ifj17: ifj17.o code_gen.o dl_list.o scanner.o stack_operations.o symtable.o syntax_analyzer.o
	$(CC) $(CFLAGS) ifj17.o code_gen.o dl_list.o scanner.o stack_operations.o symtable.o syntax_analyzer.o -o $@


ifj17.o: main.c code_gen.h syntax_analyzer.h
	$(CC) $(CFLAGS) -o $@ -c main.c

code_gen.o: code_gen.c code_gen.h syntax_analyzer.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c code_gen.c

dl_list.o: dl_list.c dl_list.h syntax_analyzer.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c dl_list.c

scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -o $@ -c scanner.c

stack_operations.o: stack_operations.c stack_operations.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c stack_operations.c

symtable.o: symtable.c symtable.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c symtable.c

syntax_analyzer.o: syntax_analyzer.c syntax_analyzer.h scanner.h symtable.h dl_list.h stack_operations.h code_gen.h
	$(CC) $(CFLAGS) -o $@ -c syntax_analyzer.c


test: ifj17
	bash tests/auto_test.sh


clean:
	-rm -f *.o
	-rm -f ifj17
	-rm -f tests/correct/*.output
	-rm -f tests/correct/*.ifjcode17
	-rm -f tests/correct/*.diff
	-rm -f tests/incorrect/*.ifjcode17
