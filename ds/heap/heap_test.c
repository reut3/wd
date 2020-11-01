#include <stdio.h>      /* printf */
#include <time.h>       /* time */
#include <stdlib.h>     /* srand rand */

#include "heap.h"

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

void PrintInYellow(const char *str){
	printf("\033[01;33m%s\033[0m",str);
}


/****************   Macros ans static global variable    *******************/

#define SizeOfArr(arr) (sizeof(arr)/sizeof(arr[0]))
#define VOID_POINTER_TO_NUM(num) (*(int *)(&num))
#define NUM_TO_VOID_POINTER(num) (*(void **)&(num))

static size_t failures = 0;
static size_t tests = 0;



/****************   Help function    *******************/

void TestsStas()
{
	printf("\n%ld ERRORS FROM %ld TESTS\n", failures, tests);
}

int CmpIntsMinHeap(const void *s1, const void *s2)
{
    int num1 = VOID_POINTER_TO_NUM(s1);
    int num2 = VOID_POINTER_TO_NUM(s2);

    return (num1 - num2);
}

int CmpIntsMaxHeap(const void *s1, const void *s2)
{
    int num1 = VOID_POINTER_TO_NUM(s1);
    int num2 = VOID_POINTER_TO_NUM(s2);

    return (num2 - num1);
}

int IsMatch(const void *s1, const void *s2)
{
    int num1 = VOID_POINTER_TO_NUM(s1);
    int num2 = VOID_POINTER_TO_NUM(s2);

    return (num1 == num2);
}


void CheckIsEmpty(heap_t *heap, int expected)
{
	PrintInBlue("IsEmpty:");
	if (HeapIsEmpty(heap) != expected)
	{
		PrintInRed();
		printf("Failed\n");
		ResetColor();
		failures++;
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}

	tests++;
}

void CheckSize(heap_t *heap, size_t expected_size)
{
    size_t size = HeapGetSize(heap);
	PrintInBlue("Size:");
	if (size != expected_size)
	{
		PrintInRed();
		printf("Failed got= %ld, expected= %ld\n", size, expected_size);
		ResetColor();
		failures++;
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}

	tests++;
}

void CheckTop(heap_t *heap, int expected_num)
{
    void *data = HeapPeek(heap);
    int num = VOID_POINTER_TO_NUM(data);

	PrintInBlue("CheckTop:");
	if (num != expected_num)
	{
		PrintInRed();
		printf("Failed got= %d, expected= %d\n", num, expected_num);
		ResetColor();
		failures++;
	}
	else
	{
		PrintInGreen();
		printf("Pass\n");
		ResetColor();
	}

	tests++;
}



/****************   Scenarios functions - black box tests    *******************/

/* 
	Test1- create empty heap and destroy it
*/
void CreateDestroyTest()
{
	heap_t *heap = HeapCreate(CmpIntsMinHeap);

	printf("\nTest 1\n");
	CheckIsEmpty(heap, 1);
	CheckSize(heap, 0);

	HeapDestroy(heap);
}


/* 
	Test2- create heap, insert 9 numbers, 
    check size is_empty and peek are as expected
    push new num that is smaller then root,
    check is the new root using peek
*/
void PushElementAndCheckPeekTest()
{
	heap_t *heap = HeapCreate(CmpIntsMinHeap);
    int data[] = {3,7,1,5,9,8,6,2,4};
    size_t i = 0 ;
    int insert_new_min = -5;

	printf("\nTest 2\n");

    PrintInBlue("Push\n");
	for ( ; i < SizeOfArr(data); i++)
	{
		HeapPush(heap, NUM_TO_VOID_POINTER(data[i]));
	}

	CheckIsEmpty(heap, 0);
	CheckSize(heap, SizeOfArr(data));
    CheckTop(heap, 1);

    PrintInBlue("Push\n");
    HeapPush(heap, NUM_TO_VOID_POINTER(insert_new_min));
    CheckSize(heap, SizeOfArr(data) + 1);
    CheckTop(heap, insert_new_min);

	HeapDestroy(heap);
}


/* 
	Test3- create heap, insert 8 numbers, 
    check peek and pop until heap is empty
    when heap is empty push one element and check
    peek size and is_empty
*/
void PushElementAndPopAllTest()
{
	heap_t *heap = HeapCreate(CmpIntsMinHeap);
    int data[] = {23, 67, 101, 45, 32, 11, 6, 90};
    int expected_data[] = {6, 11, 23, 32, 45, 67, 90, 101};

    size_t i = 0 ;
    int insert_new_min = 100;

	printf("\nTest 3\n");

    PrintInBlue("Push\n");
	for ( ; i < SizeOfArr(data); i++)
	{
		HeapPush(heap, NUM_TO_VOID_POINTER(data[i]));
	}

	CheckIsEmpty(heap, 0);
	CheckSize(heap, SizeOfArr(data));
    
    for (i = 0; !HeapIsEmpty(heap); i++)
    {
        CheckTop(heap, expected_data[i]);
        PrintInBlue("Pop ");
        HeapPop(heap);
    }

    CheckIsEmpty(heap, 1);
	CheckSize(heap, 0);

    PrintInBlue("Push\n");
    HeapPush(heap, NUM_TO_VOID_POINTER(insert_new_min));
    CheckSize(heap, 1);
    CheckTop(heap, insert_new_min);

	HeapDestroy(heap);
}




/*
	Test4- stress test. create heap, push 10000 elements
    remove half random elements
    check top is as expected
 */
#define NUM_OF_ELEMENTS 10000
void InsertDataAndRemoveStressTest()
{
	heap_t *heap = HeapCreate(CmpIntsMaxHeap);
    int data[NUM_OF_ELEMENTS] = {0};
    time_t t = time(0);
	size_t i = 0;

	printf("\nTest 4\n");
    
    srand(t); 

    PrintInBlue("Push\n");
	for ( ; i < SizeOfArr(data); i++)
	{
        int position = rand() % NUM_OF_ELEMENTS;

        while (data[position] == 1)
        {   
            position = rand() % NUM_OF_ELEMENTS;
        }
        
		HeapPush(heap, NUM_TO_VOID_POINTER(position));
        data[position] = 1;
	}

    CheckSize(heap, SizeOfArr(data));
    CheckIsEmpty(heap, 0);

    PrintInBlue("Remove\n");
    for (i = 0; i < NUM_OF_ELEMENTS / 2; i++)
    {
        int position = rand() % NUM_OF_ELEMENTS;
        
        while (data[position] == -1)
        {
            position = rand() % NUM_OF_ELEMENTS;
        }

        HeapRemove(heap, NUM_TO_VOID_POINTER(position), IsMatch);
        data[position] = -1;
    }

    CheckIsEmpty(heap, 0);
    CheckSize(heap, SizeOfArr(data) / 2);

    for (i = SizeOfArr(data) - 1 ; i > 0; i--)
	{
        if (data[i] == 1)
        {   
            CheckTop(heap, i);
            break;
        }
	}

	HeapDestroy(heap);
}





/**********************  Main functions     *************************/

void WhiteBoxTests()
{
	PrintInYellow("\n	*** White Box Tests ***\n");
}

void BlackBoxTests()
{
	PrintInYellow("\n	*** Black Box Tests ***\n");
	CreateDestroyTest();
    PushElementAndCheckPeekTest();
    PushElementAndPopAllTest();
    InsertDataAndRemoveStressTest();
}

void AllTests()
{
	BlackBoxTests();
	/* WhiteBoxTests(); */
	TestsStas();
}

int main()
{  
	AllTests();
	
	return 0;
}