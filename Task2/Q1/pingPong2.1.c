#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

static  int pipefd[2];  
static pid_t ppid;
static pid_t cpid;
static int childValue;
static int parentValue;


void signalHandler1(int sig)
{
    //printf("parent process recieved signal\n");
    sleep(2);
    
    //reading from pipe
    close(pipefd[1]);//close writing end
    read(pipefd[0], &parentValue, sizeof(parentValue));//read from pipe to parentValue
    close(pipefd[0]);//close reading end
    printf("parent recieved: %d \n", parentValue);

   //increamenting, writeing to pipe and signaling to child
    if(parentValue < 5){
        parentValue++;
        write(pipefd[1], &parentValue, sizeof(parentValue));//write to writing end
        close(pipefd[1]);//close writing end
        kill(cpid, SIGUSR1);
    }
    else{
        kill(cpid, SIGKILL);
        kill(ppid, SIGKILL);
    }

}


void signalHandler2(int sig)
{
    //printf("child process recieved signal\n");
    sleep(2);
    
    //reading from pipe
    close(pipefd[1]);//close writing end
    read(pipefd[0], &childValue, sizeof(childValue));//read from pipe to parentValue
    close(pipefd[0]);//close reading end
   printf("child recieved: %d \n", childValue);

   //increamenting, writeing to pipe and signaling to child
    if(parentValue < 5){
        parentValue++;
        write(pipefd[1], &childValue, sizeof(childValue));//write to writing end
        close(pipefd[1]);//close writing end
        kill(ppid, SIGUSR1);
    }
    else{
        kill(ppid, SIGCHLD);
        kill(cpid, SIGKILL);
    }
}




int main()
{
    //create pipe
    pipe(pipefd);
    
    //create a child process
    pid_t childpid = fork(); 
    
    //forking went bad
    if (childpid < 0)
        printf("Can't create child process\n");
        
    //parent process
    else if (childpid > 0)
    {
        
        //SIGUSR1 signal handler
        signal(SIGUSR1, signalHandler1);
        
        //-----debugging-----
        printf("parent pid: %d \n", getpid());
        //-----debugging-----

        ppid = getpid();
        cpid = childpid;
        
        while(1){}
    }
    
    //child process
    else 
    {
        //SIGUSR1 signal handler
        signal(SIGUSR1, signalHandler2); 
        
        //-----debugging-----
        printf("child pid: %d \n", getpid());
        //-----debugging-----
        
        //initial writing and signaling
        close(pipefd[0]);//close reading end
        write(pipefd[1], &childValue, sizeof(childValue));//write to writing end
        close(pipefd[1]);//close writing end
        kill(ppid, SIGUSR1);//signal parent
        
        while(1){}
    }

    return 0;
}