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

int main()
{
    // creating new proccess - forking from parent and launching subtask_b.1
    __pid_t pid = fork();
    if(pid == 0)
    {
        char * args[2] = {"./subtask_b.1",NULL};
        execvp(args[0],args);
    }

    else
        {
        // creating new proccess - forking from parent and launching subtask_b.2

        pid = fork();
        if(pid == 0)
        {
            char * args[2] = {"./subtask_b.2",NULL};
            execvp(args[0],args);
        }
        else
        {
            // creating new proccess - forking from parent and launching subtask_b.3

            pid = fork();
            if(pid == 0)
            {
                char * args[2] = {"./subtask_b.3",NULL};
                execvp(args[0],args);
            }
            else
            {
                {
                    print("hello from first process");
                }
            }
            
        }
    
    }
    

    return 0;
}