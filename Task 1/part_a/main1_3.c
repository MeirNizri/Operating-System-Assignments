#include <stddef.h>  // define null
#include <stdbool.h> // define boolean type
#include <dlfcn.h>  // load func in runtime
#include <stdio.h>   

void (*print_hello)(const char *);

bool init_library()
{
    void *hdl = dlopen("./libHello.so", RTLD_LAZY);
    if(hdl == NULL)
        return false;
    print_hello = (void(*)(const char *))dlsym(hdl,"print_hello");
    if(print_hello == NULL)
        return false;
    return true;
}

int main()
{
    if(init_library())
        print_hello("Ariel");
    else
        printf("library not loaded \n");
    return 0;
}