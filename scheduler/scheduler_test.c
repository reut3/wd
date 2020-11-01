#include <string.h>		/*strcmp strcat*/
#include <stdio.h>		/*printf*/

#include "scheduler.h"


void PrintInGreen(){
	printf("\033[0;32m");
}

void PrintInRed(){
	printf("\033[0;31m");
}

void ResetColor(){
	printf("\033[0m");
}

void PrintInBlue(const char *str){
	printf("\033[1;34m%s\033[0m",str);
}


/****************   Test for each function    *******************/


void CheckSize(scheduler_t *scheduler, size_t expected)
{
	size_t answer = SchedulerSize(scheduler);

	PrintInBlue("Size:");
	if (answer != expected)
	{
		PrintInRed();
		printf("Failed, got=%ld, expected=%ld\n", answer, expected);
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}
}


void CheckIsEmpty(scheduler_t *scheduler, int expected)
{
	int answer = SchedulerIsEmpty(scheduler);

	PrintInBlue("IsEmpty:");
	if (answer != expected)
	{
		PrintInRed();
		printf("Failed, got=%d, expected=%d\n", answer, expected);
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}
}


/****************   help functions    *******************/


#define SizeOfArr(arr) (sizeof(arr)/sizeof(arr[0]))


int MulNumWith2(void *num)
{
	*((int *)num)*= 2;

	if (*((int *)num) == 48 || *((int *)num) > 4000)
	{
		return 1;
	}

	return 0;
}

int StrCat(void *dest)
{
 	strcat((char *)dest, "Yay");

	return 0;
}

int StopFunc(void *param)
{
	SchedulerStop((scheduler_t *)param);

	return 0;
}


int ChangeFirstChar(void *dest)
{
 	char *p_dest = (char *)dest;

 	*p_dest = (*p_dest) + 1;

 	if (*p_dest == 'm')
 	{
 		return 1;
 	}

	return 0;
}


void CheckUidNotBad(unique_id_t uid)
{
	PrintInBlue("UidNotBad:");
	if (IsEqualUIDs(uid, GetBadUID()))
	{
		PrintInRed();
		printf("Failedss\n");
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass \n");
		ResetColor();
	}
}


void CheckStringEquals(const char *answer, const char *expected)
{
	if (strcmp(answer,expected) != 0)
	{
		PrintInRed();
		printf("Failed, got=%s, expected=%s\n", answer, expected);
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass \n");
		ResetColor();
	}
}


void CheckNumsEquals(int num, int expected_num)
{
	if (num != expected_num)
	{
		PrintInRed();
		printf("Failed, got=%d, expected=%d\n", num, expected_num);
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass \n");
		ResetColor();
	}
}






/****************   Scenarios functions    *******************/



/*create an empty scheduler, check isEmpty and destroy it.
	check functions:
		SchedulerCreate
		SchedulerIsEmpty
		SchedulerDestroy
*/
void TestScenario1()
{
	scheduler_t *scheduler = SchedulerCreate();

	printf("\n	TestScenario1\n");

	CheckIsEmpty(scheduler, 1);
	SchedulerDestroy(scheduler);
}


/*create a scheduler add event and run it 
	check functions:
		SchedulerAddEvent
		SchedulerIsEmpty
		SchedulerSize
	*/
void TestScenario2()
{
	scheduler_t *scheduler = SchedulerCreate();
	int num = 3;
	unique_id_t uid = {0};

	printf("\n	TestScenario2\n");
	PrintInBlue("AddEvent,");
	uid = SchedulerAddEvent(scheduler, MulNumWith2, &num, 2);
	CheckUidNotBad(uid);

	CheckIsEmpty(scheduler, 0);
	CheckSize(scheduler, 1);

	SchedulerRun(scheduler);
	PrintInBlue("Check event worked as expected:");
	CheckNumsEquals(num, 48);

	SchedulerDestroy(scheduler);
}


/*create a scheduler run it without add event
	then add one event and run again
	check functions:
		SchedulerAddEvent
		SchedulerRun
	*/
void TestScenario3()
{
	scheduler_t *scheduler = SchedulerCreate();
	int num = 3;
	unique_id_t uid = {0};

	printf("\n	TestScenario3\n");

	PrintInBlue("Run1,");
	SchedulerRun(scheduler);

	PrintInBlue("AddEvent,");
	uid = SchedulerAddEvent(scheduler, MulNumWith2, &num, 1);
	CheckUidNotBad(uid);

	PrintInBlue("Run2,");
	SchedulerRun(scheduler);

	PrintInBlue("Check event worked as expected:");
	CheckNumsEquals(num, 48);
	CheckIsEmpty(scheduler, 1);
	CheckSize(scheduler, 0);

	SchedulerDestroy(scheduler);
}



/*create a scheduler add 3 events 
	and stop event, run it, when stopped remove event
	and run again. check it works as expected.
	check functions:
		SchedulerAddEvent
		SchedulerStop
		SchedulerRemoveEvent
*/
void TestScenario4()
{
	scheduler_t *scheduler = SchedulerCreate();

	int (*funcs_arr[])(void *) = {MulNumWith2, ChangeFirstChar, StrCat, StopFunc};
	unique_id_t uid_to_remove = {0};

	int num = 1;
	char buffer_change[] = "hello";
	char buffer_cat[200] = "";

	void *params[4] = {0};
	size_t intervals[] = {1, 2, 3, 12};
	size_t i = 0;

	params[0] = &num;
	params[1] = buffer_change;
	params[2] = buffer_cat;
	params[3] = scheduler;

	printf("\n	TestScenario4\n");

	for ( ; i < SizeOfArr(funcs_arr); i++)
	{
		unique_id_t uid = {0};

		PrintInBlue("AddEvent,");
		uid = SchedulerAddEvent(scheduler, funcs_arr[i], params[i], intervals[i]);
		CheckUidNotBad(uid);
		if (i == 2)
		{
			uid_to_remove = uid;
		}
	}

	CheckSize(scheduler, SizeOfArr(funcs_arr));

	PrintInBlue("Run 1,");
	SchedulerRun(scheduler);

	PrintInBlue("Check event MulNumWith2 worked as expected:");
	CheckNumsEquals(num, 2048);
	PrintInBlue("Check event ChangeFirstChar worked as expected:");
	CheckStringEquals(buffer_change, "mello");
	PrintInBlue("Check event StrCat worked as expected:");
	CheckStringEquals(buffer_cat, "YayYayYay");

	SchedulerRemoveEvent(scheduler, uid_to_remove);
	CheckSize(scheduler, SizeOfArr(funcs_arr) - 2);

	PrintInBlue("Run 2,");	/*running events are MulNumWith2 and StopFunc*/
	SchedulerRun(scheduler);

	PrintInBlue("Check event MulNumWith2 worked as expected:");
	CheckNumsEquals(num, 4096);
	PrintInBlue("Check event ChangeFirstChar stoped:");
	CheckStringEquals(buffer_change, "mello");
	PrintInBlue("Check event StrCat removed:");
	CheckStringEquals(buffer_cat, "YayYayYay");

	SchedulerDestroy(scheduler);
}



/*create a scheduler add 2 events 
	and stop event, run it, when stopped clear all events
	add one event and stop event and run again.
	check it works as expected.
	check functions:
		SchedulerAddEvent
		SchedulerStop
		SchedulerClear
	*/
void TestScenario5()
{
	scheduler_t *scheduler = SchedulerCreate();

	int (*funcs_arr[])(void *) = {MulNumWith2, ChangeFirstChar, StopFunc};
	void *params[3] = {0};
	size_t intervals[] = {5, 2, 11};

	int num = 1;
	char buffer_change[] = "dont worry be happy";
	char buffer_cat[200] = "";
	size_t i = 0;

	params[0] = &num;
	params[1] = buffer_change;
	params[2] = scheduler;

	printf("\n	TestScenario5\n");

	for ( ; i < SizeOfArr(funcs_arr); i++)
	{
		unique_id_t uid = {0};

		PrintInBlue("AddEvent,");
		uid = SchedulerAddEvent(scheduler, funcs_arr[i], params[i], intervals[i]);
		CheckUidNotBad(uid);
	}

	CheckSize(scheduler, SizeOfArr(funcs_arr));
	
	PrintInBlue("Run,");
	SchedulerRun(scheduler);

	CheckSize(scheduler, SizeOfArr(funcs_arr));
	CheckIsEmpty(scheduler, 0);

	PrintInBlue("Check event MulNumWith2 worked as expected:");
	CheckNumsEquals(num, 4);
	PrintInBlue("Check event ChangeFirstChar stoped:");
	CheckStringEquals(buffer_change, "iont worry be happy");


	SchedulerClear(scheduler);
	CheckSize(scheduler, 0);
	CheckIsEmpty(scheduler, 1);

	PrintInBlue("AddEvent,");
	SchedulerAddEvent(scheduler, StrCat, buffer_cat, 7);
	SchedulerAddEvent(scheduler, StopFunc, scheduler, 8);
	CheckSize(scheduler, 2);
	CheckIsEmpty(scheduler, 0);

	PrintInBlue("Run,");
	SchedulerRun(scheduler);

	CheckSize(scheduler, 2);
	CheckIsEmpty(scheduler, 0);

	PrintInBlue("Check event StrCat worked as expected:");
	CheckStringEquals(buffer_cat, "Yay");

	SchedulerDestroy(scheduler);
}



void Tests()
{
	TestScenario1();
	TestScenario2();
	TestScenario3(); 
	TestScenario4();
	TestScenario5();
}


int main()
{
	Tests();

	return 0;
}