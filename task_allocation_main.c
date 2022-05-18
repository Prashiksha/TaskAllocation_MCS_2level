#include "TaskAllocation.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include <stdbool.h>

float reqUi=0.00;
int min_cores;

int main(int argc, char* argv[])
{
    if(argc!=2)  
    {
        printf("Please give the input file name\n");
        return 0;
    }

    //task_set_struct *task_set = get_taskset();
    int num_task, criticality_lvl;
    int tasks; //total number of tasks
    float reqUi;

    FILE *input, *exec;
    input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("ERROR: Cannot open input file. Format of execution is ./test input.txt\n");
        return 0;
    }

    exec = fopen(argv[1], "r");
    task_set_struct *task_set = (task_set_struct *)malloc(sizeof(task_set_struct));

    //Number of task_list
    fscanf(input, "%d", &(task_set->total_tasks));
    tasks = task_set->total_tasks;
    //printf("Number of tasks :%d\n",tasks);
    task_set->task_list = (task *)malloc(sizeof(task) * tasks);

    for (num_task = 0; num_task < tasks; num_task++)
    {
        fscanf(input, "%lf%lf%d", &task_set->task_list[num_task].phase, &task_set->task_list[num_task].relative_deadline, &task_set->task_list[num_task].criticality_lvl);

        task_set->task_list[num_task].period = task_set->task_list[num_task].relative_deadline;
        task_set->task_list[num_task].task_number = num_task;
        task_set->task_list[num_task].job_number = 0;
        task_set->task_list[num_task].util = (double *)malloc(sizeof(double) * MAX_CRITICALITY_LEVELS);
        task_set->task_list[num_task].core = -1; //No core allocated initially

        for (criticality_lvl = 0; criticality_lvl < MAX_CRITICALITY_LEVELS; criticality_lvl++)
        {
            fscanf(input, "%lf", &task_set->task_list[num_task].WCET[criticality_lvl]);
            task_set->task_list[num_task].util[criticality_lvl] = (double)task_set->task_list[num_task].WCET[criticality_lvl] / (double)task_set->task_list[num_task].period;
        }

        int num_jobs;
        fscanf(exec, "%d", &num_jobs);
        task_set->task_list[num_task].exec_times = malloc(sizeof(double)*num_jobs);
        for(int i=0; i<num_jobs; i++)
        {
            fscanf(exec, "%lf ", &task_set->task_list[num_task].exec_times[i]);
        }

    }
        print_task_list(task_set);

        double total_utilisation[MAX_CRITICALITY_LEVELS][MAX_CRITICALITY_LEVELS];
        int total_tasks = task_set->total_tasks;
        task *tasks_list = task_set->task_list;

        find_total_utilisation(total_tasks, tasks_list, total_utilisation);
        float Ui=print_calculate_Ui(total_utilisation,reqUi);

        

/******************************TASK ALLOCATION******************************/

        printf("Available Cores : %d\n",NUM_CORES);
        min_cores=(int)(ceil(Ui));  //typecasting ceil value to integer
        printf("Minimum Cores required is : %d\n",min_cores);
        task_allocation(task_set,min_cores);

return 0;
}