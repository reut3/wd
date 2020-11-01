#include <stdlib.h>     /*free malloc*/
#include <assert.h>     /*assert*/

#include "heap_pq.h"
#include "heap.h"



struct pqueue
{
   heap_t *heap;
};

enum status{SUCCESS, FAIL};


/*********************** Functions implementions ***********************/
pq_t *PQCreate(int (*compare)(const void *data1, const void *data2))
{
    pq_t *pq = NULL;

    assert(compare);

    pq = (pq_t *)malloc(sizeof(pq_t));
    if (pq == NULL)
    {
        return NULL;
    }

    pq->heap = HeapCreate(compare);
    if (pq->heap == NULL)
    {
        free(pq);
        pq = NULL;
    }

    return pq;
}

void PQDestroy(pq_t *queue)
{
    assert(queue);
    assert(queue->heap);

    HeapDestroy(queue->heap);
    free(queue);
    queue = NULL;
}

/* inserting */
int PQEnqueue(pq_t *queue, void *data)
{
    assert(queue);
    assert(queue->heap);

    return HeapPush(queue->heap, data);
}


void PQDequeue(pq_t *queue)
{
    assert(queue);
    assert(queue->heap);

    HeapPop(queue->heap);
}

int PQIsEmpty(const pq_t *queue)
{
    assert(queue);
    assert(queue->heap);

    return HeapIsEmpty(queue->heap);
}

size_t PQSize(const pq_t *queue)
{
    assert(queue);
    assert(queue->heap);

    return HeapGetSize(queue->heap);
}


void *PQPeek(const pq_t *queue)
{
    assert(queue);
    assert(queue->heap);

    return HeapPeek(queue->heap);
}

void PQClear(pq_t *queue)
{    
    assert(queue);
    assert(queue->heap);

    while (!HeapIsEmpty(queue->heap))
    {
       HeapPop(queue->heap);
    }
}

void *PQEraseIfMatch(pq_t *queue, void *element_to_cmp,
                            int (*compare)(const void *queue_data, const void *input_data))
{
    assert(queue);
    assert(queue->heap);
    assert(compare);
    assert(element_to_cmp);

    return HeapRemove(queue->heap, element_to_cmp, compare);
}
