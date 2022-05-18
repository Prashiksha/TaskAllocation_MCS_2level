#include "TaskAllocation.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include <stdbool.h>


void find_total_utilisation(int total_tasks, task *tasks_list, double total_utilisation[][MAX_CRITICALITY_LEVELS])
{
    int i, l, k;
    for (l = 0; l < MAX_CRITICALITY_LEVELS; l++)
    {
        for (k = 0; k < MAX_CRITICALITY_LEVELS; k++)
        {
            total_utilisation[l][k] = 0;
            for (i = 0; i < total_tasks; i++)
            {
                if (tasks_list[i].criticality_lvl == l)
                {
                    total_utilisation[l][k] += tasks_list[i].util[k];
                }
            }
        }
    }
}

/*
    Function to print the utilisation matrix. 
    ie Ui at each level at each criticality
*/
float print_calculate_Ui(double total_utilisation[][MAX_CRITICALITY_LEVELS], float reqUi)
{
    int i, j;
    printf("\nUtilisation Values:\n");
    for (i = 0; i < MAX_CRITICALITY_LEVELS; i++)
    {
        for (j = 0; j < MAX_CRITICALITY_LEVELS; j++)
        {
            printf("Utilization at level %d and criticality %d is ",i,j);
            printf("%lf  ", total_utilisation[i][j]);
            printf("\n");

            if(i==j)
            {
                reqUi+=total_utilisation[i][j];
            }
        }
        printf("\n");
    }
    printf("Required Ui (at worst case) is %lf\n", reqUi);

    return reqUi;
}
