#include <stdlib.h>			/* malloc free */
#include <assert.h>			/* assert */
#include <unistd.h>			/* sleep */
#include <time.h> 		/* time, difftime*/
#include <stdio.h>		/* printf*/

#include "scheduler.h"
#include "task.h"
#include "heap_pq.h"





/********* struct enum and static functions *********/

struct scheduler
{
	pq_t *pq;
	int run;
};

enum run {KEEP_RUNNING, STOP_RUNNING};

#define TASK_TIME_NOT_INITIALIZED(task) (TaskGetTimeToOperate(task) == 0)

static int CompareTimeToOperate(const void *data1, const void *data2)
{
	time_t time1 = TaskGetTimeToOperate((task_t *)data1);
	time_t time2 = TaskGetTimeToOperate((task_t *)data2);

	return difftime(time1, time2);
}

static int CompareUid(const void *queue_data, const void *input_data)
{
	unique_id_t uid1 = TaskGetUid((task_t *)queue_data);
	unique_id_t uid2 = *((unique_id_t *)input_data);

	return (IsEqualUIDs(uid1, uid2));
}

static void InitialScheduler(scheduler_t *scheduler)
{
	task_t *task = NULL;
	scheduler->run = KEEP_RUNNING;

	task = PQPeek(scheduler->pq);
	while (TASK_TIME_NOT_INITIALIZED(task))
	{
		PQDequeue(scheduler->pq);
		TaskCaclTimeToOperate(task);
		PQEnqueue(scheduler->pq, task);
		task = PQPeek(scheduler->pq);
	}
}


/********* Functions implemention *********/


scheduler_t *SchedulerCreate()
{
	scheduler_t *my_scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));

	if (my_scheduler == NULL)
	{
		return NULL;
	}

	my_scheduler->pq = PQCreate(CompareTimeToOperate);
	if (my_scheduler->pq == NULL)
	{
		free(my_scheduler);
		my_scheduler = NULL;

		return NULL;
	}

	return my_scheduler;
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->pq);

	SchedulerClear(scheduler);
	PQDestroy(scheduler->pq);
	free(scheduler);
	scheduler = NULL;
}


unique_id_t SchedulerAddEvent(scheduler_t *scheduler,
					int (*to_do_function)(void *param), void *param, size_t interval)
{
	task_t *task = NULL;

	assert(scheduler);
	assert(scheduler->pq);
	assert(to_do_function);
	
	task = TaskCreate(to_do_function, param, interval);

	if (task == NULL || PQEnqueue(scheduler->pq, task))
	{
		return GetBadUID();
	}

	return TaskGetUid(task);
}


void SchedulerRemoveEvent(scheduler_t *scheduler, unique_id_t uid)
{
	task_t *task = NULL;

	assert(scheduler);
	
	task = (task_t *)PQEraseIfMatch(scheduler->pq, &uid, CompareUid);

	if (task != NULL)
	{
		TaskDestroy(task);
	}
}



void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler);

	scheduler->run = STOP_RUNNING;
}


void SchedulerRun(scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->pq);

	if (PQIsEmpty(scheduler->pq)) 	/*no events to run*/
	{
		return;
	}

	InitialScheduler(scheduler);

	while (scheduler->run == KEEP_RUNNING && !PQIsEmpty(scheduler->pq))
	{
		task_t *task = PQPeek(scheduler->pq);

		PQDequeue(scheduler->pq);
		sleep(difftime(TaskGetTimeToOperate(task), time(NULL)));
		
		if (TaskOperateFunc(task) == KEEP_RUNNING)
		{
			TaskCaclTimeToOperate(task);
			if (PQEnqueue(scheduler->pq, task) != 0) /*failed enqueue*/
			{
				printf("Failed\n");
				TaskDestroy(task);
			}
		}
		else
		{
			TaskDestroy(task);
		}		
	}
}




void SchedulerClear(scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->pq);

	while (!PQIsEmpty(scheduler->pq))
	{
		task_t *task = PQPeek(scheduler->pq);

		PQDequeue(scheduler->pq);
		TaskDestroy(task);
	}
}



int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->pq);

	return PQIsEmpty(scheduler->pq);
}

size_t SchedulerSize(const scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->pq);
	
	return PQSize(scheduler->pq);
}
