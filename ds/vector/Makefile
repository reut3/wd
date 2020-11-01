# $@ -left side of :
# $^ -right side of :

GD=gcc -ansi -pedantic-errors -Wall -Wextra -g


#vector
a.out: vector.o vector_test.o
	$(GD) $^ -o $@

#vector
vector.o: vector.c vector.h 
	$(GD) -c $<

#vector_test
vector_test.o: vector_test.c vector.h 
	$(GD) -c $<




.PHONY: clean
clean:
	rm *.o  *.out

