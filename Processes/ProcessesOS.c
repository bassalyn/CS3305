/* Assignment 1 part1
by Bradley Assaly-Nesrallah
September 29, 2019
Parent and child proccesses, creates a parent that spawns two child proccesses
parent waits until child 1 done to create child 2
child 1 creates its own child 1.1
inside child 2 calls external external_program
uses fork, wait, getpid, getppid, execl functions

*/



#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>   //include libraries
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main()
{
  pid_t pid1, pid2;  //pid_t variables initialized
  pid1 = fork();      //calls fork to create child 1
  if (pid1 > 0){      // if parent process outputs parent and child PID
     printf("parent process (PID %d) created child_1 (PID %d)\n", getpid(), pid1);
     printf("parent (PID %d) is waiting for child_1 (PID%d) to complete before creating child_2\n", getpid(), pid1);
     wait(NULL);      // waits until child 1 complete before proceeding
  }
  if (pid1 == 0){     //if child 1
    pid2 = fork();    //creates child 1.1
    if (pid2 > 0){    //if child 1 prints child 1 and child 1.1 PID
      printf("child_1 (PID %d) created child_1.1 (PID %d)\n", getpid(), pid2);
      printf("child_1 (PID %d) is now completed\n", getpid());
    }
    exit(0);      //child 1 and child 1.1 completes
    }
  pid2 = fork();  //parent creates child 2
  if (pid2 > 0){    //if parent prints parent and child 2 PID
    printf("parent (PID %d) created child_2 (PID %d)\n", getpid(), pid2);
  }else if (pid2 == 0){    //if child 2 calls external program 
    printf("child_2 (PID %d) is calling an external program external_program.out and leaving child_2...\n", getpid(), pid1);
    execl("external_program.out", "external_program.out", NULL);
  }

return 0;

}
