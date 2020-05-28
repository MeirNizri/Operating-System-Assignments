#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

int main()
{
    __pid_t deamon_pid = fork();

    if(deamon_pid == 0)
    {
        chdir("/");
        setsid();

        close(stdout);
        close(stderr);
        close(stdin);

        openlog("my_deamon_proccess", LOG_PID, LOG_DAEMON);
        syslog(LOG_NOTICE, "deamon proccess has started");
        sleep(3);
        syslog(LOG_NOTICE, "deamon is working on something..");
        sleep(30);
        syslog(LOG_NOTICE, "deamon proccess has finished");


    }  
    else
    {
        printf("deamon PID is: %d\n", deamon_pid);
    }

    return 0;
}