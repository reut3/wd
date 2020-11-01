# $@ -left side of :
# $^ -right side of :

# export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

GD=gcc -ansi -pedantic-errors -Wall -Wextra -g
DS_DIR=../ds/
export1 = export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH

scheduler.out: scheduler_test.o libscheduler.so
	$(GD) $< -o $@ -L. -lscheduler


scheduler_test.o: scheduler.h scheduler_test.c $(DS_DIR)uid/uid.h
	$(GD) -I $(DS_DIR)uid/ -c scheduler_test.c


# libscheduler.so  -> scheduler shared library
libscheduler.so: scheduler.o heap_pq.o heap.o vector.o uid.o task.o
	$(GD) -shared -L./ $^ -o $@ 

# scheduler
scheduler.o: scheduler.c scheduler.h $(DS_DIR)uid/uid.h $(DS_DIR)heap_pq/heap_pq.h
	$(GD) -fPIC -I $(DS_DIR)task/ -I $(DS_DIR)uid/ -I $(DS_DIR)heap_pq/ -c $<
	

# task
task.o: $(DS_DIR)task/task.c $(DS_DIR)task/task.h $(DS_DIR)uid/uid.h
	$(GD) -fPIC -I $(DS_DIR)uid/ -c $<

# uid
uid.o: $(DS_DIR)/uid/uid.c $(DS_DIR)/uid/uid.h 
	$(GD) -fPIC -c $<


#priority queue heap
heap_pq.o: $(DS_DIR)/heap_pq/heap_pq.c $(DS_DIR)/heap_pq/heap_pq.h $(DS_DIR)heap/heap.h 
	$(GD) -fPIC -I $(DS_DIR)heap/ -c $<

#heap
heap.o: $(DS_DIR)heap/heap.c $(DS_DIR)heap/heap.h 
	$(GD) -fPIC -I $(DS_DIR)vector/ -c $<

#vector
vector.o: $(DS_DIR)vector/vector.c $(DS_DIR)vector/vector.h 
	$(GD) -fPIC -c $<


.PHONY: clean
clean:
	rm *.o  *.out

