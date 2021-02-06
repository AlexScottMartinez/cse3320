#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
  fork() a child and print a message from the parent and 
  a message from the child
*/

int main( void )
{
  pid_t pid = fork();

  if( pid == -1 )
  {
    // When fork() returns -1, an error happened.
    perror("fork failed: ");
    exit( EXIT_FAILURE );
  }
  else if ( pid == 0 )
  {
    // When fork() returns 0, we are in the child process.
    // printf("Hello from the child process\n");
    printf("10 seconds\n");
    sleep(1);
    printf("9 seconds\n");
    sleep(1);
    printf("8 seconds\n");
    sleep(1);
    printf("7 seconds\n");
    sleep(1);
    printf("6 seconds\n");
    sleep(1);
    printf("5 seconds\n");
    sleep(1);
    printf("4 seconds\n");
    sleep(1);
    printf("3 seconds\n");
    sleep(1);
    printf("2 seconds\n");
    sleep(1);
    printf("1 seconds\n");
    sleep(1);
    printf("0 seconds\n");
    fflush(NULL);
    exit( EXIT_SUCCESS );
  }
  else 
  {
    // When fork() returns a positive number, we are in the parent
    // process and the return value is the PID of the newly created
    // child process.
    int status;

    // Force the parent process to wait until the child process 
    // exits
    waitpid(pid, &status, 0 );
    printf("Countdown complete\n");
    fflush( NULL );
  }
  return EXIT_SUCCESS;
}
