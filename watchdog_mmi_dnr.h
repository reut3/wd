#ifndef __WATCHDOG_MMI_DNR_H__
#define __WATCHDOG_MMI_DNR_H__

#include <stddef.h> /* size_t */

/*
	MMI- make me immortal function starts a watch on the calling process.
	the watch will be checked by samples every interval_seconds for num_of_cycles times. 
	If the process terminates between calls to functions MMI and DNR,
	the process will be revived.
	
	In case of a failure MMI function will return non-zero value.
	MMI functions uses SIGUSR1 and SIGUSR2,
	undefined behavior if it recives from other sources.
*/
int MMI(int argc, char const *argv[], size_t interval_seconds, size_t num_of_cycles);

/*
	DNR- do not revive function stops the watch on the process.
	timeout is the max time in seconds for closing the watch.
	In case of a failure MMI function will return non-zero value.
*/
int DNR(size_t timeout);


#endif /* __WATCHDOG_MMI_DNR_H__ */

