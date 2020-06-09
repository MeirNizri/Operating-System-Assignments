#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static pid_t cpid;
static pid_t ppid;
static int pipefd[2];
static int pipefd2[2];
static int childValue;
static int parentValue;

void parentSignalHandler(int sig)
{
    //printf("parent handles SIGUSR1 signal\n");
    
    //reading from pipe
    printf("parent got %d from pipe.\n", (parentValue = parentRead()));
    //
    if(parentValue < 5)
    {
        //increamenting and sending to pipe
        parentValue++;
        printf("parent sent %d to pipe.\n", parentWrite(parentValue));
        //signaling child
        kill(cpid, SIGUSR1);
    }
    return;
}

void childSignalHandler(int sig)
{
    //printf("child handles SIGUSR1 signal\n");

    //reading from pipe
    printf("child got %d from pipe.\n", (childValue = childRead()));
    if(childValue < 5)
    {
        childValue++;
        printf("child sent %d to pipe.\n", childWrite(childValue));
        //signaling parent
        kill(ppid, SIGUSR1);
    }
    else
        {
        printf("child is terminating.\n");
        //all open file descriptors are being closed, child terminates and SIGCHLD signal is sent to his parent
        exit(0);
        }
    return;
}

void parentSIGCHLDHandler(int sig)
{
    //child has terminated it self so parent can terminate as well.
    printf("parent is terminating.\n");
    //all open file descriptors are being closed and parent terminates
    exit(0);
}

int childRead()
{       
    int success = 0;
    int val = 0;
    success = read(pipefd[0], &val, sizeof(val));
    if(success != -1)
        return val;
    else
    {
        printf("childRead::readin from pipe #1 failed\n");
        return -1;
    }
}

int parentWrite(int val)
{
    int success = 0;
    success = write(pipefd[1], &val, sizeof(val));
    if(success != -1)
        return val;
    else
    {
        printf("parentWrite::writing to pipe #1 failed\n");
        return -1;
    }
    
}

int parentRead()
{    
    int success = 0;
    int val = 0;
    success = read(pipefd2[0], &val, sizeof(val));
    if(success != -1)
        return val;
    else
    {
        printf("parentRead::readin from pipe #2 failed\n");
        return -1;
    }
}

int childWrite(int val)
{
    int success = 0;
    success = write(pipefd2[1], &val, sizeof(val));
    if(success != -1)
        return val;
    else
    {
        printf("childWrite::writing to pipe #2 failed\n");
        return -1;
    }
}

int main()
{
    //initializing pipes and values
    pipe(pipefd);
    pipe(pipefd2);
    childValue = 0;
    parentValue = 0;
    ppid = getpid();
    printf("parent pid: %d\n", ppid);
    
    cpid = fork(); 
    
    //parent
    if(cpid > 0)
    {
        printf("child pid: %d\n", cpid);
        //preparing sig set with SIGUSR1 to set up sig suspend
        // sigset_t sigusr1;
        // sigaddset(&sigusr1, SIGUSR1);
        //set up SIGUSR signal handler
        signal(SIGUSR1, parentSignalHandler);
        signal(SIGCHLD, parentSIGCHLDHandler);

        while(1)
        {
            //parent process will pause until it gets SIGUSR1 signal and handle it.
            //sigsuspend(&sigusr1); 
            sleep(1);

        }
    }
    //child
    else
    {
        //preparing sig set with SIGUSR1 to set up sig suspend
        // sigset_t sigusr1;
        // sigaddset(&sigusr1, SIGUSR1);
        //set up SIGUSR signal handler
        signal(SIGUSR1, childSignalHandler);
        
        //sending first value and kickstarting pinpong session between the processes
        printf("child sent %d to pipe.\n", childWrite(childValue));    
        kill(ppid, SIGUSR1);
        
        while(1)
        {
            //child process will pause until it gets SIGUSR1 signal and handle it.
            //sigsuspend(&sigusr1);  
            sleep(1);
        }
    }
    return;
}