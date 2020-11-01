#define _GNU_SOURCE         /* setenv */

#include <assert.h>         /* assert */
#include <pthread.h>        /* pthread_create */
#include <semaphore.h>      /* semaphore */
#include <fcntl.h>          /* For O_* constants */
#include <sys/stat.h>       /* For mode constants S_*/
#include <unistd.h>         /* fork execv */
#include <stdlib.h>         /* getenv */
#include <stdio.h>          /* sprintf */
#include <signal.h>         /* SIGUSR1 SIGUSR2 sigaction */

#include "watchdog_mmi_dnr.h"
#include "watch_module.h"

typedef struct mmi_info
{
    char const **argv;
    size_t num_of_cycles;
    size_t interval_seconds;
} mmi_info_t;

/*********************** macros ***********************/
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define SUCCESS (0)
#define FAILED (-1)
#define NOT_USED(x) ((void)(x))
#define IS_PARENT_IS_WATCHDOG (getenv("WD"))
#define CHILD_PID (0)
#define MMI_SEM_BUF_SIZE (16)
#define NUM_TO_VOID_POINTER(num) (*(void **)&(num))
#define PROC_SEM_BUF_SIZE (12)
#define SIZE_T_BUF_SIZE (11)

/*********************** Static functions declaration ***********************/
static int CreateWatchDog(char *const argv[], volatile pid_t *wd_pid);
static void *CreateWatchersThread(void *_client_args);
static int SetEnvp(size_t interval_seconds, size_t num_of_cycles);
static int ResetEnvp();
static int SetMMIEnv();

/*********************** Extern variables ***********************/
static volatile pthread_t watch_thread_id = 0;
static sem_t *mmi_sem = NULL;
static char mmi_sem_name[MMI_SEM_BUF_SIZE] = "MMI_";
static volatile proc_to_watch_info_t watchdog_info = {0};


/*********************** Functions implementation ***********************/
int MMI(int argc, char const *argv[], size_t interval_seconds, size_t num_of_cycles)
{
    pthread_t thread_id = 0;
    int status = 0;

    assert(argv);

    NOT_USED(argc);

    status = SetEnvp(interval_seconds, num_of_cycles);
    if (status != SUCCESS) return FAILED;

    status = SetMMIEnv();
    if (status != SUCCESS) return FAILED;

    status = pthread_create(&thread_id, NULL, CreateWatchersThread, argv);
    if (status != SUCCESS) return FAILED;

    watch_thread_id = thread_id;

    /* client waiting for watchers to be ready */
    status = sem_wait(mmi_sem);
    if (status != SUCCESS) return FAILED;

    status = sem_wait(mmi_sem);
    if (status != SUCCESS) return FAILED;

    return status;
}

int DNR(size_t timeout)
{
    int status = ResetEnvp();

    if (status != SUCCESS) return FAILED;
    
    status = StopWatch(timeout);

    if (status == FAILED)
    {
        pthread_cancel(watch_thread_id);
        kill(watchdog_info.proc_id, SIGKILL);
    }
    
    return SUCCESS;
}





/*********************** Static functions implementation ***********************/

static int SetMMIEnv()
{
    int value = -1;

    sprintf(mmi_sem_name, "MMI_%d", getpid()); 
    mmi_sem = sem_open(mmi_sem_name, O_CREAT, SEM_PERMS, 0);
    if (mmi_sem == SEM_FAILED) return FAILED;

    sem_getvalue(mmi_sem, &value);
    while (value)
    {
        if (sem_wait(mmi_sem) == FAILED) return FAILED;

        value--;
    }

    return SUCCESS;
}

static void *CreateWatchersThread(void *_client_args)
{
    char *const *client_args = _client_args;
    const char *program_name = "./watchdog.o";
    int status = 0;

    if (IS_PARENT_IS_WATCHDOG)
    {
        watchdog_info.proc_id = getppid();
    }
    else
    {
        status = CreateWatchDog(client_args, &watchdog_info.proc_id);
        if (status == FAILED) return (void *)FAILED;
    }

    watchdog_info.argv = client_args;
    watchdog_info.program_name = program_name;
    watchdog_info.is_client = 0;

    status = Watch(&watchdog_info);

    return NUM_TO_VOID_POINTER(status);
}

static int SetEnvp(size_t interval_seconds, size_t num_of_cycles)
{
    char interval_seconds_buf[SIZE_T_BUF_SIZE] = "";
    char num_of_cycles_buf[SIZE_T_BUF_SIZE] = "";
    int status = 0;

    sprintf(interval_seconds_buf, "%ld", interval_seconds);
    sprintf(num_of_cycles_buf, "%ld", num_of_cycles);
    
    status = setenv("interval_seconds", interval_seconds_buf, 0);
    if (status == FAILED) return FAILED;

    return setenv("num_of_cycles", num_of_cycles_buf, 0);
}

static int CreateWatchDog(char *const argv[], volatile pid_t *wd_pid)
{
    pid_t child_pid = fork();
                
    if (child_pid == FAILED) return FAILED;

    if (child_pid == CHILD_PID)
    {    
        execv("./watchdog.o", argv);

        return FAILED;  
    }

    *wd_pid = child_pid;

    return SUCCESS;
}

static int ResetEnvp()
{
    int status = unsetenv("interval_seconds");

    if (status == FAILED) return FAILED;

    status = unsetenv("num_of_cycles");

    if (status == FAILED) return FAILED;

    return unsetenv("WD");
}



