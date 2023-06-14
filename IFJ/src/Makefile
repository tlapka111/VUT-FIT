CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -O3 -g

PARTS = str.o scanner.o main.o parser.o psa_stack.o symtable.o stack.o instr.o codegen.o

all:	build clear

test:	test_script.sh
	bash test_script.sh

build:	$(PARTS)
	$(CC) $(CFLAGS) $(PARTS) -o IFJ
		
main.o:	scanner.o parser.o symtable.o main.c 
	$(CC) $(CFLAGS) -c main.c

str.o:	str.c str.h
	$(CC) $(CFLAGS) -c str.c

psa_stack.o: psa_stack.c psa_stack.h
	$(CC) $(CFLAGS) -c psa_stack.c

scanner.o: scanner.c scanner.h str.o
	$(CC) $(CFLAGS) -c scanner.c

parser.o: scanner.o str.o psa_stack.o symtable.o parser.c parser.h codegen.o
	$(CC) $(CFLAGS) -c parser.c

symtable.o: symtable.h symtable.c stack.o
	$(CC) $(CFLAGS) -c symtable.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c			

instr.o: instr.c instr.h
	$(CC) $(CFLAGS) -c instr.c

codegen.o: codegen.h codegen.c instr.o 
	$(CC) $(CFLAGS) -c codegen.c


clear: 
	rm -f *.o
