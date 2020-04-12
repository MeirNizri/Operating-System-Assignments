#include <stdio.h>
#include <unistd.h>

void print(const char * proccess)
{
    int counter = 0;

    while(counter < 15)
        {
            printf("Hello from proccess %s \t#%d\n", proccess, counter);
            sleep(1);
            counter++;
        }
}

int main()
{
    __pid_t childPid, grandchildPid; 

    //creating new proccess - forking from parent to child
    childPid = fork();
    if(childPid == 0)
    {
        //creating new proccess - forking from child to grandchild
        grandchildPid = fork();
        if(grandchildPid == 0)
        {
            print("grandchild");
        }
        else
        {
            print("child");
        }
    }
    else
    {
        print("parent");
    }

    return 0;
}