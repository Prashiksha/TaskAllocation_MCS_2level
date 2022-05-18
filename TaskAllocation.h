#ifndef _TASK_ALLOCATION

#define _TASK_ALLOCATION
#define _POSIX_SOURCE

/*Global Variables*/
#define MAX_CRITICALITY_LEVELS 2
#define LOW 0
#define HIGH 1
#define NUM_CORES 4

int min_cores;

/*Global Structures*/

typedef struct task
{
    int task_number;
    double phase;  //Assuming phase=0
    double period;
    double relative_deadline;
    int criticality_lvl;
    double WCET[MAX_CRITICALITY_LEVELS]; //execution time for each criticality level
    double virtual_deadline;
    int job_number;
    int core;   //to which core task will be assigned
    double *util;
    double *exec_times;
}task;

typedef struct task_set_struct
{
    int total_tasks;
    task *task_list;
} task_set_struct;

struct core_parameters
{
    double Ui; //Utilization for each core
    int capacity;
    int curr_criticality_lvl;
    int no_task_allocated;     //Modify it to max tasks available in task set

}cores[NUM_CORES]; 



/*Functions*/
extern void allocate_critical_task(task_set_struct *task_set, double HI_Ui[] ,int number_of_cores, int number_of_tasks, int taskID[]);
extern void allocation_LOmode(task_set_struct *task_set, double LO_Ui[] ,int number_of_cores, int number_of_tasks, int taskID[]);
extern void print_task_list(task_set_struct *task_set);
extern void find_total_utilisation(int total_tasks, task *tasks_list, double total_utilisation[][MAX_CRITICALITY_LEVELS]);
extern float print_calculate_Ui(double total_utilisation[][MAX_CRITICALITY_LEVELS], float reqUi);
extern void task_allocation(task_set_struct *task_set, int min_cores);

#endif