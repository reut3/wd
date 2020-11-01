#ifndef __UID_H__
#define __UID_H__

#include <sys/types.h> 	/* pid_t, size_t, time_t */

typedef struct unique_id 
{
	size_t counter;
	time_t timestamp;
	pid_t pid;
} unique_id_t;


/* 
	CreateUID returns a unique id for a process.
*/
unique_id_t CreateUID();

int IsEqualUIDs(unique_id_t, unique_id_t);

/*
	GetBadUID returns a representation of a bad id.
*/
unique_id_t GetBadUID();


#endif /* __UID_H__ */
