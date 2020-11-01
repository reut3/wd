CC =gcc
CFLAGS =-ansi -pedantic-errors -Wall -Wextra -g

OBJS_LIST =heap.o vector.o heap_pq.o heap_pq_test.o
TARGET =a.out
FOLDER1 =../heap/
FOLDER2 =../vector/

# $@ -left side of :
# $^ -right side of :



all: $(OBJS_LIST)
	$(CC) $(CFLAGS) $^ -o $(TARGET) -lm


heap_pq_test.o: heap_pq_test.c heap_pq.h
	$(CC) $(CFLAGS) -c $< 

heap_pq.o: heap_pq.c heap_pq.h $(FOLDER1)heap.h
	$(CC) $(CFLAGS) -I $(FOLDER1) -I $(FOLDER2) -c $< 



heap.o: $(FOLDER1)heap.c $(FOLDER1)heap.h
	$(CC) $(CFLAGS) -I $(FOLDER2) -c $< 

vector.o: $(FOLDER2)vector.c $(FOLDER2)vector.h
	$(CC) $(CFLAGS) -I $(FOLDER2) -c $< 





.PHONY : clean
clean:
	rm *.o *.out
