.PHONY: all
all: subtaskA_1 subtaskA_2 subtaskA_3 subtaskA_4

subtaskA_1: hello_Ariel.o main1_1.o
	gcc hello_Ariel.o main1_1.o -o Hello1
	./Hello1

subtaskA_2: main1_2.o lib
	gcc main1_2.c -L. -l Hello -o Hello2
	./Hello2

subtaskA_3: lib main1_3.o
	gcc main1_3.o -fPIC -ldl -o Hello3
	./Hello3

subtaskA_4: lib Hello2
	echo -n "" > Tools_Output.txt
	echo "Information about libHello.so" >> Tools_Output.txt
	file libHello.so >> Tools_Output.txt
	echo "\n\nAll functions in libHello.so" >> Tools_Output.txt
	nm libHello.so >> Tools_Output.txt
	echo "\n\nAll library calls in hello2" >> Tools_Output.txt
	ltrace ./Hello2 >> Tools_Output.txt 2>&1
	echo "\n\nAll system calls in hello2" >> Tools_Output.txt
	strace ./Hello2 >> Tools_Output.txt 2>&1

hello_Ariel.o: hello_Ariel.c hello_Ariel.h
	gcc -c hello_Ariel.c
	
main1_1.o: hello_Ariel.h
	gcc -c main1_1.c
	
main1_2.o:
	gcc -c main1_2.c
	
main1_3.o:
	gcc -c main1_3.c	
	
lib: hello_Ariel.c
	gcc -o libHello.so -shared -fPIC hello_Ariel.c

.PHONY: clean
clean:
	rm -f *.o Hello1 Hello2 Hello3 libHello.so