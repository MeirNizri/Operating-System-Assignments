#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

static  int pipefd[2];  
static pid_t ppid;
static pid_t cpid;
static int childValue;
static int parentValue;


void parentSignalHandler(int sig)
{
    if(sig == SIGUSR1){ 
        //printf("parent process recieved signal\n");
        sleep(2);
        
        //reading from pipe
        close(pipefd[1]);//close writing end
        read(pipefd[0], &parentValue, sizeof(parentValue));//read from pipe to parentValue
        close(pipefd[0]);//close reading end
        
        //-----debugging-----
        printf("parent recieved: %d \n", parentValue);
        //-----debugging-----
            
       //increamenting, writeing to pipe and signaling to child
        if(parentValue < 5){
            parentValue++;
            
            //-----debugging-----
            printf("parent sends %d \n", parentValue);
            //-----debugging-----
            
            write(pipefd[1], &parentValue, sizeof(parentValue));//write to writing end
            close(pipefd[1]);//close writing end
            kill(cpid, SIGUSR1);
        }
        else{
            printf("parent killing child");
            kill(cpid, SIGKILL);
            printf("parent commiting suicide");
            kill(ppid, SIGKILL);
        }
    }
    
    else if(sig == SIGCHLD)
        kill(ppid, SIGKILL);
}


void childSignalHandler(int sig)
{
    if(sig == SIGUSR1){ 
        //printf("child process recieved signal\n");
        sleep(2);
        
        //reading from pipe
        close(pipefd[1]);//close writing end
        read(pipefd[0], &childValue, sizeof(childValue));//read from pipe to parentValue
        close(pipefd[0]);//close reading end
        
        //-----debugging-----
        printf("child recieved: %d \n", childValue);
        //-----debugging-----
    
       //increamenting, writeing to pipe and signaling to child
        if(childValue < 5){
            childValue++;
            
            //-----debugging-----
            printf("child sends %d \n", childValue);
            //-----debugging-----
            
            write(pipefd[1], &childValue, sizeof(childValue));//write to writing end
            close(pipefd[1]);//close writing end
            kill(ppid, SIGUSR1);
        }
        else{
            printf("child signaling SIGCHLD to parent");
            kill(ppid, SIGCHLD);
            printf("child commiting suicide");
            kill(cpid, SIGKILL);
        }
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
        parentValue = 0;
        //SIGUSR1 signal handler
        signal(SIGUSR1, parentSignalHandler);
        
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
        childValue = 0;
        //SIGUSR1 signal handler
        signal(SIGUSR1, childSignalHandler); 
        
        //-----debugging-----
        printf("child pid: %d \n", getpid());
        printf("child sends 0 \n");
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