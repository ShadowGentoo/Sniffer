#include "stdio.h"
#include "string.h"
#include "../include/ui.h"


int main(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-help") == 0)
        {
            showHelp();
            return 0;
        }
        else
        {
            printf("unknown command: %s\n", argv[i]);
            return 0; 
        }     
        printf("arg #%d: %s\n", i, argv[i]);
    }


    return 0;
}
