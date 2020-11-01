#ifndef __WATCH_MODULE_H__
#define __WATCH_MODULE_H__


#include <stddef.h> 		/* size_t */
#include <sys/types.h>		/* pid_t */

typedef struct proc_to_watch_info
{
	pid_t proc_id;
	int is_client;
	char *const *argv;
	const char *program_name;
} proc_to_watch_info_t;


int Watch(volatile proc_to_watch_info_t *proc_info);

int StopWatch(size_t timeout);

#endif /* __WATCH_MODULE_H__ */


