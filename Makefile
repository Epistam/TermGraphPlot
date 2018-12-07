# Generic vars & flags
CC=gcc
CFLAGS=-Wall -O2 #-fdiagnostics-color=always gcc >=4.9
LDFLAGS=-lm
EXEC=TermGraphPlot
SRC = $(filter-out test.c, $(wildcard *.c))
#SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

# Color management (--> built-in color support in GCC 4.9...
CLR_RS=$(\e[0m)
CLR_RED=$(\e[1;33m)
CLR_OUT=$(| sed -e "s/warning/$(CLR_RED)warning$(CLR_RS)/g")

COLORIZE=$(|& while read x ; do echo $x ; done \
| sed -e "s/.*error:.*/\x1b[1;36m&\x1b[0m/" \
-e "s/.*warning:.*/\x1b[1;36m&\x1b[0m/" \
-e "s/^\(.*\)\(required from\)/\x1b[1;36m\1\x1b[0mnote: \2/" \
-e "s/^\(.*\)\(In instantiation of\)/\x1b[1;36m\1\x1b[0mnote: \2/" \
-e "s/^\(.*\)\(In member\)/\x1b[1;36m\1\x1b[0mnote: \2/" \
| sed -e "s/error:/\x1b[1;31m&\x1b[1;36m/" \
-e "s/warning:/\x1b[1;35m&\x1b[1;36m/" \
-e "s/note:/\x1b[1;30m&\x1b[0m/")

 # Main exec rule (libgraphique in a different dir thus left out by wildcard)
p4: $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

# Header check

# General compiling rule
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) 

# Cleaning stuff
clean: 
	rm -f *.o
mrproper: clean
	rm -f $(EXEC)
