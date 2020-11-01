#ifndef __HEAP_PQ_H__
#define __HEAP_PQ_H__

#include <stddef.h> /*size_t*/

typedef struct pqueue pq_t;


/*
    PQCreate create priority queue, the function recives cmp function.
    Return value of cmp function should be:
    if data1's priority > datas's priotiry returns negative number,
    if data1's priority < datas's priotiry returns positive number,
    and zero (0) if equal.
    PQCreate returns pointer to priority queue or NULL for failure.
*/
pq_t *PQCreate(int (*cmp)(const void *data1, const void *data2));

void PQDestroy(pq_t *queue);

/* 
    PQEnqueue inserts data to pq in the right position
    ambiguous if data is NULL (for erase func),
    returns 0 for success.
*/
int PQEnqueue(pq_t *queue, void *data);

/* 
    PQDequeue removes the first element in the queue
    undefined for an empty queue.
*/
void PQDequeue(pq_t *queue);


int PQIsEmpty(const pq_t *queue);

size_t PQSize(const pq_t *queue);

/*
    PQPeek returns the first element in queue,
    undefined for an empty queue.
*/
void *PQPeek(const pq_t *queue);

/*
    PQClear function will remove all elements in the queue
*/
void PQClear(pq_t *queue);

/* 
    PQErase removes the first elements that is_match function
    returns a match (=1) for when recive element from the queue and element_to_cmp,
    is_match returns zero for inequality,
    if element is found the function returns the element and erase it from the queue,
    otherwise returns NULL.
*/
void *PQEraseIfMatch(pq_t *queue, void *element_to_cmp,
        int is_match(const void *queue_data, const void *input_data));

#endif /* __HEAP_PQ_H__ */
