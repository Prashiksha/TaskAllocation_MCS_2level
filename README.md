List of Files
    1. TaskAllocation.h
            Header file contaning global variables and structure for task and core parameters.

    2. task_allocation_main.c
            File that accepts input file as command line argument and reads input file parameters and stores them in structure. main() calls other functions for utilisation and task allocation.

    3. print_task_list.c
            Prints the values of task set and filled capacity of each core at the moment.

    4. taskallocation.c
            Contains functions for allocation of High mode and Low mode tasks.Seperate arrays are used to store Ui values for tasks of different criticality. 

            allocate_critical_task() allocates tasks those are in HI mode. The allocation is done by keeping in mind that Ui across all active cores is balanced. An array is maintained that keeps track about the core who was allocated the recent task (this is done to avoid clustering of HI mode tasks to a single core).

            allocation_LOmode() performs allocation in LO mode. It first recomputes LO mode Ui of HI criticality tasks and then allocates HI mode tasks by considering their LO mode Ui. Allocation of LO criticality tasks is done by minimising the difference between the maximum Ui and minimum Ui across all active cores. This is done by string values in intermediate array and finding the optimal combination of tasks allocation.

    5. utilisation.c
                Function to print the utilisation matrix, ie Ui at each level at each criticality. 

    6. TaskAllocationInput.txt
                Input text file containing number of total tasks in first line. Followed by parameters of task set in order:
                    Phase, Period, Criticality level, WCET for both modes

How to compile the program
    Write make command on terminal for compilation.

How to execute the program
    Give input in form of:
            ./mcs_alloc input_filename.txt

Output format:
    Initial Task Set with unallocated cores
    Utilisation values at each level
    Required Ui which is the worst case possible total Ui
    Available Cores
    Minimum cores required as per the calculated Ui
    Task Set after allocation in HI mode (core value for LO mode task will be -1)
    Capacity filled in each core after HI criticality tasks are being allocated
    Task Set after allocation in LO mode
    Capacity filled in each core after LO criticality tasks are being allocated


Next Step (Yet to be done)

    1. Test this program with complex task set.
    2. Finding example where a new core will be opened. Then test the reallocation function with min_core+1
    3. Writing the output to file instead of console.
