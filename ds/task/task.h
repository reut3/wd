#ifndef __TASK_H__
#define __TASK_H__

#include "uid.h"

#include <stddef.h> 	/*size_t*/

typedef struct task task_t;

/* 
	TaskCreate params:

	to_do_function: function's return value is 0 while function
		is active, and non-zero value otherwise.
	param: param of the function.
	interval: the number of seconds between each running of to_do_function.
 */
task_t *TaskCreate(int (*to_do_function)(void *param), void *param, size_t interval);

void TaskDestroy(task_t *task);

/* 
	TaskGetUid returns task's uid.
*/
unique_id_t TaskGetUid(const task_t *task);

/*
	TaskCaclTimeToOperate Function updates time to operate the task.
*/
void TaskCaclTimeToOperate(task_t *task);

/*
	TaskOperateFunc operates the task's function,
	returns zero while the function is active,
	and non-zero value otherwise
*/
int TaskOperateFunc(const task_t *task);

/* 
	TaskGetTimeToOperate returns the next time task will operate.
*/
time_t TaskGetTimeToOperate(const task_t *task);


#endif /*__TASK_H__*/