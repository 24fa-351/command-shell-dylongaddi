# Makefile for shell (Your Custom Shell)

CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = main.o parser.o execute.o builtins.o env.o
TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c parser.h execute.h builtins.h env.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

execute.o: execute.c execute.h parser.h builtins.h env.h
	$(CC) $(CFLAGS) -c execute.c

builtins.o: builtins.c builtins.h parser.h
	$(CC) $(CFLAGS) -c builtins.c

env.o: env.c env.h builtins.h parser.h
	$(CC) $(CFLAGS) -c env.c

clean:
	rm -f $(OBJ) $(TARGET)
