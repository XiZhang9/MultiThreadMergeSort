CC := gcc
CCOPTS := -O


Mergesort : Mergesort.o
	$(CC) -o Mergesort Mergesort.o
# how to compile .c files
Mergesort.o: Mergesort.c Mergesort.h
	$(CC) $(CCOPTS) -c -o Mergesort.o Mergesort.c
clean:
	rm -rf Mergesort.o Mergesort
