#include <time.h> 		/* time */
#include <unistd.h>		/* getpid */
#include <stdatomic.h>	/* atomic_size_t */

#include "uid.h"	


unique_id_t CreateUID()
{
	static atomic_size_t counter = 0;
	unique_id_t my_id = {0};

	my_id.counter = atomic_fetch_add(&counter, 1) + 1;
	my_id.timestamp = time(NULL);
	my_id.pid = getpid();

	return my_id;
}

int IsEqualUIDs(unique_id_t uid1, unique_id_t uid2)
{
	return (uid1.counter == uid2.counter &&
			uid1.timestamp == uid2.timestamp &&
			uid1.pid == uid2.pid);
}

unique_id_t GetBadUID()
{
	unique_id_t my_id = {0};

	return my_id;
}


