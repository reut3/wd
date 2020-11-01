#include "vector.h"
#include <stdio.h>		/*printf*/

/*create a vector and destroy it*/
void Test1()
{
	vector_t *vector = VectorCreate(5);
	VectorDestroy(vector);

	printf("\nTest1\n");

}

/*create a vector push element and get it*/
void Test2()
{
	vector_t *vector = VectorCreate(5);
	int x = 1;

	printf("\nTest2\n");
	VectorPushBack(vector, &x);
	printf("element should be 1==%d\n", *((int *)VectorGetElement(vector, 0)));

	VectorDestroy(vector);
}


/* create vector size 2 push 4 elements and 
   check at every step size and capacity is correct
   also check set and get element*/
void Test3()
{
	vector_t *vector = VectorCreate(2);
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	printf("\nTest3");

	printf("size should be 0==%ld\n", VectorGetSize(vector));
	printf("capacity should be 2==%ld\n", VectorGetCapacity(vector));

	VectorPushBack(vector, &a);
	printf("size should be 1==%ld\n", VectorGetSize(vector));
	printf("capacity should be 2==%ld\n", VectorGetCapacity(vector));

	VectorPushBack(vector, &b);
	printf("size should be 2==%ld\n", VectorGetSize(vector));
	printf("capacity should be 2==%ld\n", VectorGetCapacity(vector));

	VectorPushBack(vector, &c);
	printf("size should be 3==%ld\n", VectorGetSize(vector));
	printf("capacity should be 4==%ld\n", VectorGetCapacity(vector));


	VectorSetElement(vector, 2, &d);
	printf("size should be 3==%ld\n", VectorGetSize(vector));
	printf("capacity should be 4==%ld\n", VectorGetCapacity(vector));
	printf("element should be 4==%d\n", *((int *)VectorGetElement(vector, 2)));

	VectorDestroy(vector);
}

/*create vector with capacity of 7, push 4 elements
 shrink to size and check size and capacity correctness*/
void Test4()
{
	vector_t *vector = VectorCreate(7);
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	printf("\nTest4\n");

	VectorPushBack(vector, &a);
	VectorPushBack(vector, &b);
	VectorPushBack(vector, &c);
	VectorPushBack(vector, &d);

	printf("size should be 4==%ld\n", VectorGetSize(vector));
	printf("capacity should be 7==%ld\n", VectorGetCapacity(vector));

	printf("status is=%d\n",  VectorShrinkToFit(vector));

	printf("size should be 4==%ld\n", VectorGetSize(vector));
	printf("capacity should be 4==%ld\n", VectorGetCapacity(vector));

	VectorDestroy(vector);
}

/*create vector with capacity 7, push 4 elements
  pop 4 times and check size is correct*/
void Test5()
{
	vector_t *vector = VectorCreate(7);
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	printf("\nTest5\n");

	VectorPushBack(vector, &a);
	VectorPushBack(vector, &b);
	VectorPushBack(vector, &c);
	VectorPushBack(vector, &d);

	
	printf("size should be 4==%ld\n", VectorGetSize(vector));
	printf("capacity should be 7==%ld\n", VectorGetCapacity(vector));
	VectorPopBack(vector);
	printf("size should be 3==%ld\n", VectorGetSize(vector));
	VectorPopBack(vector);
	printf("size should be 2==%ld\n", VectorGetSize(vector));
	VectorPopBack(vector);
	printf("size should be 1==%ld\n", VectorGetSize(vector));
	VectorPopBack(vector);
	printf("size should be 0==%ld\n", VectorGetSize(vector));

	VectorDestroy(vector);
}

/*create vector with capacity 4, push 4 elements
  resize capacity to 10 and check size, capacity and element
  are as expected*/
void Test6()
{
	vector_t *vector = VectorCreate(4);
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	printf("\nTest6\n");

	VectorPushBack(vector, &a);
	VectorPushBack(vector, &b);
	VectorPushBack(vector, &c);
	VectorPushBack(vector, &d);

	printf("size should be 4==%ld\n", VectorGetSize(vector));
	printf("capacity should be 4==%ld\n", VectorGetCapacity(vector));
	VectorResize(vector, 10);
	printf("size should be 4==%ld\n", VectorGetSize(vector));
	printf("capacity should be 10==%ld\n", VectorGetCapacity(vector));

	printf("element should be 1==%d\n", *((int *)VectorGetElement(vector, 0)));
	printf("element should be 2==%d\n", *((int *)VectorGetElement(vector, 1)));
	printf("element should be 3==%d\n", *((int *)VectorGetElement(vector, 2)));
	printf("element should be 4==%d\n", *((int *)VectorGetElement(vector, 3)));


	VectorDestroy(vector);
}


int main()
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	Test6();



	return 0;
}