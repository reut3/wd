#include <stdio.h>		/* printf */
#include <unistd.h>		/* sleep */
#include <time.h> 		/* time */
#include <pthread.h>	/* threads functions */
#include <stdatomic.h>	/* atomic */

#include "uid.h"



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
	printf("\033[1;34m%s \033[0m",str);
}

void PrintInYellow(const char *str){
	printf("\033[01;33m%s\033[0m",str);
}

/********************   Tests helpers    ***********************/

void CheckCounter(unique_id_t id, size_t expected_counter)
{
	PrintInBlue("Counter:");
	if (id.counter != expected_counter)
	{
		PrintInRed();
		printf("Failed, got=%ld, expected=%ld\n", id.counter, expected_counter);
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}
}

void CheckEquality(unique_id_t id1, unique_id_t id2, int expected)
{
	PrintInBlue("Equality:");
	if (IsEqualUIDs(id1, id2) != expected)
	{
		PrintInRed();
		printf("Failed\n");
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}
}

void CheckTimestamp(unique_id_t id, time_t expected_time)
{
	PrintInBlue("Timestamp:");
	if (id.timestamp != expected_time)
	{
		PrintInRed();
		printf("Failed\n");
		ResetColor();
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}
}

/********************   Tests    ***********************/


/*check Counter is correct*/
void Test1()
{
	unique_id_t a = CreateUID();
	unique_id_t b = CreateUID();
	unique_id_t c = CreateUID();

	PrintInYellow("\nTest1\n");
	CheckCounter(a, 1);
	CheckCounter(b, 2);
	CheckCounter(c, 3);
}

/*check Equality is correct*/
void Test2()
{
	unique_id_t a = CreateUID();
	unique_id_t b = CreateUID();

	PrintInYellow("\nTest2\n");
	CheckEquality(a, b, 0);

	a = GetBadUID();
	CheckEquality(a, b, 0);
	b = GetBadUID();
	CheckEquality(a, b, 1);
}

/*check Timestamp is correct*/
void Test3()
{
	unique_id_t a = {0};
	time_t time_a = time(NULL);

	PrintInYellow("\nTest3\n");
	sleep(3);
	a = CreateUID();
	CheckTimestamp(a, time_a + 3);
}



void AllTests()
{
	Test1();
	Test2();
	Test3();
}


int main()
{
	
	time_t a = time(0);
	time_t b = time(0);
	printf("%ld\n", a);
	printf("%ld\n", b);
	if(a == b)
	{
		printf("equals\n");
	}

	AllTests();

	return 0;
}