#include <stdlib.h>			/* malloc free */
#include <assert.h>			/* assert */
#include <time.h> 			/* time */

#include "task.h"



struct task
{
	time_t time_to_operate;
	int (*to_do_function)(void *param);
	void *param;
	size_t interval;
	unique_id_t uid;
};



task_t *TaskCreate(int (*to_do_function)(void *param), void *param, size_t interval)
{
	task_t *task = (task_t *)malloc(sizeof(task_t));

	assert(to_do_function);

	if (task == NULL)
	{
		return NULL;
	}

	task->to_do_function = to_do_function;
	task->param = param;
	task->interval = interval;
	task->uid = CreateUID();
	task->time_to_operate = 0;

	return task;

}


void TaskDestroy(task_t *task)
{
	assert(task);

	free(task);
	task = NULL;
}


unique_id_t TaskGetUid(const task_t *task)
{
	assert(task);

	return (task->uid);
}



int TaskOperateFunc(const task_t *task)
{
	assert(task);

	return (task->to_do_function(task->param));
}



void TaskCaclTimeToOperate(task_t *task)
{
	assert(task);

	task->time_to_operate = time(NULL) + task->interval;
}


time_t TaskGetTimeToOperate(const task_t *task)
{
	assert(task);

	return (task->time_to_operate);
}