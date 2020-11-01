#include <stdlib.h>     /* malloc free */
#include <assert.h>     /* assert */

#include "heap.h"
#include "vector.h"

/*********************** Struct ***********************/
struct heap
{
    vector_t *vector;
    cmp_func_t cmp;
};

/******************************* macros *******************************/
#define INIT_CAPACITY 10
#define PARENT(i) (((i) - 1) / 2)
#define LEFT_CHILD(i) ((i) * 2 + 1)
#define RIGHT_CHILD(i) ((i) * 2 + 2)

enum status{SUCCESS, FAIL};


/*********************** Static functions declarations ***********************/
static void HeapifyUp(heap_t *heap, size_t index);
static void HeapifyDown(heap_t *heap, size_t index);
static void SwapData(heap_t *heap, size_t index1, size_t index2);

/*********************** Functions implementions ***********************/


heap_t *HeapCreate(cmp_func_t compare)
{
    heap_t *heap = malloc(sizeof(heap_t));

    assert(compare);

    if (!heap)
    {
        return NULL;
    }

    heap->cmp = compare;
    heap->vector = VectorCreate(INIT_CAPACITY);

    if (!heap->vector)
    {
        free(heap);
        heap = NULL;
    }

    return heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(heap);

    VectorDestroy(heap->vector);
    free(heap);
    heap = NULL;
}


int HeapPush(heap_t *heap, void *data)
{
    int status = 0; 

    assert(heap);

    status = VectorPushBack(heap->vector, data);
    if (status != SUCCESS)
    {
        return FAIL;
    }

    HeapifyUp(heap, VectorGetSize(heap->vector) - 1);

    return SUCCESS;
}


void HeapPop(heap_t *heap)
{
    size_t size = 0;
    void *data = NULL;

    assert(heap);

    size = VectorGetSize(heap->vector);
    data = VectorGetElement(heap->vector, size - 1);
    VectorSetElement(heap->vector, 0, data);
    VectorPopBack(heap->vector);
    HeapifyDown(heap, 0);
}


void *HeapPeek(const heap_t *heap)
{
    assert(heap);

    return VectorGetElement(heap->vector, 0);
}

size_t HeapGetSize(const heap_t *heap)
{
    assert(heap);

    return VectorGetSize(heap->vector);
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(heap);

    return (VectorGetSize(heap->vector) == 0);
}


void *HeapRemove(heap_t *heap, const void *element_to_cmp, 
					int (*is_match)(const void *data, const void *param))
{
    size_t i = 0;
    size_t size = 0;

    assert(heap);
    assert(is_match);

    for (size = VectorGetSize(heap->vector); i < size; i++)
    {
        void *data = VectorGetElement(heap->vector, i);

        if (is_match(data, element_to_cmp))
        {
            VectorSetElement(heap->vector, i,
                    VectorGetElement(heap->vector, size - 1));
            VectorPopBack(heap->vector);
            HeapifyUp(heap, i);
            HeapifyDown(heap, i);

            return data;
        }
    }

    return NULL;
}



/*********************** Static functions implementions ***********************/

static void HeapifyUp(heap_t *heap, size_t index)
{
    size_t parent_index = PARENT(index);

    if (index == 0)
    {
        return;
    }

    if (heap->cmp(VectorGetElement(heap->vector, index),
                    VectorGetElement(heap->vector, parent_index)) < 0)
    {
        SwapData(heap, parent_index, index);
        HeapifyUp(heap, parent_index);
    }
}


static void SwapData(heap_t *heap, size_t index1, size_t index2)
{
    void *data = VectorGetElement(heap->vector, index1);

    VectorSetElement(heap->vector, index1,
                    VectorGetElement(heap->vector, index2));
    VectorSetElement(heap->vector, index2, data);
}


static void HeapifyDown(heap_t *heap, size_t index)
{
    size_t size = VectorGetSize(heap->vector);
    size_t left_child_index = LEFT_CHILD(index);
    size_t right_child_index = RIGHT_CHILD(index);

    if (right_child_index < size)
    {
        size_t max_index = index;

        if (heap->cmp(VectorGetElement(heap->vector, left_child_index),
                        VectorGetElement(heap->vector, max_index)) < 0)
        {
            max_index = left_child_index;
        }

        if (heap->cmp(VectorGetElement(heap->vector, right_child_index),
                        VectorGetElement(heap->vector, max_index)) < 0)
        {
            max_index = right_child_index;
        }

        if (max_index != index)
        {
            SwapData(heap, max_index, index);
            HeapifyDown(heap, max_index);
        }
    }
    else if (left_child_index < size)
    {
        if (heap->cmp(VectorGetElement(heap->vector, left_child_index),
                            VectorGetElement(heap->vector, index)) < 0)
        {
            SwapData(heap, left_child_index, index);
            HeapifyDown(heap, left_child_index);
        }
    }
}
