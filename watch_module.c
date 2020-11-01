#define _GNU_SOURCE         /* sigaction, siginfo_t */
#include <semaphore.h>      /* semaphore */
#include <fcntl.h>          /* For O_* constants */
#include <sys/stat.h>       /* For mode constants S_*/
#include <signal.h>         /* SIGUSR1 SIGUSR2 sigaction */
#include <unistd.h>         /* fork execv */
#include <stdio.h>          /* sprintf */
#include <stdatomic.h>      /* atomic_t */
#include <stdlib.h>         /* getenv */
#include <errno.h>          /* EINTR */
#include <time.h>           /* clock_gettime */

#include "watch_module.h"
#include "scheduler.h"

/*********************** macros ***********************/
#define MMI_DNR_SEM_BUF_SIZE (16)
#define PROC_SEM_BUF (12)
#define FAILED (-1)
#define ALIVE (0)
#define NOT_USED(x) ((void)(x))
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define CHILD_PID (0)
#define SUCCESS (0)
#define TIME_IN_SEC_TO_CHECK_DNR (1)

/*********************** Static variables ***********************/
static char cur_proc_name[PROC_SEM_BUF] = "";
static char proc_to_watch_name[PROC_SEM_BUF] = "";
static char mmi_sem_name[MMI_DNR_SEM_BUF_SIZE] = "MMI_";
static char dnr_sem_name[MMI_DNR_SEM_BUF_SIZE] = "DNR_";
static atomic_size_t alive_signal_counter = 0;
static atomic_size_t dnr_counter = 0;
static volatile pid_t proc_to_watch_pid = 0;
static size_t threashold = 0;
static sem_t *dnr_sem = NULL;

/*********************** Static functions declaration ***********************/
static int InitHanlders();
static void AliveHandler(int signum);
static void DnrSignalHandler(int signum);
static int FillSchedulerWithTasks(scheduler_t *scheduler);
static int Revive(volatile proc_to_watch_info_t *proc_to_watch_info);
static int CheckAliveSignalCounterTask(void *param);
static int SendAliveSignalTask(void *param);
static void UnlinkSem();
static void SetWatchEnv(volatile proc_to_watch_info_t *proc_to_watch_info);

/*********************** Functions implementation ***********************/
int Watch(volatile proc_to_watch_info_t *proc_to_watch_info)
{
    sem_t *mmi_sem = NULL;
    sem_t *proc_to_watch_sem = NULL;
    sem_t *cur_proc_sem = NULL;
    scheduler_t *scheduler = NULL;
    int status = 0;
        
    SetWatchEnv(proc_to_watch_info);

    status = InitHanlders();
    if (status == FAILED) return FAILED;

    scheduler = SchedulerCreate(); 
    if (!scheduler) return FAILED;

    status = FillSchedulerWithTasks(scheduler); 
    if (status == FAILED)
    {
        SchedulerDestroy(scheduler);

        return FAILED;
    }

    cur_proc_sem = sem_open(cur_proc_name, O_CREAT, SEM_PERMS, 0);
    if (cur_proc_sem == SEM_FAILED) return FAILED;

    while (1)
    {
        proc_to_watch_sem = sem_open(proc_to_watch_name, O_CREAT, SEM_PERMS, 0);
        if (proc_to_watch_sem == SEM_FAILED) return FAILED;

        mmi_sem = sem_open(mmi_sem_name, O_CREAT, SEM_PERMS, 0);
        if (mmi_sem == SEM_FAILED) return FAILED;

        status = sem_post(cur_proc_sem);
        if (status == FAILED) return FAILED;

        status = sem_wait(proc_to_watch_sem);
        if (status == FAILED) return FAILED;

        status = sem_post(mmi_sem);
        if (status == FAILED) return FAILED;

        SchedulerRun(scheduler);

        if (atomic_load(&dnr_counter) >= 2)
        {
            UnlinkSem();
            SchedulerDestroy(scheduler);
            if (proc_to_watch_info->is_client)
            {
                sprintf(dnr_sem_name, "DNR_%d", proc_to_watch_pid);
                dnr_sem = sem_open(dnr_sem_name, O_CREAT, SEM_PERMS, 0);
            }

            sem_post(dnr_sem);
            
            break;
        }

        status = Revive(proc_to_watch_info);
        if (status == FAILED) return FAILED;
    }

    return SUCCESS;
}

int StopWatch(size_t timeout)
{
    
    struct timespec tm = {0};
    int status = SUCCESS;
    
    sprintf(dnr_sem_name, "DNR_%d", getpid());
    dnr_sem = sem_open(dnr_sem_name, O_CREAT, SEM_PERMS, 0);
    if (dnr_sem == SEM_FAILED) return FAILED;

    kill(getpid(), SIGUSR2);
    kill(proc_to_watch_pid, SIGUSR2);

    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_sec += timeout;

    while ((status = sem_timedwait(dnr_sem, &tm)) == FAILED && errno == EINTR);

    if (status != SUCCESS && errno != ETIMEDOUT) return FAILED;

    if (errno != ETIMEDOUT)
    {
        while ((status = sem_timedwait(dnr_sem, &tm)) == FAILED && errno == EINTR);

        if (status != SUCCESS && errno != ETIMEDOUT) return FAILED;
    }

    sem_unlink(dnr_sem_name);

    if (errno == ETIMEDOUT)
    {
        UnlinkSem();

        return FAILED;
    }


    return SUCCESS;
}


/*********************** Static functions implementation ***********************/

static void SetWatchEnv(volatile proc_to_watch_info_t *proc_to_watch_info)
{
    sprintf(cur_proc_name, "%d", getpid()); 
    sprintf(proc_to_watch_name, "%d", proc_to_watch_info->proc_id); 
    sprintf(mmi_sem_name, "MMI_%d", proc_to_watch_info->is_client ? proc_to_watch_info->proc_id : getpid()); 
    proc_to_watch_pid = proc_to_watch_info->proc_id;
    atomic_store(&dnr_counter, 0);
    atomic_store(&alive_signal_counter, ALIVE);
}

static void UnlinkSem()
{
    sem_unlink(proc_to_watch_name);
    sem_unlink(cur_proc_name);
    sem_unlink(mmi_sem_name);
}

static int InitHanlders()
{
    struct sigaction sigusr = {0};

    sigusr.sa_handler = AliveHandler;

    if (sigaction(SIGUSR1, &sigusr, NULL) == FAILED) return FAILED;

    sigusr.sa_handler = DnrSignalHandler;

    return sigaction(SIGUSR2, &sigusr, NULL);

}

static void AliveHandler(int signum)
{
    atomic_store(&alive_signal_counter, ALIVE);
    
    NOT_USED(signum);
}

static void DnrSignalHandler(int signum)
{
    atomic_fetch_add(&dnr_counter, 1);

    NOT_USED(signum);
}

static int SendAliveSignalTask(void *param)
{
    kill(proc_to_watch_pid, SIGUSR1);

    NOT_USED(param);

    return 0;
}

static int CheckAliveSignalCounterTask(void *param)
{
    if (atomic_load(&alive_signal_counter) >= threashold &&
                atomic_load(&dnr_counter) == 0)
    {
        SchedulerStop((scheduler_t *)param);
    }

	return SUCCESS;
}

static int CheckDnrCounterTask(void *param)
{
    if (atomic_load(&dnr_counter) == 1)
    {
        kill(proc_to_watch_pid, SIGUSR2);
    }
    
    else if (atomic_load(&dnr_counter) >= 2)
    {
        kill(proc_to_watch_pid, SIGUSR2);
        SchedulerStop((scheduler_t *)param);
    }

    NOT_USED(param);

	return SUCCESS;
}

static int IncreasingAliveSignalCounterTask(void *param)
{
    atomic_fetch_add(&alive_signal_counter, 1);

    NOT_USED(param);

	return SUCCESS;
}

static int FillSchedulerWithTasks(scheduler_t *scheduler)
{
    size_t interval_seconds = atol(getenv("interval_seconds"));
    size_t num_of_cycles = atol(getenv("num_of_cycles"));

    unique_id_t uid1 = SchedulerAddEvent(scheduler, SendAliveSignalTask,
                        NULL, interval_seconds);
    unique_id_t uid2 = SchedulerAddEvent(scheduler, CheckAliveSignalCounterTask,
                        scheduler, interval_seconds * num_of_cycles);
    unique_id_t uid3 = SchedulerAddEvent(scheduler, CheckDnrCounterTask,
                        scheduler, TIME_IN_SEC_TO_CHECK_DNR);
    unique_id_t uid4 = SchedulerAddEvent(scheduler,
                        IncreasingAliveSignalCounterTask, NULL, interval_seconds);
    unique_id_t bad_uid = GetBadUID();

    threashold = interval_seconds * num_of_cycles;

    if (IsEqualUIDs(bad_uid, uid1) || IsEqualUIDs(bad_uid, uid2) ||
                IsEqualUIDs(bad_uid, uid3) || IsEqualUIDs(bad_uid, uid4))
    {
        return FAILED;
    }

    return SUCCESS;
}

static int Revive(volatile proc_to_watch_info_t *proc_to_watch_info)
{
    pid_t child_pid = 0;

    child_pid = fork();

    if (child_pid == FAILED) return FAILED;

    if (child_pid == CHILD_PID)
    {
        execv(proc_to_watch_info->program_name, proc_to_watch_info->argv);
                
        return FAILED;  
    }

    sem_unlink(proc_to_watch_name);

    proc_to_watch_info->proc_id = child_pid;
    proc_to_watch_pid = child_pid;

    if (proc_to_watch_info->is_client)
    {
        sem_unlink(mmi_sem_name);
        sprintf(mmi_sem_name, "MMI_%d", child_pid);
    }
    
    sprintf(proc_to_watch_name, "%d", child_pid); 
    atomic_store(&alive_signal_counter, ALIVE);

    return SUCCESS;
}