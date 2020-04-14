all: subtask_b.4

subtask_b.1: subtask_b.1.o
	gcc subtask_b.1.o -o subtask_b.1

subtask_b.2: subtask_b.2.o
	gcc subtask_b.2.o -o subtask_b.2

subtask_b.3: subtask_b.3.o
	gcc subtask_b.3.o -o subtask_b.3

subtask_b.4: subtask_b.4.o subtask_b.1 subtask_b.2 subtask_b.3
	gcc subtask_b.4.o -o subtask_b.4



subtask_b.1.o: 
	gcc -c subtask_b.1.c

subtask_b.2.o:
	gcc -c subtask_b.2.c

subtask_b.3.o:
	gcc -c subtask_b.3.c

subtask_b.4.o:
	gcc -c subtask_b.4.c


clean:
	rm -f *.o subtask_b.1 subtask_b.2 subtask_b.3 subtask_b.4

