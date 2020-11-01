#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h> /*size_t*/


typedef struct vector vector_t;


/*
	VectorCreate creates a vector with capacity param,
	and returns pointer to vector or NULL for failure.
	Capacity 0 is undefined.
*/
vector_t *VectorCreate(size_t capacity);

void VectorDestroy(vector_t *vector);


/*
	VectorPushBack push element to the end of vector,
	if vector is full, capacity will increase.
	the function returns 0 for success.

*/
int VectorPushBack(vector_t *vector, void *element);


/*
	VectorPopBack delete the last element in the vector.
	undefined for vector with size 0.
*/
void VectorPopBack(vector_t *vector);

size_t VectorGetSize(const vector_t *vector);


size_t VectorGetCapacity(const vector_t *vector);

/*
	VectorGetElement returns element at index in vector.
	index starts from 0, udnefined for index > capacity.

*/
void *VectorGetElement(const vector_t *vector, size_t index);

/*
	VectorGetElement setes element at index in vector to element.
	index starts from zero, undefined for index >= capacity.
*/
void VectorSetElement(vector_t *vector, size_t index, void *element);


/* Return non zero value if failed */
int VectorShrinkToFit(vector_t *vector);

/* 
	VectorResize resizes capacity to new_capacity.
	Returns 0 for success.
*/
int VectorResize(vector_t *vector, size_t new_capacity);

#endif /* __VECTOR_H__ */
