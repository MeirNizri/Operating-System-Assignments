all: pingPong

pingPong: pingPong.o
	gcc pingPong.o -o pingPong
	
pingPong.o:
	gcc -c pingPong.c
	
clean:
	rm -f *.o pingPong
