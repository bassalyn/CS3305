/* Assignment 1 part2
by Bradley Assaly-Nesrallah
September 29, 2019
Inter Process communication
Takes two command line arguments, create parent and child that read X
child reads Y and sends Y to parent through a pipe, then parent produces a sum
uses fork, wait, getpid, getppid, pipe, write, read

*/

#include <sys/types.h>    //include libaries
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){  //command line arguments
  int x;
  int y;                    //declares variables x,y sum
  int sum;
  int fd[2];               //declares structures fd and pid
  pid_t pid;

  pipe(fd);                //creates pipe and prints PID
  printf("1.  A pipe is created for communication between parent (PID %d) and child\n", getpid());
  pid = fork();           //creates child process

  if(pid > 0) {           //if parent prints PID and child PID
    printf("2.  parent (PID %d) created a child (PID %d)\n", getpid(), pid);
    x = atoi(argv[1]);   //parent reads first arg as x variable
    printf("3.  parent (PID %d) Reading X = %d from the user\n", getpid(), x);
    wait(NULL);          //parent waits
    read(fd[0], &y, sizeof(y));  //reads y from the pipe
    printf("6.  parent (PID %d) Reading Y from the pipe (Y = %d)\n", getpid(), y);
    sum = x + y;           //computes sum and prints result 
    printf("7.  parent (PID %d) adding X + Y = %d\n", getpid(), sum);
  } else if (pid == 0){     //if child process
    y = atoi(argv[2]);      //reads y variable from command line arg 2 and prints
    printf("4.  child (PID %d) Reading Y = %d from the user\n", getpid(), y);
    write(fd[1],&y,sizeof(y));    // sends Y variable to pipe
    printf("5.  child (PID %d) Writing Y into the pipe\n", getpid());
    exit(0);                //child exits
  }
  return 0;
}
