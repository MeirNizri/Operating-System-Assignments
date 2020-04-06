all: 
	gcc *.c
	./a.out

clean:
	rm -f *.o a.out