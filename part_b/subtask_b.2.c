#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 10000
#define CYCLES 100
char child_stack[STACK_SIZE+1];

void print(const char * proccess)
{
    int counter = 0;

    while(counter < CYCLES)
    {
        printf("Hello from %s #%d\n", proccess, counter);
        sleep(1);
        counter++;
    }
}


int create_new_child_thread(void * param)
{
    print((char*)param);
}

int main()
{
    int first_cloning_result = clone(create_new_child_thread, child_stack+STACK_SIZE, CLONE_PARENT, "first child");
    int second_cloning_result = clone(create_new_child_thread, child_stack+STACK_SIZE, CLONE_PARENT, "second child");

    print("parent");

    return 0;
}