#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CYCLES 100

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

// void new_process(const char * file_name)
// {
//     __pid_t pid = fork();
//     printf("new child process pid: %d\n", pid);
//     if(pid == 0)
//     {
//         char * args[2] = {file_name ,NULL};
//         execvp(args[0],args);
//     }

// }


int main()
{
    // char * files = {"./subtask_b.1", "./subtask_b.2", "./subtask_b.3"};

    // int i;
    // for ( i = 0 ; i < 3 ; i++)
    // {
    //     new_process(files[i]);
    // }

    // creating new proccess - forking from parent and lounching subtask_b.1
    __pid_t pid = fork();
    printf("new child process pid: %d\n", pid);
    if(pid == 0)
    {
        char * args[2] = {"./subtask_b.1",NULL};
        execvp(args[0],args);
    }

    // creating new proccess - forking from parent and lounching subtask_b.2

    pid = fork();
    printf("new child process pid: %d\n", pid);
    if(pid == 0)
    {
        char * args[2] = {"./subtask_b.2",NULL};
        execvp(args[0],args);
    }

    // creating new proccess - forking from parent and lounching subtask_b.3

    pid = fork();
    printf("new child process pid: %d\n", pid);
    if(pid == 0)
    {
        char * args[2] = {"./subtask_b.3",NULL};
        execvp(args[0],args);
    }

    else
    {
        print("hello from first process");
    }
    

    return 0;
}