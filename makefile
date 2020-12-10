all: main.exe

main.exe: main.o
	gcc -o main.exe main.o

main.o: main.c
	gcc -c main.c

flash.o: flash.c
	gcc -c flash.c
     
logger.o: logger.c
	gcc -c logger.c
     
unittest.o: unittest.c
	gcc -c unittest.c

clean:
	rm main.o main.exe
	rm flash.o flash.exe
	rm logger.o logger.exe
	rm unittest.o unittest.exe