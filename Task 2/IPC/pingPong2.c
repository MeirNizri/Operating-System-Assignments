#include <stdio.h>

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

static int signalCounter = 0;

void readUsual(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("child received SIGUSR1 #%d.\n", ++signalCounter ); 
    }
}

int main()
{
    //SIGUSR1 signal handler
    //when reicieving a signal the handler updates "received" to 1
    signal(SIGUSR1,readUsual);
    
    //create a child process
    pid_t childpid = fork(); 
    
    //forking went bad
    if (childpid < 0)
        printf("Can't create child process\n");
    //if we are the child process
    else if (childpid == 0)
    {
        //child is looping until it gets a kill signal from parent
        while (1){}

    }
    
    //we are the parent process
    else 
    {
        //send 3 SIGUSR1 signals to the child
        sleep(1);
        int i;
        for(i = 0 ; i < 3 ; i++){
            kill(childpid, SIGUSR1);
            sleep(1);

        }
        // terminate child process
        printf("parent is terminating child\n");
        kill(childpid, SIGKILL);
        printf("parent is terminating it self\n");

    }

    return 0;
}