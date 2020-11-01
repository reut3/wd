gd=gcc -ansi -pedantic-errors -Wall -Wextra -g
flags= -pthread

SCHEDULER_DIR=scheduler/
UID_DIR=ds/uid/

# $@ -left side of :
# $^ -right side of :

# export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

a.out: watchdog_test.o libwatchdog.so | $(EXPORT_LIB) watchdog.o 
	$(gd) $(flags) -L ./ $^ -o $@

watchdog.o: watchdog.c libwatchdog.so
	$(gd) $(flags) -L ./ $^ -o $@

watchdog_mmi_dnr.o: watchdog_mmi_dnr.c watchdog_mmi_dnr.h watch_module.h
	$(gd) $(flags) -fPIC -c $<

watch_module.o: watch_module.c watch_module.h $(SCHEDULER_DIR)scheduler.h
	$(gd) -fPIC -I $(UID_DIR) -I $(SCHEDULER_DIR) -c $< 

libwatchdog.so: watch_module.o watchdog_mmi_dnr.o $(SCHEDULER_DIR)libscheduler.so
	$(gd) -shared -Wl,-rpath=./ -o $@ $^


watchdog_test.o: watchdog_test.c watchdog_mmi_dnr.h
	$(gd) -c $<

$(EXPORT_LIB):
	export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH



.PHONY: clean
clean:
	rm *.o  *.out libwatch_module.so

