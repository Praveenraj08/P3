all:
	gcc -c -fpic bitmem.c -Wall -Werror
	gcc -shared -o libmem.so bitmem.o
	gcc -L/home/ghostrecon/p3 -Wl,-rpath=/home/ghostrecon/p3 -Wall -o test test.c -lmem

clean:
	rm bitmem.o libmem.so test
