#include "TaskAllocation.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void print_task_list(task_set_struct *task_set)
{
    /*  Printing to console
        Modify to print in file
    */
    int i, j, total_tasks;
    task *task_list;

    total_tasks = task_set->total_tasks;
    task_list = task_set->task_list;

    printf("\nTaskset:\n");

    for (i = 0; i < total_tasks; i++)
    {
        printf("Task: %d | core: %d | crit_level: %d | phase: %.2lf | rel_deadline: %.2lf | virt_deadline: %.2lf | ",
                i,
                task_list[i].core,
                task_list[i].criticality_lvl,
                task_list[i].phase,
                task_list[i].relative_deadline,
                task_list[i].virtual_deadline);
        printf("WCET: ");
        for (j = 0; j < MAX_CRITICALITY_LEVELS; j++)
        {
            printf("%.2lf ", task_list[i].WCET[j]);
        }
        printf(" | Util: ");
        for (j = 0; j < MAX_CRITICALITY_LEVELS; j++)
        {
            printf("%.3f ", task_list[i].util[j]);
        }
        printf("\n");
    }
    printf("\n");

/*Printing Util of each core*/
    for(int i=0;i<min_cores;i++)
    {
        printf("Filled Capacity (Ui*100) of core %d : %lf\n", i, cores[i].Ui);
    }

}
