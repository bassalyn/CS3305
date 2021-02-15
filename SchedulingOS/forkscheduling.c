#include <sys/wait.h>
#include <stdio.h>            //include libraries
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x,y,z;                    //initialize three global variables

void *sum_x_y ()              //function to sum compute sum x y and store in z
{
  z = x + y;
}
int main()                   //main function
{
    pid_t pid;               //declares variables and pid
    x = 10;
    y = 20;
    z = 0;

    pid = fork();             //creates child process
    if (pid > 0){
      wait(NULL);             //parent waits till child completes
    }else if (pid == 0){
      z = x + y;              //child sums x and y stores in z then exits
      exit(0);
    }                           //prints value of z after that excecutes
    printf("The value of z after the child is complete is: %d\n", z);
    pthread_t thread_1;          //creates thread
    pthread_create(&thread_1, NULL, sum_x_y, NULL); //thread sums x y
    pthread_join(thread_1, NULL);     //terminates thread, and prints value of z
    printf("The value of z after the thread is complete is: %d\n", z);

    return 0;

}
