#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

static int signalState = 0;

void signalHandler(int sig)
{
    if (sig == SIGUSR1)
        signalState = 1;
}

int main()
{
    //SIGUSR1 signal handler
    //when reicieving a signal the handler updates "received" to 1
    signal(SIGUSR1, signalHandler);
    
    int fd[2];  
    //int fd2[2]; 
    pipe(fd);
    //create a child process
    pid_t childpid = fork(); 
    
    //forking went bad
    if (childpid < 0)
        printf("Can't create child process\n");
    //child process
    else if (childpid == 0)
    {
        int childValue = 99;
        //child is looping until it gets a kill signal from parent
        while (1){
            sleep(1);
            if(signalState == 1)
            {
                printf("child received SIGUSR1.\n" ); 
                // Close writing end of first pipe 
                close(fd[1]);
                printf("child is reading from pipe.\n" ); 
                // Read a string using first pipe 
                read(fd[0], &childValue, sizeof(childValue)); 
                printf("child is closing pipe.\n" ); 
                close(fd[0]);//close first reading end
                printf("pipe closed.\n" ); 
                printf("child value: %d.\n", childValue);
                printf("debugging");
                signalState = 0 ;
            }
        }
    }
    //parent process
    else 
    {
        int paretValue = 0;
        close(fd[0]);  // Close reading end of first pipe 
        // Write input string and close writing end of first 
        // pipe. 
        write(fd[1], &paretValue, sizeof(paretValue)); 
        close(fd[1]); 
        //send 3 SIGUSR1 signals to the child
        sleep(1);
        kill(childpid, SIGUSR1);

        sleep(12);
        //terminate child process
        printf("parent is terminating child\n");
        kill(childpid, SIGKILL);
        printf("parent is terminating it self\n");
    }

    return 0;
}