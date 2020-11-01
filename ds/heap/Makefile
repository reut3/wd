TARGET= a.out
CC= gcc
CFLAGS= -ansi -pedantic-errors -Wall -Wextra -g

OBJS_LIST=heap.o heap_test.o
OBJDIR=Objs
OBJS=$(addprefix $(OBJDIR)/,$(OBJS_LIST))

OTHER_OBJS_LIST=vector.o
INCLUDE_PATH=../vector/
OTHEROBJDIR=OtherObjs
OTHEROBJS=$(addprefix $(OTHEROBJDIR)/,$(OTHER_OBJS_LIST))


all: $(OBJS) $(OTHEROBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) -lm



$(OBJDIR)/%.o: %.c
	$(CC) -I $(INCLUDE_PATH) $(CFLAGS) -c -o $@ $<
	$(CC) -I $(INCLUDE_PATH) -MM $*.c -MT"$@" -MF $(OBJDIR)/$*.d

$(OBJS): |$(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

-include $(OBJDIR)/$(OBJS:.o=.d)




# other objs 
$(OTHEROBJDIR)/%.o: $(INCLUDE_PATH)%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	$(CC) -I $(INCLUDE_PATH) -MM $(INCLUDE_PATH)$*.c -MT"$@" -MF $(OTHEROBJDIR)/$*.d


$(OTHEROBJS): |$(OTHEROBJDIR)

$(OTHEROBJDIR):
	mkdir $(OTHEROBJDIR)

-include $(OTHEROBJDIR)/$(OTHEROBJS:.o=.d)





.PHONY : clean
clean:
	rm -rf *.o *.out $(OBJDIR) $(OTHEROBJDIR)





