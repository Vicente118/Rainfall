#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *shellcode_env = getenv("SHELLCODE");

    printf("Address of SHELLCODE environment variable: %p\n", shellcode_env);

    return 0;
}