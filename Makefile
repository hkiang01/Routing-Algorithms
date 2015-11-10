# 
# Makefile for ECE/CS 438 mp3
#
EXE = distvec
EXE2 = linkstate
OBJS = main.o node.o link.o routetableentry.o
OBJS2 = main2.o node.o link.o routetableentry.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror
LINKER = g++
# LINKER_OPTS = -lpng

all : $(EXE) $(EXE2)

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) $(LINKER_OPTS) -o $(EXE)

main.o : main.cpp node.h link.h routetableentry.h
	$(COMPILER) $(COMPILER_OPTS) main.cpp

node.o : node.cpp link.h routetableentry.h
	$(COMPILER) $(COMPILER_OPTS) node.cpp

link.o : link.cpp
	$(COMPILER) $(COMPILER_OPTS) link.cpp

routetableentry.o : routetableentry.cpp
	$(COMPILER) $(COMPILER_OPTS) routetableentry.cpp

$(EXE2) : $(OBJS)
	$(LINKER) $(OBJS) $(LINKER_OPTS) -o $(EXE2)

clean :
	-rm -f *.o $(EXE)
