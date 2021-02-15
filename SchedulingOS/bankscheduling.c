#include <stdio.h>
#include <stdlib.h>       //include libraries
#include <string.h>

struct ReadyQueue{        //define a struct for a ready quene
  int num;
  int size;
  int burst[40];
  int tq;                 //declare attributes for rqueue
};

int excecute (struct ReadyQueue rqueue){     //function to excecute the rqueue
  FILE *fptr;
  fptr = fopen("cpu_scheduling_output_file.txt", "a");
  int totaltime = 0;                        //opens the file for output
  int i;
  int turnaround[40];                       //declares attributes and counters
  int completedCount = 0;
  float avgSum = 0;

  printf("Ready Queue %d Applying FCFS Scheduling:\n\n", rqueue.num);
  fprintf(fptr,"Ready Queue %d Applying FCFS Scheduling:\n\n", rqueue.num);

  printf("Order of selection by CPU:\n");
  fprintf(fptr,"Order of selction by CPU:\n");

  for (i = 0; i< rqueue.size; i++){   //prints order of selection by FCFS sched
    printf("p%d ", i+1);
    fprintf(fptr,"p%d ", i+1);
    turnaround[i] = totaltime;        //fills the turnaround array with total time
    totaltime += rqueue.burst[i];     //increments the total time w burst time
  }

  printf("\n\nIndividual waiting times for each process:\n");
  fprintf(fptr,"\n\nIndividual waiting times for each process:\n");
  for (i = 0; i < rqueue.size; i++){
    printf("p%d = %d\n", i+1, turnaround[i]);    //prints wait time for each process
    fprintf(fptr,"p%d = %d\n", i+1, turnaround[i]);
    avgSum += turnaround[i];                    //gets total time
  }
                                          //prints average waiting time
  printf("\nAverage waiting time = %0.1f\n\n", avgSum/rqueue.size);
  fprintf(fptr,"\nAverage waiting time = %0.1f\n\n", avgSum/rqueue.size);

  printf("Ready Queue %d Applying SJF Scheduling:\n\n", rqueue.num);
  fprintf(fptr,"Ready Queue %d Applying SJF Scheduling:\n\n", rqueue.num);

  printf("Order of selection by CPU:\n");
  fprintf(fptr,"Order of selction by CPU:\n");  //prints order of selection SJF

  int tempMin = 1000000;
  int j,k;
  int SJFburst[rqueue.size];   //declares SJF attributes
  int SJFturnaround[rqueue.size];
  totaltime = 0;
  for (k = 0; k <rqueue.size; k++){
    SJFburst[k] = rqueue.burst[k];          //fills the burst array
  }

  while (completedCount < rqueue.size){
    for (i=0; i < rqueue.size; i++){
      if (SJFburst[i] != 0 && SJFburst[i] < tempMin) {
        tempMin = SJFburst[i];        //finds the order of jobs from shortest on
        j = i;
      }
    }
      SJFturnaround[j] = totaltime;
      totaltime += SJFburst[j];           //computes total time
      SJFburst[j] = 0;
      printf("p%d ", j+1);               //prints the process order
      fprintf(fptr, "p%d ", j+1);
      completedCount++;                 //increments completed count
      tempMin = 1000000;
  }

  printf("\n\nIndividual waiting times for each process:\n");
  fprintf(fptr,"\n\nIndividual waiting times for each process:\n");
  avgSum = 0;                   //computes wait time for each process
  completedCount = 0;
  tempMin = 1000000;
  while (completedCount < rqueue.size){
    for (i = 0; i < rqueue.size; i++){      //finds the turnaround for each
      if (SJFturnaround[i] != -1 && SJFturnaround[i] < tempMin){
        tempMin = SJFturnaround[i];
        j = i;
      }
    }
    printf("p%d = %d\n", j+1, SJFturnaround[j]);
    fprintf(fptr,"p%d = %d\n", j+1, SJFturnaround[j]);
    avgSum += SJFturnaround[j];     //prints the turnaround and adds it to total
    SJFturnaround[j] = -1;
    completedCount++;              //increments counter and resets variables
    tempMin = 1000000;
  }
  printf("\nAverage waiting time = %0.1f\n\n", avgSum/rqueue.size);
  fprintf(fptr,"\nAverage waiting time = %0.1f\n\n", avgSum/rqueue.size);

  printf("Ready Queue %d Applying RR Scheduling:\n\n", rqueue.num);
  fprintf(fptr,"Ready Queue %d Applying RR Scheduling:\n\n", rqueue.num);

  printf("Order of selection by CPU:\n");       //prints order selection for RR
  fprintf(fptr,"Order of selction by CPU:\n");

  int l;
  completedCount = 0;
  int completedorder[rqueue.size];     //initializes RR variables
  int RRturnaround[rqueue.size];
  int processtimer[rqueue.size];       //array for active processes
  for (i = 0; i < rqueue.size ; i++){
    processtimer[i] = 0;                //sets the timer and turnaround array 0
    RRturnaround[i] = 0;
  }

  for (i=0; completedCount < rqueue.size; i++){
    if (i == rqueue.size){            //follows the RR algorithm scedulign
      i = 0;                         //if size resets i to 0
    }
    if (rqueue.burst[i] > 0){
      printf("p%d ", i+1);         //if positive burst then up to tq burst
      fprintf(fptr, "p%d ", i+1);
      if (rqueue.burst[i] > rqueue.tq) {    //if burst > tq
        rqueue.burst[i] = rqueue.burst[i] - rqueue.tq;
        processtimer[i] = 1;                 //updates remaining cpu time and sets active
        for (int l = 0; l < rqueue.size ; l++){
          if (processtimer[l] == 1){
            RRturnaround[l] += rqueue.tq;   //for all active processes adds time
          }
        }
      }else{
        RRturnaround[i] += rqueue.burst[i];  //if burst smaller than tq
        processtimer[i] = 0;                //note inactive
        for (int k = 0; k < rqueue.size ; k++){
          if (processtimer[k] == 1){        //increment by burst for actives
            RRturnaround[k] += rqueue.burst[i];
          }
        }
        rqueue.burst[i] = 0;
        completedorder[i] = completedCount; //increase completed count and mark order
        completedCount++;
      }
    }
  }

  printf("\n\nTurnaround times for each process:\n");
  fprintf(fptr,"\n\nTurnaround times for each process:\n");
  for (i = 0; i < rqueue.size; i++){
    for (j = 0; j <rqueue.size; j++){       //prints the RR turnaround for each
      if (completedorder[j] == i){
        printf("p%d = %d\n", j+1, RRturnaround[j]);
        fprintf(fptr,"p%d = %d\n", j+1, RRturnaround[j]);
      }
    }

  }

  printf("\n");
  fprintf(fptr,"\n");
}

int main(){                           //main function
  remove("cpu_scheduling_output_file.txt");
  struct ReadyQueue queues[15];         //makes rqueue to handle 15 queues
  int size = 0;
  FILE* fptr;
  char delim[] = " ";                  //declares vars, ptr, and delim, char
  fptr = fopen("cpu_scheduling_input_file.txt", "r"); //opens input file
  char line[256];
  while (fgets(line, sizeof(line), fptr)){
    queues[size].num = size +1;             //parses the input file
    char *ptr = strtok(line, delim);        //tokenizes and reads the input file
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    queues[size].tq = atoi(ptr);           //converts to int from string
    queues[size].size = 0;
    int l =0;
    while (ptr != NULL){
      if (ptr != NULL){
        ptr = strtok(NULL, delim);         //handles null pointers
      }
      if (ptr != NULL){
        ptr = strtok(NULL, delim);
      }
      if (ptr != NULL){
        queues[size].burst[l] = atoi(ptr);
        queues[size].size++;              //updates the burst size as int
        l++;
      }
    }
    size++;                              //increases number of queues
  }
  for (int i = 0; i < size; i++){       //excecutes all of the rqueues with method
    excecute(queues[i]);
  }
}
