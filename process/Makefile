all:
	gcc -g -o atexit atexit.c ../include/ourhdr.c -I ../include
	gcc -g -o jmp jmp.c ../include/ourhdr.c -I ../include
	gcc -g -o rlimit rlimit.c ../include/ourhdr.c -I ../include
	gcc -g -o nozfork nozfork.c  ../include/ourhdr.c ../include/csapp.c -I ../include -lpthread
	gcc -g -o timeptr timeptr.c  ../include/csapp.c -I ../include -lpthread
