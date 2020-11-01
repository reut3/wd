#include <stdio.h>		/*printf*/
#include <string.h>		/*strcmp strcat*/
#include "heap_pq.h"


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


void CheckCount(size_t answer, size_t expected)
{
	PrintInBlue("Count:");
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


void CheckIsEmpty(int answer, int expected)
{
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


/****************   help functions/structs    *******************/

#define BUFFER_SIZE (20)
#define SizeOfArr(arr) (sizeof(arr)/sizeof(arr[0]))


typedef struct human
{
	int age;
	char name[10];
} human;



int CmpStrings(const void *s1, const void *s2)
{
	return strcmp((char *)s1, (char *)s2);
}


int CmpHumans(const void *human1, const void *human2)
{
	return strcmp(((human *)human1)->name, ((human *)human2)->name);
}


int IsAgeEqual(const void *human1, const void *human2)
{
	if (((human *)human1)->age == ((human *)human2)->age)
	{
		return 1;
	}
	
	return 0;
}


int CheckHumansEquals(void *human1, void *human2)
{
	human *p_human1 = (human *)human1;
	human *p_human2 = (human *)human2;

	PrintInBlue("structs Equals:");
	if ((strcmp(p_human1->name, p_human2->name) != 0) ||
			(p_human1->age != p_human2->age))
	{
		PrintInRed();
		printf("Failed got= %s, %d, expected= %s, %d\n", p_human1->name, p_human1->age, p_human2->name, p_human2->age);
		ResetColor();

		return 0;
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();

		return 1;
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

/****************   Scenarios functions    *******************/



/*create an empty priority_queue, check isEmpty and destroy it.
	check functions:
		PQCreate
		PQDestroy
*/
void TestScenario1()
{
	pq_t *pq = PQCreate(CmpStrings);

	printf("\n	TestScenario1\n");

	CheckIsEmpty(PQIsEmpty(pq), 1);
	PQDestroy(pq);
}


/*create a priority_queue enqueue 4 elements check the queue  
	is as expected.
	check functions:
		PQEnqueue
		PQIsEmpty
		PQSize
	*/
void TestScenario2()
{
	pq_t *pq = PQCreate(CmpStrings);

	char data[][BUFFER_SIZE] = {"dont", "worry", "be", "happy"};
	size_t i = 0;

	printf("\n	TestScenario2\n");
	PrintInBlue("Enqueue,");
	for ( ; i < SizeOfArr(data); i++)
	{
		PQEnqueue(pq, data[i]);
	}

	CheckIsEmpty(PQIsEmpty(pq), 0);
	CheckCount(PQSize(pq), SizeOfArr(data));

	PQDestroy(pq);
}



/*create a priority_queue enqueue 4 elements 
	check the queue is as expected using peek and dequeue.
	check functions:
		PQDequeue
		PQPeek
	*/
void TestScenario3()
{
	pq_t *pq = PQCreate(CmpStrings);

	char data[][BUFFER_SIZE] = {"dont", "worry", "be", "happy"};
	char expected_data[][BUFFER_SIZE] = {"be", "dont", "happy", "worry"};
	size_t i = 0;

	printf("\n	TestScenario3\n");
	PrintInBlue("Enqueue, ");
	for ( ; i < SizeOfArr(data); i++)
	{
		PQEnqueue(pq, data[i]);
	}

	i = 0;
	PrintInBlue("Is queue is as expected:\n");
	while (!PQIsEmpty(pq))
	{
		PrintInBlue("Peek, Dequeue:");
		CheckStringEquals(PQPeek(pq), expected_data[i++]);
		PQDequeue(pq);
	}

	PQDestroy(pq);
}



/*create priority_queue enqueue 7 elements check ,
	clear the queue and add one element, check it works as expected.
	check functions:
		PQClear
		PQIsEmpty
		PQSize
		PQEnqueue
*/
void TestScenario4()
{
	pq_t *pq = PQCreate(CmpStrings);

	char data[][BUFFER_SIZE] = {"11", "1", "12", "0", "20", "011", "11"};
	size_t i = 0;

	printf("\n	TestScenario4\n");
	PrintInBlue("Enqueue,");
	for ( ; i < SizeOfArr(data); i++)
	{
		PQEnqueue(pq, data[i]);
	}

	CheckCount(PQSize(pq), SizeOfArr(data));

	PrintInBlue("Clear, ");
	PQClear(pq);
	CheckCount(PQSize(pq), 0);
	CheckIsEmpty(PQIsEmpty(pq), 1);

	PrintInBlue("Enqueue one element,");
	PQEnqueue(pq, data[0]);
	CheckCount(PQSize(pq), 1);
	CheckIsEmpty(PQIsEmpty(pq), 0);

	PQDestroy(pq);
}





/*create a priority_queue contains struct human, enqueue 5 elements
	 the queue is prioritize according to the name of the human.
	 remove specific element - if the age is equal.
	 check function:
		PQEraseIfMatch
	*/
void TestScenario5()
{
	pq_t *pq = PQCreate(CmpHumans);

	human data[] = {{15, "Hila"}, {18, "Avi"}, {27, "Shira"}, {30, "Ariel"}, {18, "Reut"}, {10, "Dan"}};
	human expected_data[] = {{30, "Ariel"}, {10, "Dan"}, {15, "Hila"}, {18, "Reut"}, {27, "Shira"}};
	size_t i = 0;

	printf("\n	TestScenario5\n");
	PrintInBlue("Enqueue, ");
	for ( ; i < SizeOfArr(data); i++)
	{
		PQEnqueue(pq, &data[i]);
	}

	CheckCount(PQSize(pq), SizeOfArr(data));

	PrintInBlue("PQEraseIfMatch:");
	CheckHumansEquals(PQEraseIfMatch(pq, &data[4], IsAgeEqual), &data[1]);
	CheckCount(PQSize(pq), SizeOfArr(expected_data));

	i = 0;
	PrintInBlue("Is queue is as expected:\n");
	while (!PQIsEmpty(pq))
	{
		PrintInBlue("Peek, Dequeue:");
		CheckHumansEquals(PQPeek(pq), &expected_data[i++]);
		PQDequeue(pq);
	}

	CheckIsEmpty(PQIsEmpty(pq), 1);
	PQDestroy(pq);
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


