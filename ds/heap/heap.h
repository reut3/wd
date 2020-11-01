#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /* size_t */

typedef struct heap heap_t;

typedef int (*cmp_func_t)(const void *data1, const void *data2);

/*
	HeapCreate recives compare function.
	return value of compare function should be:
	if data1's priority > datas's priotiry returns negative number,
	if data1's priority < datas's priotiry returns positive number,
	and zero (0) if equal.
	HeapCreate returns pointer to heap or NULL for failure.
*/
heap_t *HeapCreate(cmp_func_t compare);

void HeapDestroy(heap_t *heap);

/* 	
	HeapPush push data to heap,
	the functions uses cmp_func_t to find the right position for data.
	Returns 0 for success.
*/
int HeapPush(heap_t *heap, void *data);

/* 
	HeapPop removes the top of the heap
 	Undefined for empty heap.
*/
void HeapPop(heap_t *heap);

/* 	
	HeapPeek returns the top of the heap.
 	Undefined for empty heap.
*/
void *HeapPeek(const heap_t *heap);

/* 
	HeapRemove removes the first element in heap that is_match function
	returns a match (= 1) when comparing with element_to_cmp.
	if element is found HeapRemove returns the found element or NULL otherwise.
*/
void *HeapRemove(heap_t *heap, const void *element_to_cmp, 
					int (*is_match)(const void *data, const void *param));


size_t HeapGetSize(const heap_t *heap);


int HeapIsEmpty(const heap_t *heap);



#endif /* __HEAP__H__ */