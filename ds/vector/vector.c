#include <assert.h>		/*assert*/
#include <stdlib.h>		/*free malloc*/
#include "vector.h"

struct vector
{
	size_t capacity;
	size_t size;
	void **base_pointer;
};


enum Status{SUCCESS, FAILED};


vector_t *VectorCreate(size_t capacity)
{
	vector_t *vector = (vector_t *) malloc(sizeof(vector_t));

	if (vector == NULL)
	{
		return NULL;
	}

	vector->base_pointer = (void **) malloc(sizeof(void *) * capacity);
	if (vector->base_pointer == NULL)
	{
		return NULL;
	}

	vector->size = 0;
	vector->capacity = capacity;

	return vector;
}

void VectorDestroy(vector_t *vector)
{
	free(vector->base_pointer);
	vector->base_pointer = NULL;
	free(vector);
	vector = NULL;
}


int VectorPushBack(vector_t *vector, void *element)
{
	assert(vector);

	if (vector->size == vector->capacity)
	{
		void **realloc_saftey_p = (void **)
			realloc(vector->base_pointer, vector->capacity * 2 * sizeof(void *));

		if (realloc_saftey_p == NULL)
		{
			return FAILED;
		}

		vector->base_pointer = realloc_saftey_p;
		vector->capacity = vector->capacity * 2;
	}

	*(vector->base_pointer + vector->size) = element;
	vector->size++;

	return SUCCESS;
}


void VectorPopBack(vector_t *vector)
{
	assert(vector && vector->size != 0);

	vector->size--;
}

void *VectorGetElement(const vector_t *vector, size_t index)
{
	assert(vector && index < vector->capacity);

	return (*(vector->base_pointer + index));
}

void VectorSetElement(vector_t *vector, size_t index, void *element)
{
	assert(vector && index < vector->capacity);

	*(vector->base_pointer + index) = element;
}


size_t VectorGetSize(const vector_t *vector)
{
	assert(vector);

	return (vector->size);
}


size_t VectorGetCapacity(const vector_t *vector)
{
	assert(vector);

	return (vector->capacity);
}


int VectorResize(vector_t *vector, size_t new_capacity)
{
	void **realloc_saftey_p = (void **)
		realloc(vector->base_pointer, new_capacity * sizeof(void *));

	assert(vector && new_capacity != 0);

	if (realloc_saftey_p == NULL)
	{
		return FAILED;
	}

	vector->base_pointer = realloc_saftey_p;
	vector->capacity = new_capacity;

	return SUCCESS;

}


int VectorShrinkToFit(vector_t *vector)
{
	void **realloc_saftey_p = (void **)
		realloc(vector->base_pointer, sizeof(void *) * vector->size);

	assert(vector && vector->size != 0);

	if (realloc_saftey_p == NULL)
	{
		return FAILED;
	}

	vector->base_pointer = realloc_saftey_p;
	vector->capacity = vector->size;

	return SUCCESS;
}