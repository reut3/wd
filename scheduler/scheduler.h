#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "uid.h"

#include <stddef.h> 	/*size_t*/

typedef struct scheduler scheduler_t;

/*
	SchedulerCreate returns the a pointer to the created scheduler,
	in case of failure returns NULL.
*/
scheduler_t *SchedulerCreate();

void SchedulerDestroy(scheduler_t *scheduler);

/* 
	SchedulerAddEvent creates and add a new event to the scheduler.
	The event will occur every interval seconds, and will call to_do_func.
	SchedulerAddEvent returns event's UID, in case of a failure the returning uid will be a bad uid.
	to_do_func is active while return value is 0,
	if to_do_func returns non-zero value the event will remove from the scheduler.
*/
unique_id_t SchedulerAddEvent(scheduler_t *scheduler, int (*to_do_func)(void *param),
									 void *param, size_t interval);

/*
	SchedulerRemoveEvent recives UID of event in scheduler and removes it
*/
void SchedulerRemoveEvent(scheduler_t *scheduler, unique_id_t uid);

/*
	SchedulerStop stops the scheduler,
	this function should be called inside a user_function that will be send as a new event to SchedulerAddEvent,
	ex: SchedulerAddEvent(scheduler, user_function, scheduler, interval)
	ex to user_function:
		int user_function(void *param)
		{
			...
			SchedulerStop((scheduler_t *)param);

			return 0;
		}
*/
void SchedulerStop(scheduler_t *scheduler);

/*
	SchedulerRun starts the scheduler operation.
*/
void SchedulerRun(scheduler_t *scheduler);

/*
	SchedulerClear Clear all events from the scheduler.
*/
void SchedulerClear(scheduler_t *scheduler);

int SchedulerIsEmpty(const scheduler_t *scheduler);

size_t SchedulerSize(const scheduler_t *scheduler);




#endif /*__SCHEDULER_H__*/

