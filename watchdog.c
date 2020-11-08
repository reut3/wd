#define _GNU_SOURCE         
#include <stdlib.h>         /* setenv */
#include <assert.h>         /* assert */
#include <unistd.h>         /* getppid */

#include "watch_module.h"

#define NOT_USED(x) ((void)(x))
#define FAILED (-1)

static void FillClientInfo(proc_to_watch_info_t *client_info, char *argv[])
{
    client_info->is_client = 1;
    client_info->proc_id = getppid();
    client_info->argv = argv;
    client_info->program_name = argv[0];
}

int main (int argc, char *argv[])
{
    int status = setenv("WD", "WD", 0);
    proc_to_watch_info_t client_info = {0};

    assert(argv);

    FillClientInfo(&client_info, argv);

    if (status == FAILED) exit(1);

    status = Watch(&client_info);

    if (status == FAILED) exit(1);

    NOT_USED(argc); 

    return 0;
}
