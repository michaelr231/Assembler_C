assembler: main.o data.o functions.o secondAssemblerRun.o binary.o
	gcc -g -ansi -Wall main.o data.o functions.o secondAssemblerRun.o binary.o -pedantic -o assembler

main.o: main.c main.h defines.h binary.h data.h
	gcc -c  -g -ansi main.c  -Wall -pedantic -o main.o

data.o: data.c defines.h data.h
	gcc -c -g -ansi data.c -Wall -pedantic -o data.o

functions.o: functions.c defines.h data.h
	gcc -c -g -ansi functions.c -Wall -pedantic -o functions.o

secondAssemblerRun.o: secondAssemblerRun.c defines.h binary.h data.h
	gcc -c -g -ansi secondAssemblerRun.c -Wall -pedantic -o secondAssemblerRun.o

binary.o: binary.c defines.h binary.h data.h
	gcc -c -g -ansi binary.c -Wall -pedantic -o binary.o

clean: main.o data.o functions.o secondAssemblerRun.o binary.o
