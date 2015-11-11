# 
# Makefile for ECE/CS 438 mp3
#
EXE1 = linkstate
EXE2 = distvec

# may want to include main.o in OBJS and OBJS2
OBJS = linkstate.o node.o link.o graph.o routetableentry.o file_core.o
OBJS2 = distvec.o node.o link.o graph.o routetableentry.o file_core.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror
LINKER = g++
# LINKER_OPTS = -lpng

all : $(EXE1) $(EXE2)

$(EXE1) : $(OBJS)
	$(LINKER) $(OBJS) $(LINKER_OPTS) -o $(EXE1)

$(EXE2) : $(OBJS2)
	$(LINKER) $(OBJS2) $(LINKER_OPTS) -o $(EXE2)

main.o : main.cpp node.h link.h routetableentry.h
	$(COMPILER) $(COMPILER_OPTS) main.cpp

linkstate.o : linkstate.cpp node.h link.h routetableentry.h
	$(COMPILER) $(COMPILER_OPTS) linkstate.cpp

distvec.o : distvec.cpp node.h link.h routetableentry.h
	$(COMPILER) $(COMPILER_OPTS) distvec.cpp

node.o : node.cpp link.h routetableentry.h
	$(COMPILER) $(COMPILER_OPTS) node.cpp

link.o : link.cpp
	$(COMPILER) $(COMPILER_OPTS) link.cpp

graph.o : graph.cpp
	$(COMPILER) $(COMPILER_OPTS) graph.cpp

routetableentry.o : routetableentry.cpp
	$(COMPILER) $(COMPILER_OPTS) routetableentry.cpp

file_core.o : file_core.cpp
	$(COMPILER) $(COMPILER_OPTS) file_core.cpp

clean :
	-rm -f *.o $(EXE1) $(EXE2)
