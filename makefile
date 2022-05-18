$ (CC)=gcc
mcs_alloc -lm : task_allocation_main.o print_task_list.o utilisation.o taskallocation.o
					$(CC) task_allocation_main.o print_task_list.o utilisation.o taskallocation.o -o mcs_alloc -lm

task_allocation_main.o: task_allocation_main.c TaskAllocation.h
					$(CC) -c task_allocation_main.c

taskallocation.o: taskallocation.c TaskAllocation.h
					$(CC) -c taskallocation.c

print_task_list.o: print_task_list.c TaskAllocation.h
					$(CC) -c print_task_list.c

utilisation.o: utilisation.c TaskAllocation.h
					$(CC) -c utilisation.c

clean:
	rm -f *.o mcs_alloc -lm