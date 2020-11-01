#include <stdio.h>      /* printf */
#include <unistd.h>     /* sleep */

#include "watchdog_mmi_dnr.h"


static void TestMmiDnr(int argc, char const *argv[])
{
    size_t i = 0;
    int status = 0;

    printf("BEFORE MMI client process is= %d\n", getpid());

    status = MMI(argc, argv, 2, 3);
    if (status == -1) printf("FAILED MMI\n");

    printf("INSIDE MMI\n");
    
    printf("~~~~~~ start sleeping\n");
    for (i = 0; i < 10; i++)
    {
        sleep(10);
    }

    printf("~~~~~~ end sleeping\n");
    status = DNR(3);
    if (status == -1) printf("FAILED DNR\n");

    printf("AFTER DNR\n");
}


int main(int argc, char const *argv[])
{
    int i = 0;

    printf("\n");
    for (; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    
    printf("\nMMI DNR first time\n");
    TestMmiDnr(argc, argv);

    sleep(5);

    printf("\nMMI DNR second time\n");
    TestMmiDnr(argc, argv);

    return 0;
}


/* 
    checked tests:
    
    killing watchdog and app several times in the mmi section
    checked in 2 sections of mmi-dnr
    checked client's arguments restored when client failed

 */