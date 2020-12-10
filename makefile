CC = gcc
CFLAGS  = -g -Wall

default: main.exe

# To create the executable file main.exe we need object files flash.o, logger.o, and unittest.o

main: flash.o logger.o unittest.o 
	$(CC) $(CFLAGS) -o main flash.o logger.o unittest.o

# To create the object file flash.o we need the source files flash.c and configuration.h

flash.o: flash.c configuration.h 
	$(CC) $(CFLAGS) -c flash.c

# To create the object file logger.o we need the source files logger.c, flash.h and configuration.h

logger.o: logger.c flash.h, configuration.h
	$(CC) $(CFLAGS) -c logger.c

# To create the object file unittest.o we need the source files logger.h and configuration.h

unittest.o:  unittest.c logger.h configuration.h
	$(CC) $(CFLAGS) -c unittest.c

# To start over from scratch, type 'make clean'.
# This removes the executable file, old .o object files and *~ backup files.

clean: 
	$(RM) count *.o *~