all: mymalloc.c mymalloc.h driver.c
	gcc -w -m32 -o malloc driver.c mymalloc.c
