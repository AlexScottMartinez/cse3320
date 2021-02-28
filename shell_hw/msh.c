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

#define MAX_COMMAND_SIZE 255
#define MAX_NUM_ARGUEMENTS 12
#define WHITESPACE " \t\n"

int getuserinput();

int main()
{
  pid_t pid = fork();
  if( pid == -1)
  {
    perror("fork failed: ");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0)
  {
    getuserinput();
    fflush(NULL);
    exit(EXIT_SUCCESS);
  }
  else
  {
    int status;
    waitpid(pid, &status, 0);
    fflush(NULL);
  }
  return EXIT_SUCCESS;
}

  int getuserinput()
  {
    char *cmd_str = (char *)malloc(MAX_COMMAND_SIZE);
    while (1)
    {
      printf("msh> ");
      while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin));
      //if (cmd_str[0] == '!' ){
        //int index = atoi(cmd_str[1]);
      //}
      if (cmd_str[0] == '\n' || cmd_str[0] == ' ' || cmd_str[0] == '\t'){

      }
      char *token[MAX_NUM_ARGUEMENTS];
      int token_count = 0;
      char *arg_ptr;
      char *working_str = strdup(cmd_str);
      char *working_root = working_str;
      while (((arg_ptr = strsep(&working_str, WHITESPACE)) != NULL) && (token_count < MAX_NUM_ARGUEMENTS))
      {
        token[token_count] = strndup(arg_ptr, MAX_COMMAND_SIZE);
        if (strcmp(token[token_count], "exit") == 0 || strcmp(token[token_count], "quit") == 0)
        {
          exit(EXIT_SUCCESS);
        }
        if (strlen(token[token_count]) == 0)
        {
          token[token_count] = NULL;

        }
        token_count++;
      }
      if (strcmp(token[0],"cd") == 0)
      {
        chdir(token[1]);
        continue;

      }
      //else if{}
      //else if{}
      else {
        pid_t pid = fork();
        if (pid == 0)
        {
          int ret = execvp(token[0], &token[0]);
          if (ret == -1)
          {
            printf("%s: Command not found\n", token[0]);
            _exit(EXIT_FAILURE);

          }
        }
        else {
          int status;
          waitpid(pid, &status, 0);
          fflush(NULL);
        }
      }

    }
    return 0;
  }

    