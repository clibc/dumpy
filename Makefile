CFLAGS=-ggdb -Wall -Wextra -pedantic -std=c99

main:main.c
	gcc $(CFLAGS) main.c

clean:
	rm -f *.o
	rm -f *.out
	rm -f *.core