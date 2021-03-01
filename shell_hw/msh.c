/*
  Name: Alexander Martinez
  ID:   100762509
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_COMMAND_SIZE 100  // Limits the user to only enter 100 characters in the shell
#define MAX_NUM_ARGUEMENTS 10 // For the user to only be able to have a maximum of 10 arguements the shell
#define WHITESPACE " \t\n"    // Defines a whitespace as a space, tab, or enter key 



  			      // Makes sure the inital fork happens, if not the program doesnt run,
  			      // if it does then the mshInput function runs 
int main()
{
  pid_t pid = fork();
  if( pid == -1)
  {
    perror("fork failed: ");
    _exit(1);
  }
  else if (pid == 0)	      // Parent process
  {
    mshInput();
    fflush(NULL);
    _exit(0);
  }
  else			       // Child process
  {
    int status;
    waitpid(pid, &status, 0);
    fflush(NULL);
  }
  return 0;
}

			      // This function shows the 'msh>' shell and reads the input the user
			      // enters and does the appropriate action
  int mshInput()
  {
    char *cmd_str = (char *)malloc(MAX_COMMAND_SIZE);
    while (1)
    {
      printf("msh> ");
      while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin));
      //if (cmd_str[0] == '!' ){
        //int index = atoi(cmd_str[1]);
      //}
      //if (cmd_str[0] == '\n' || cmd_str[0] == ' ' || cmd_str[0] == '\t'){
	//}
      char *token[MAX_NUM_ARGUEMENTS];
      int token_count = 0;
      char *arg_ptr;
      
      			      // Save a copy of the command line since strsep
      			      // will end up miving the pointer head 
      char *working_str = strdup(cmd_str);
      
      			      // we are going to move the working_str pointer so
      			      // keep track of its orginal value so we can deallocate
      			      // the correct amount at the end
      char *working_root = working_str;
      
      			      // Make sure the conditions for the whitespace and number of arguements are meet
      while (((arg_ptr = strsep(&working_str, WHITESPACE)) != NULL) && (token_count < MAX_NUM_ARGUEMENTS))
      {
        token[token_count] = strndup(arg_ptr, MAX_COMMAND_SIZE);
        		      // Implementation of the exit or quit functionality
        if (strcmp(token[token_count], "exit") == 0 || strcmp(token[token_count], "quit") == 0)
        {
          exit(0);
        }
        else if (strlen(token[token_count]) == 0)
        {
          token[token_count] = NULL;
        }
        token_count++;
      }
      			        // Implementation of the valid commands the user enters
      if (strcmp(token[0],"cd") == 0)
      {
        chdir(token[1]);
        continue;
      }
      else if (strcmp(token[0], WHITESPACE) == 0)
      {
      	continue;
      }
      else 
      {
      				// Implementation of the invaild coomands the user enters
        pid_t pid = fork();
        if (pid == 0)		// Parent process
        {
          int ret = execvp(token[0], &token[0]);
          if (ret == -1)
          {
            printf("%s: Command not found\n", token[0]);
            _exit(1);
          }
        }
        else {			// Child process
          int status;
          waitpid(pid, &status, 0);
          fflush(NULL);
        }
      }
    }
    return 0;
  }  
