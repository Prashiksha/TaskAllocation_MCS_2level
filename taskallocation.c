#include "TaskAllocation.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include <stdbool.h>

void swap(int *x, int *y);
void sort(int Ui_all_tasks[], int task_id[], int n);

int MIN_DIFF = 100;
int HI_tasks=0,LO_tasks=0;

void task_allocation(task_set_struct *task_set, int min_cores)
{
    int total_tasks;
    int i;

    task *task_list;
    total_tasks = task_set->total_tasks;
    task_list = task_set->task_list;

    double HI_Ui[total_tasks], LO_Ui[total_tasks];
    int HItask_id[total_tasks],LOtask_id[total_tasks];
    int curr_level=HIGH;

    while(curr_level>-1)
    {
        for (i = 0; i < total_tasks; i++)
        {
            if(task_list[i].criticality_lvl==curr_level)  
            {
                if(curr_level==1)
                {
                    HI_Ui[HI_tasks]=task_list[i].util[curr_level];
                    HItask_id[HI_tasks]=task_list[i].task_number;
                    HI_tasks++;
                }
                else
                {
                    LO_Ui[LO_tasks]=task_list[i].util[curr_level];
                    LOtask_id[LO_tasks]=task_list[i].task_number;
                    LO_tasks++;
                }
            }
            
        }
        curr_level--;
    }   //end of while

    for(i=0;i<min_cores;i++)
        {
            cores[i].Ui=0.00;             //initial Ui of all cores=0
            cores[i].capacity=100; 
            cores[i].no_task_allocated=0;       //assuming initial capacity of each core =100 
        }

    allocate_critical_task(task_set, HI_Ui, min_cores, HI_tasks, HItask_id);
    print_task_list(task_set);

    allocation_LOmode(task_set, LO_Ui,min_cores, LO_tasks, LOtask_id);
    print_task_list(task_set);



}//end of task allocation function


/* Allocating HI critical tasks */

void allocate_critical_task(task_set_struct *task_set, double HI_Ui[] ,int number_of_cores, int number_of_HItasks, int taskID[])
{
    int x;
    int min_coreUi=__INT_MAX__;
    int min_task=__INT_MAX__;

    printf("--------------------------------------------------------\n");
    printf("Allocating tasks in HI mode\n\n");

    task *task_list;
    task_list = task_set->task_list;

    int UtilHI_tasks[number_of_HItasks];  //working with a copy of High Ui array HI_Ui
    int coreno,coreno_minUi=0,coreno_mintask=0,rem_tasks=number_of_HItasks;

    bool alloc_history[number_of_cores];

    for(int i=0;i<number_of_cores;i++)
        alloc_history[i]=false;

    for(int i=0;i<number_of_HItasks;i++)
    {
        UtilHI_tasks[i]=HI_Ui[i]*100;
    }

    sort(UtilHI_tasks, taskID, number_of_HItasks);

/*Choosing minimum between number of cores and number of HI tasks*/
    if(min_cores > number_of_HItasks)
        x=number_of_HItasks;
    else
        x=min_cores;

    for(int i=0;i<x;i++)
    {
        for(int k=0;k<number_of_cores;k++)
            alloc_history[k]=false;

        cores[i].capacity-= UtilHI_tasks[i];
        cores[i].Ui+=UtilHI_tasks[i];
        cores[i].no_task_allocated++;

        task_list[taskID[i]].core = i;
        rem_tasks--;

        alloc_history[i]=true;
    }

    /*Allocate remaining tasks to core having min Ui*/

    int index=number_of_HItasks-rem_tasks;

    for(int i=index;i<number_of_HItasks;i++)
    {
        //find coreno_mintask and coreno_minUi

        /*1.find core with min Util*/
        for(int itr=0;itr<number_of_cores;itr++)
        {

            if(cores[itr].Ui < min_coreUi)
            {
                if(alloc_history[itr]==false)
                {
                    min_coreUi=cores[itr].Ui;
                    coreno_minUi=itr;
                }
                
            }
                
        }
        //printf("Min Ui of core %d with Ui %d\n",coreno_minUi,min_coreUi);

        /*2.find core with min number of tasks out of all cores*/               /*multiple cores has same min no of tasks?*/
        /*Might be used when difference is very high in case of high clustering of tasks in one core*/

        for(int i=0;i<number_of_cores;i++)
        {
            if(cores[i].no_task_allocated < min_task)
            {
                if(alloc_history[i]==false)
                {
                    min_task=cores[i].no_task_allocated;
                    coreno_mintask=i;
                }
                
            }
                
        }
        //printf("Min no of task in core %d with %d tasks\n",coreno_mintask,min_task);

            /*allocate to the core with Min Ui*/
            coreno=coreno_minUi;
            for(int k=0;k<number_of_cores;k++)
                alloc_history[k]=false;

            cores[coreno].capacity-= UtilHI_tasks[i];
            cores[coreno].Ui+=UtilHI_tasks[i];
            cores[coreno].no_task_allocated++;

            task_list[taskID[i]].core = coreno;
            alloc_history[coreno]=true;

    }


    
}

/** ----------------------------------------------------------------------------------------------------------------------------*/

void allocate(task *task_list, double tasks[], int cores[], int number_of_tasks, int number_of_cores, int idx, int optimal_state[], int allocated_core_id[], bool *found, int taskId[]){

    /* BASE CONDITION */

    if(idx==number_of_tasks)
    {

        int mn = 100, mx = 0;
        for(int c=0; c<number_of_cores; c++)
        {
            if((100-cores[c])>mx) mx = 100-cores[c];
            if((100-cores[c])<mn) mn = 100-cores[c];
        }

        int curr_diff = mx-mn;

        /*
            mx          : maximum utilisation of all cores
            mn          : minimum utilization of all cores
            curr_diff   : current difference between max and min utilization
        */

        /* If current difference is the minimum so far then update the optimal state */ 

        if(curr_diff<MIN_DIFF)
        {
            MIN_DIFF = curr_diff;
            *found = true;

            for(int c=0; c<number_of_cores; c++)
                optimal_state[c] = cores[c];

            for(int acid=0; acid<number_of_tasks; acid++)
            {
                if(allocated_core_id[acid]>-1)
                    task_list[taskId[acid]].core = allocated_core_id[acid];
            }
        }

        return;
    }

    /* Checking all permutations by placing current task at all cores recursively */

    for(int c=0; c<number_of_cores; c++)
    {
        if(tasks[idx]<=cores[c]){
            cores[c] -= abs(tasks[idx]);
            allocated_core_id[idx] = c;

            allocate(task_list, tasks, cores, number_of_tasks, number_of_cores, idx+1, optimal_state, allocated_core_id, found, taskId);
            
            cores[c] += abs(tasks[idx]);
            allocated_core_id[idx] = -1;
        }
    }

}


void allocation_LOmode(task_set_struct *task_set, double LO_Ui[] ,int number_of_cores, int number_of_LOtasks, int LOtaskID[])
{
    printf("--------------------------------------------------------\n");
    printf("Allocating tasks in LO mode\n\n");
    int i,j=0,total_tasks;
    task *task_list;
    total_tasks = task_set->total_tasks;
    task_list = task_set->task_list;

    int LOUi_HItasks[HI_tasks], LOUi_HItask_ID[HI_tasks];

   for (i=0; i < total_tasks; i++)
    {
        if(task_list[i].criticality_lvl==HIGH)  
        {
            double temp = task_list[i].util[LOW];
            LOUi_HItasks[j]=temp*100;
            LOUi_HItask_ID[j]=task_list[i].task_number;
            j++;
        }
            
    }

/*
    for(i=0;i<j;i++)
    {
        printf("Values in LOUi_HItasks is %d\n",LOUi_HItasks[i]);
        printf("Values in LOUi_HItask_ID is %d\n\n",LOUi_HItask_ID[i]);

    }
*/
    
/*To update core values as per Low Mode Ui*/
    for(i=0;i<min_cores;i++)
        {
            cores[i].Ui=0.00;             //initial Ui of all cores=0
            cores[i].capacity=100; 
            cores[i].no_task_allocated=0;       //assuming initial capacity of each core =100 
        }

    /*
        for each HI task, look at allocated coreno, modify core capacity as per LOUi_HItasks
    */
    int coreno;
    int itr = 0; 
    for(int i=0;i<total_tasks;i++)
    {   
        if(task_list[i].criticality_lvl==HIGH) 
        {   
            coreno = task_list[i].core;
            cores[coreno].capacity -= LOUi_HItasks[itr];
            cores[coreno].Ui += LOUi_HItasks[itr];
            itr++;
        } 
    }

    int cores_temp[number_of_cores];
    int optimal_state[number_of_cores];
    int allocated_core_id_HI[HI_tasks];
    int allocated_core_id_LO[LO_tasks];


    for(int i=0; i<number_of_cores; i++)
    {
        cores_temp[i] = cores[i].capacity;
    }

    for(int i=0; i<number_of_cores; i++)
        optimal_state[i] = cores_temp[i];

    for(int i=0;i<HI_tasks;i++)
        allocated_core_id_LO[i]=-1;

    for(int i=0; i<LO_tasks; i++)
        LO_Ui[i] *= 100;

    bool combination_found = false;
    allocate(task_list, LO_Ui, cores_temp, LO_tasks, number_of_cores, 0, optimal_state, allocated_core_id_LO, &combination_found, LOtaskID);

    for(int i=0; i<number_of_cores; i++)
    {
        cores[i].capacity = optimal_state[i];
        cores[i].Ui = 100 - cores[i].capacity;
    }

}

/***************HELPER FUNCTIONS**************/

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

 
void sort(int Ui_all_tasks[], int task_id[], int n)
{
    int i, j;
    for (i = 0; i < n-1; i++)
    {    
        for (j = 0; j < n-i-1; j++)
        {
            if (Ui_all_tasks[j] < Ui_all_tasks[j+1])
            {
                swap(&Ui_all_tasks[j], &Ui_all_tasks[j+1]);
                swap(&task_id[j], &task_id[j+1]);
            }
        }
    }
}
