#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//declare local variables including a table structure to hold scheduling info
struct node{
    int id;
    int arrival;
    int totalCPU;
    int totalRemaining;
    int done;
    int startTime;
    int alreadyStarted;
    int endTime;
    int turnAroundTime;
} *table_ptr = NULL;

typedef struct node table_type;
int num_processes;

#define max(a,b) (a > b ? a : b)

/*********************************************************************************************/
//procedure to print contents of table
void printTable(){

    printf("ID\tArrival\tTotal\tStart\tEnd\tTurnAround");
    printf("\n-------------------------------------------------");
    for (int i = 0; i < num_processes; i++){
        printf("\np%d\t%d\t%d\t", table_ptr[i].id, table_ptr[i].arrival, table_ptr[i].totalCPU);
        if (table_ptr[i].done == 1){ //if the processes are complete
            printf("%d\t%d\t%d", table_ptr[i].startTime, table_ptr[i].endTime, table_ptr[i].turnAroundTime);
        }
    }
    printf("\n");
}

/*********************************************************************************************/
//procedure for entering the attributes for each process
void option1(){
    // prompts for number of processes, and allocates enough space for them in the table
    printf("Enter the total number of processes:");
    scanf("%d", &num_processes);
    table_ptr = (table_type *)calloc(num_processes, sizeof(table_type));

    //prompts for arrival time and total cycles for each process
    for(int i = 0; i < num_processes; i++){
        printf("\nEnter arrival time for process p%d:", i);
        table_ptr[i].id = i;
        scanf("%d", &table_ptr[i].arrival);
        printf("Enter total cycles for process p%d:", i);
        scanf("%d", &table_ptr[i].totalCPU);
    }
    printTable();
}

/*********************************************************************************************/
//procedure for FIFO
void option2(){
    int numDone = 0;
    int earliestArrival;
    int currCycle = 0;
    int earliestIndex;

    for(int i = 0; i < num_processes; i++){//sets everything to not done
        table_ptr[i].done = 0;
    }
    while (numDone < num_processes){ //keep going until all the processes are done
        earliestArrival = INT_MAX;
        for(int i = 0; i < num_processes; i++){
            if(table_ptr[i].done == 0){
                if(table_ptr[i].arrival < earliestArrival){//finds which process came first
                    earliestArrival = table_ptr[i].arrival;
                    earliestIndex = i;
                }
            }
        }
        //calculates each of the attributes according to FIFO
        table_ptr[earliestIndex].startTime = max(table_ptr[earliestIndex].arrival, currCycle);
        table_ptr[earliestIndex].endTime = table_ptr[earliestIndex].startTime + table_ptr[earliestIndex].totalCPU;
        table_ptr[earliestIndex].turnAroundTime = table_ptr[earliestIndex].endTime - table_ptr[earliestIndex].arrival;
        currCycle = table_ptr[earliestIndex].endTime;
        table_ptr[earliestIndex].done = 1;
        numDone ++;
    }
    printTable();
}

/*********************************************************************************************/
//procedure for SJF
void option3(){
    int numDone = 0;
    int lowestTotal;
    int currCycle = 0;
    int lowestIndex = 0;
    int atLeastOne;

    for(int i = 0; i < num_processes; i++){ //sets everything to not done
        table_ptr[i].done = 0;
    }
    while(numDone < num_processes){//keep going until all the processes are done
        lowestTotal = INT_MAX;
        atLeastOne = 0;

        for(int i = 0; i < num_processes; i++){
            if(table_ptr[i].done == 0){
                if((table_ptr[i].totalCPU < lowestTotal) && (table_ptr[i].arrival <= currCycle)){ //finds the process with the shortest job
                    atLeastOne = 1;
                    lowestTotal = table_ptr[i].totalCPU;
                    lowestIndex = i;
                }
            }
        }
        if(atLeastOne == 0){
            currCycle ++;
        }
        else {
            //calculates all attributes according to SJF
            table_ptr[lowestIndex].startTime = max(table_ptr[lowestIndex].arrival, currCycle);
            table_ptr[lowestIndex].endTime = table_ptr[lowestIndex].startTime + table_ptr[lowestIndex].totalCPU;
            table_ptr[lowestIndex].turnAroundTime = table_ptr[lowestIndex].endTime - table_ptr[lowestIndex].arrival;
            currCycle = table_ptr[lowestIndex].endTime;
            table_ptr[lowestIndex].done = 1;
            numDone++;
        }
    }
    printTable();
}

/*********************************************************************************************/
//Procedure for SRT
void option4(){
    int numDone = 0;
    int currCycle = 0;
    int lowestIndex = 0;
    int lowestRemaining;
    int atLeastOne;

    for(int i = 0; i < num_processes; i++){
        table_ptr[i].done = 0;
        table_ptr[i].alreadyStarted = 0;
        table_ptr[i].totalRemaining = table_ptr[i].totalCPU;
    }

    while(numDone < num_processes){
        lowestRemaining = INT_MAX;
        atLeastOne = 0;
        for(int i = 0; i < num_processes; i++){
            if(table_ptr[i].done == 0){
                if((table_ptr[i].totalRemaining < lowestRemaining) && (table_ptr[i].arrival <= currCycle)){//finds the process the lowest remaining time
                    atLeastOne = 1;
                    lowestIndex = i;
                    lowestRemaining = table_ptr[i].totalRemaining;
                }
            }
        }
        if(atLeastOne == 0){
            currCycle ++;
        }
        else{
            //set start time, end time, turnaround time, done fields for unscheduled process with the earliest arrival time
            if(table_ptr[lowestIndex].alreadyStarted == 0){
                table_ptr[lowestIndex].startTime = max(table_ptr[lowestIndex].arrival, currCycle);
                table_ptr[lowestIndex].alreadyStarted = 1;
            }
            table_ptr[lowestIndex].endTime = currCycle + 1;
            table_ptr[lowestIndex].turnAroundTime = table_ptr[lowestIndex].endTime - table_ptr[lowestIndex].arrival;
            table_ptr[lowestIndex].totalRemaining --;
            currCycle ++;
            if(table_ptr[lowestIndex].totalRemaining == 0){
                table_ptr[lowestIndex].done = 1;
                numDone ++;
            }
        }
    }
    printTable();
}

/*********************************************************************************************/
//frees the schedule table if it is not full
void option5(){
    if(table_ptr != NULL){
        free(table_ptr);
    }
}

//*********************************************************************************************
int main() {
    int choice = 0;

    while(choice != 5){
        printf("*********************************************************\n");
        printf("Welcome to Batch Process Scheduling!\n\n");
        printf("Please choose one of the options below:\n");
        printf("1. Enter parameters\n");
        printf("2. Schedule processes with FIFO algorithm\n");
        printf("3. Schedule processes with SJF algorithm\n");
        printf("4. Schedule processes with SRT algorithm\n");
        printf("5. Quit\n");
        printf("Selection:");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                option1();
                break;
            case 2:
                option2();
                break;
            case 3:
                option3();
                break;
            case 4:
                option4();
                break;
        }
    }
    option5();
    printf("Have a great day!");
    return 0;
}