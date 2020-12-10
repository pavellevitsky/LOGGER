CC = gcc
CFLAGS  = -g -Wall

default: main.exe

# Executable file main.exe requires object files flash.o, logger.o, and unittest.o

main.exe: unittest.o logger.o flash.o main.o
	$(CC) $(CFLAGS) -o main.o flash.o logger.o unittest.o

# Object file main.o requires source files main.c, logger.h and unittest.h

main.o: main.c logger.h unittest.h
	$(CC) $(CFLAGS) -c main.c

# Object file flash.o requires source files flash.c and configuration.h

flash.o: flash.c configuration.h
	$(CC) $(CFLAGS) -c flash.c

# Object file logger.o requires source files logger.c, flash.h and configuration.h

logger.o: logger.c flash.h configuration.h
	$(CC) $(CFLAGS) -c logger.c

# Object file unittest.o requires source files logger.h and configuration.h

unittest.o:  unittest.c logger.h configuration.h
	$(CC) $(CFLAGS) -c unittest.c

# To start over from scratch, type 'make clean'.
# This removes the executable file, old .o object files and *~ backup files.

clean: 
	$(RM) count *.o *~
