CC = gcc
CFLAGS  = -g -Wall
MAIN = main.exe

default: $(MAIN)

# Executable file requires object files flash.o, logger.o, unittest.o

$(MAIN): main.o unittest.o logger.o flash.o
	$(CC) $(CFLAGS) -o $@ $^

# Object file main.o requires source files main.c, logger.h, unittest.h

main.o: main.c logger.h unittest.h
	$(CC) $(CFLAGS) -c main.c

# Object file flash.o requires source files flash.c, configuration.h

flash.o: flash.c configuration.h
	$(CC) $(CFLAGS) -c flash.c

# Object file logger.o requires source files logger.c, flash.h, configuration.h

logger.o: logger.c flash.h configuration.h
	$(CC) $(CFLAGS) -c logger.c

# Object file unittest.o requires source files logger.h, configuration.h

unittest.o:  unittest.c logger.h configuration.h
	$(CC) $(CFLAGS) -c unittest.c

# Type 'make clean' to remove the executable file, old .o object files and *~ backup files

clean: 
	$(RM) -v $(MAIN) *.o *~
