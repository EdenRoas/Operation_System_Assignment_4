
# Compilation of all files must be enabled using the command 'Make all'
# Make clean must delete all craft


CC = gcc
AR = ar
FLAGS = -Wall -g

run: main
	./main

all: main

main: main.o
	$(CC) -pthread -o main main.c $(FLAGS)

.PHONY: clean all

#clean all files
clean:
	rm -f *.o *.so main