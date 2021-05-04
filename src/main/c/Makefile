# ##########
# File: Makefile
# Author: borjagilperez at github.com
# ##########

# The GNU compiler collection
CC = gcc
# -O to apply to code optimizer, -g to be able to debug, -Wall: extra warnings
CFLAGS = -O -g -Wall
# -lm link the math library
LIBS = -lm

# $ make
# $ make all
all: review processes semaphores

P0 = programming
# $ make review
review: $(P0)

P1S1 = intro zombie orphan clone
P1S2 = sequential parallel generations
P1 = $(P1S1) $(P1S2)
# $ make processes
processes: $(P1)

P2S1 = alternate alternate2
P2S2 = gensem
P2 = $(P2S1) $(P2S2)
# $ make semaphores
semaphores: $(P2)


# A phony target is one that is not the name of a file,
# but a descriptive name of some action,
# of some orders that will be executed when they are explicity required.
UTILS = info astyle valgrind doxygen clean
.PHONY: $(UTILS)

# ##########
# Utilities
# ##########
# $ make info
info:
	@echo "Author: borjagilperez at github.com"
	@echo "Executable programs."
	@echo "\tP0, pointers: $(P0)"
	@echo "\tP1, processes: $(P1)"
	@echo "\tP2, semaphores: $(P2)"
	@echo "Utilities."
	@echo "\t$(UTILS)"
# $ make essential
essential:
	sudo apt-get install -y astyle build-essential nemiver nmon terminator valgrind
	sudo apt-get install -y doxygen graphviz
	sudo update-alternatives --config x-terminal-emulator
# $ make astyle
astyle:
	astyle *.h *.c
	@echo "Run make clean to remove *.orig files if any file was formated."
# $ make valgrind ARGS="./<executable> [<arguments>]"
valgrind:
	valgrind --leak-check=full ${ARGS}
# $ make doxygen
doxygen:
	rm -fr Doxyfile html latex
	doxygen -g
	doxygen Doxyfile
# $ make clean
clean:
	rm -fr Doxyfile html latex
	rm -f *~ *.o *.orig core $(P0) $(P1) $(P2)


# $@ is the item that appears to the left of ':'
# $< is the first item in the list of dependencies
# $^ are all the files to the right of ':'

# ##########
# P0
# ##########
programming.o: programming.c const.h pointers.h
	$(CC) -c -o $@ $< $(CFLAGS)
pointers.o: pointers.c const.h pointers.h
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make programming
programming: programming.o pointers.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# ##########
# P1S1
# ##########
intro.o: intro.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make intro
intro: intro.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
zombie.o: zombie.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make zombie
zombie: zombie.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
orphan.o: orphan.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make orphan
orphan: orphan.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
clone.o: clone.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make clone
clone: clone.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# ##########
# P1S2
# ##########
sequential.o: sequential.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make sequential
sequential: sequential.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
parallel.o: parallel.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make parallel
parallel: parallel.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
generations.o: generations.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make generations
generations: generations.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# ##########
# P2S1
# ##########
alternate.o: alternate.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make alternate
alternate: alternate.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
alternate2.o: alternate2.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make alternate2
alternate2: alternate2.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
# ##########
# P2S2
# ##########
gensem.o: gensem.c
	$(CC) -c -o $@ $< $(CFLAGS)
# $ make gensem
gensem: gensem.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
