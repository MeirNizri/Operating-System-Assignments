#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static pid_t cpid;
static pid_t ppid;
static int pipefd[2];
static int childValue;
static int parentValue;

void parentSignalHandler(int sig)
{
    //reading from pipe
    printf("parent got %d from pipe.\n", (parentValue = getValue()));
    //
    if(parentValue < 5)
    {
        //increamenting and sending to pipe
        parentValue++;
        printf("parent sent %d to pipe.\n", sendValue(parentValue));
        //signaling child
        kill(cpid, SIGUSR1);
    }
    //return;
}

void childSignalHandler(int sig)
{
    //reading from pipe
    printf("child got %d from pipe.\n", (childValue = getValue()));
    if(childValue < 5)
    {
        //increamenting and sending to pipe
        childValue++;
        printf("child sent %d to pipe.\n", sendValue(childValue));
        //signaling parent
        kill(ppid, SIGUSR1);
    }
    else
        {
        printf("child is terminating.\n");
        //all open file descriptors are being closed, child terminates and SIGCHLD signal is sent to his parent
        exit(0);
        }
    //return;
}

void parentSIGCHLDHandler(int sig)
{
    //child has terminated it self so parent can terminate as well.
    printf("parent is terminating.\n");
    //all open file descriptors are being closed and parent terminates
    exit(0);
}

int getValue()
{       
    int val = 0;
    int success = read(pipefd[0], &val, sizeof(val));
    if(success != -1)
        return val;
    else
    {
        printf("getValue::reading from pipe failed\n");
        return -1;
    }
}

int sendValue(int val)
{
    int success = write(pipefd[1], &val, sizeof(val));
    if(success != -1)
        return val;
    else
    {
        printf("sendValue::writing to pipe failed\n");
        return -1;
    }
    
}


int main()
{
    //initializing pipe and values
    pipe(pipefd);
    childValue = 0;
    parentValue = 0;
    ppid = getpid();
    printf("parent pid: %d\n", ppid);
    
    cpid = fork(); 
    
    //parent
    if(cpid > 0)
    {
        printf("child pid: %d\n", cpid);
        //set up SIGUSR signal handler
        signal(SIGUSR1, parentSignalHandler);
        signal(SIGCHLD, parentSIGCHLDHandler);

        while(1)
            sleep(2);
    }
    //child
    else if(cpid == 0)
    {
        //set up SIGUSR signal handler
        signal(SIGUSR1, childSignalHandler);
        
        //sending first value and kickstarting pinpong session between the processes
        printf("child sent %d to pipe.\n", sendValue(childValue));    
        kill(ppid, SIGUSR1);
        
        while(1)
            sleep(2);
    }
    else
        printf("forking went bad\n");
    return;
}